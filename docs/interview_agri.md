# 智慧农业项目面试讲解手册

> 工程：`STM32F407_LCD_Test` · 分支 `feat/agri-cloud`  
> 主线：**裸机 AT 阻塞 → 多任务重构 → 本地闭环 → 云可靠链路 → 系统可运维**

## 1. 一句话项目

STM32F407 + FreeRTOS 端云协同智慧农业系统：周期采集温湿度/CO2/光照，滑动平均滤波后本地阈值闭环控制补光/灌溉/通风；L610 走 AT 状态机对接华为云 IoT，断线指数退避重连并按环形缓冲续传；LVGL 三页现场人机交互；IWDG + 任务心跳保证卡死可恢复。

## 2. 架构（先画图）

```
main (CubeMX HAL) ── osKernelStart ──► defaultTask(自删除)
                                          │ User_App_Init()
                                          ├─ sensorTask  1s   Normal
                                          ├─ controlTask 200ms AboveNormal
                                          ├─ cloudTask   20ms  BelowNormal
                                          ├─ healthTask  500ms High
                                          └─ guiTask(已有) ~30fps Normal
```

- **业务隔离**：任务/锁/数据只在 `user_app` / `*_app` / `system_data`，CubeMX 重生成不覆盖。
- **共享黑板**：`g_SysData` + `g_DataMutex`，短临界区（先拷贝局部再算，算完再写回）。

## 3. 六个难点标准答法

### 3.1 为什么不用裸机 while(1)?

AT 指令等待 `OK`、传感器慢读、GUI 刷新周期差一个数量级。裸机要么忙等饿死 UI，要么状态机回调地狱。  
**答**：按实时性拆任务——控制 200ms 高于 GUI；采集 1s；云 20ms 轮询低优先级；初始化任务做完自删除。

### 3.2 滑动平均 + 滞回

- 窗口 N=8 的 SMA 抑制单点毛刺，面试可问“为什么不中值/卡尔曼”：成本与可解释性，农业慢变量足够。
- **滞回（hysteresis）**：温度 `>阈值` 开风扇，`<阈值-Δ` 才关，Δ=1℃，防止在阈值附近继电器抖动。湿度/光照/CO2 同理。

### 3.3 环形缓冲双用途

同一 64 槽结构：
1. 本地 UI 历史曲线快照；
2. 离线续传队列（`seq` 单调 + `sent_seq` 游标）。  
覆盖时若发送游标落后于最旧样本，自动前跳到最旧-1，避免重发已丢数据。  
**追问“满了怎么办”**：环形覆盖 + 游标前跳，不阻塞采集。

### 3.4 AT 状态机 + 指数退避

```
INIT→CPIN→CEREG→MQTT_CFG→CONN→SUB→ONLINE
                 ↘ 失败/超时 → BACKOFF: 1s→2s→…→60s → INIT
```

- 应答（OK/ERROR）与 URC（`+CMQTTPUBLISH`）分流解析；
- `APP_CLOUD_SIM=1` 无模组也能跑状态机（演示/CI）；接真模组改宏 + 填三元组。

### 3.5 MQTT 命令去重 + ACK + 双端同步

- 下行 JSON：`{"cmd":"set_mode","seq":N,"params":{...}}`
- `last_cmd_seq` 去重，重复直接丢并计数 `cmd_dup_drop`；
- 执行后发 ACK；超时窗口内未确认视为失败，等重连后云端可重推；
- 云 `set_actuator` 会先切手动，避免自动逻辑立刻覆盖——**双端状态一致**是关键点。

### 3.6 看门狗 + 任务健康

- 各任务 `health_beat(slot)`；
- `healthTask` 检查所有已注册槽位心跳，**全绿才** `HAL_IWDG_Refresh`；
- 任一任务死锁/长阻塞 → 停喂 → 4s 硬件复位。  
**追问**：为什么不直接在任务里喂狗？——防止“只有喂狗任务活着”的假健康。

## 4. 数据流（被问“端到端怎么走”）

```
传感器/仿真 → SMA → g_SysData → history_ring_push
                     ↓                    ↓
              control 闭环/报警      cloud 续传/上报华为云
                     ↓
              actuator GPIO/PWM
                     ↓
              gui 刷新 LVGL
```

## 5. 预期追问与答法

| 追问 | 答法要点 |
|------|----------|
| 互斥锁会优先级反转吗？ | FreeRTOS Mutex 带优先级继承；临界区极短 |
| 为什么 init 任务自删除？ | 避免空转占栈；职责一次完成 |
| LVGL 线程安全？ | `xGuiMutex` 包住 `lv_timer_handler` 与业务侧 UI 更新入口 |
| 没真机怎么证明云逻辑？ | `APP_CLOUD_SIM` 驱动同一状态机与命令路径 |
| RAM 为何偏紧？ | LVGL 双缓冲+多任务栈；可把 gui 栈 16KB→12KB 或减历史深度 |
| Flash 存参数？ | 本期刻意不做，避免叙事发散；可作为 roadmap |

## 6. 演示路径（有板子时）

1. 上电看三页 UI：数据/控制/阈值  
2. 改阈值触发风扇/水泵/报警  
3. 拔掉 L610（或等 SIM 断链）看 BACKOFF 与本地继续闭环  
4. 恢复后看续传（`uplink_ok` 增加）  
5. 故意卡死 sensor（调试死循环）看 IWDG 复位  

## 7. 文件地图（面试可点名）

| 模块 | 文件 |
|------|------|
| 任务入口 | `Core/Src/user_app.c` |
| 数据字典 | `Core/Src/system_data.c` |
| 滤波采集 | `Core/Src/sensor_app.c` |
| 滞回控制 | `Core/Src/control_app.c` |
| 环形缓冲 | `Core/Src/history_ring.c` |
| 健康/IWDG | `Core/Src/health_app.c` |
| AT 状态机 | `Core/Src/at_sm.c` |
| 云同步 | `Core/Src/cloud_sync.c` |
| JSON | `Core/Src/json_mini.c` |
