---
feature: agri-cloud
status: delivered
updated: 2026-07-11
branch: feat/agri-cloud
commits: 313966a..a707ddc
---

# 端云协同智慧农业物联网系统（面试展示版）

## Report

**What was built** — 在既有 FreeRTOS+LVGL/GUI Guider 底座上补齐端云协同智慧农业业务层：滑动平均滤波、滞回闭环与声光报警、环形缓冲双用途（本地快照+断线续传）、L610 AT 状态机（显式 next_on_ok、指数退避 1s→60s、SIM/真机共用 INIT→…→ONLINE 序列）、MQTT/JSON 上报与命令 seq 去重、双端模式/执行器同步、IWDG+任务心跳。`APP_CLOUD_SIM=1` 默认无模组演示全链路。

**Verification** — `cmake --preset Debug && cmake --build --preset Debug`：0 error；FLASH 552152B（52.66%），RAM 121232B/128KB（92.49%）。四轮独立 Review：R1 堆/UART RX/退避/UI 回写；R2 SIM 链坍缩、真机 INIT、阈值标签覆写；R3 OK 越态、pub 截断、ACK 计数语义；R4 **clean（无 critical/major）**。

**Journey log**
1. 真实残余工程是 `STM32F407_LCD_Test` 而非 `basic_example`。
2. HAL 包缺 uart/iwdg 源，从 STM32Cube_FW_F4_V1.28.3 拷入。
3. FreeRTOS 堆与多任务栈峰值冲突：压栈 + 32KB 堆。
4. AT 状态机禁止用命令字符串猜状态——必须显式 `s_next_on_ok`。
5. OK/ERROR 仅在 `WAIT_OK` 处理；UI 状态徽标并入数据行，不碰 tab3 字段标题。

## [S1] Problem

现有 `STM32F407_LCD_Test` 已具备 FreeRTOS + LVGL/GUI Guider + 业务隔离骨架（user_app/sensor_app/control_app/system_data），但：传感器为裸随机数、无滑动平均/报警/历史、无云端链路、无健康看门狗、执行器与阈值不完整。需在该底座上补齐简历描述的端云协同能力，并保证 CMake/Ninja 全量编译通过，便于面试讲解。

## [S2] Design

### 基线（已存在，保持不动或仅挂 USER CODE）

- STM32F407ZGT6 + FreeRTOS V10.3.1 (CMSIS-RTOS2) + LVGL v9.5 + GUI Guider 三页 Tabview
- `main.c`：HAL/CubeMX 初始化 → `osKernelStart()`；`freertos.c` defaultTask 自删除前调用 `User_App_Init()`
- 业务代码集中在 `Core/Src|Inc` 的 `*_app.*`、`system_data.*`，CubeMX 重生成不覆盖

### 扩展模块

| 模块 | 路径 | 职责 |
|------|------|------|
| 数据字典扩展 | `system_data.*` | 传感/阈值/执行器/模式/云状态/报警/seq |
| 滑动平均+采集 | `sensor_app.*` | 窗口8滤波；默认仿真，可选 DHT11 |
| 闭环控制 | `control_app.*` | 滞回阈值；自动/手动；声光报警 |
| 执行器 BSP | `actuator.*` | 风扇/水泵/补光/蜂鸣/告警灯 GPIO+软PWM |
| 历史环形缓冲 | `history_ring.*` | 64槽；本地曲线+断网续传游标 |
| 健康监控 | `health_app.*` | 心跳表+IWDG；任一任务卡死不喂狗 |
| L610 UART | `l610_uart.*` | USART1 中断收发环形缓冲 |
| AT 状态机 | `at_sm.*` | 初始化/注册/MQTT/URC；指数退避；next_on_ok |
| mini JSON | `json_mini.*` | 打包属性/解析命令（字段 has 位） |
| MQTT 客户端 | `mqtt_client.*` | 基于 L610 CMQTT 的连接/订阅/发布 |
| 云同步 | `cloud_sync.*` | 上报、命令去重 ACK、断线续传 |

### 任务模型

| 任务 | 周期 | 优先级 | 栈 |
|------|------|--------|-----|
| defaultTask（已有） | 一次 | AboveNormal | 6KB，自删除 |
| guiTask（已有） | ~5ms handler | Normal | 12KB |
| sensorTask | 1000ms | Normal | 3KB |
| controlTask | 200ms | AboveNormal | 2KB |
| cloudTask | 20ms 轮询 | BelowNormal | 3KB |
| healthTask | 500ms | High | 2KB |

共享：`g_DataMutex` 短临界区；云命令经 `cloud_sync` 写回字典。

### 云协议要点

- 主题：属性上报 `$oc/devices/{id}/sys/properties/report`；命令 `$oc/devices/{id}/sys/commands/#`
- 命令：`set_mode` / `set_threshold` / `set_actuator` / `force_report`；`seq` 去重 + ACK（发布失败计数）
- 断链：指数退避 1s→60s；恢复后按游标从环形缓冲补发
- 无真机时 `APP_CLOUD_SIM=1`：AT 层用仿真应答驱动完整状态链

### HAL 挂点

- 启用 `HAL_UART_MODULE_ENABLED`、`HAL_IWDG_MODULE_ENABLED`
- UART/IWDG 初始化在 `User_App_Init` 中手写；CMake 增加 uart/iwdg 驱动源

### UI

复用 GUI Guider 三页；状态徽标并入 TEMP/LIGHT/CO2 数据行；阈值编辑退出时提交，字典→spinbox 回显。

### 验证边界

- `cmake --preset Debug && cmake --build --preset Debug` 0 error
- 真机烧录/华为云联调不在本次范围（配置宏预留设备三元组）

## [S3] Out of Scope

- .ioc 重生成、Flash 掉电存储、Host 单测框架、CPU/栈水位统计
- 修改 `basic_example` 仓库
- 重做 GUI Guider 工程文件（只改 custom/generated 允许的 C 代码）

## Tasks

- [x] T1: 环境与任务底座（分支/spec/CMake/HAL 开关） — acceptance: 工程可配置编译目标完整 (covers: S2 基线)
- [x] T2: 数据字典+滑动平均采集+执行器+闭环控制+报警 — acceptance: 滤波与控制路径完整、执行器 API 齐 (covers: S2)
- [x] T3: 环形缓冲+健康监控+IWDG — acceptance: push/pop/续传游标与心跳喂狗逻辑完整 (covers: S2)
- [x] T4: AT状态机+MQTT+JSON+云同步 — acceptance: 退避/去重/ACK/续传代码路径完整，SIM 可驱动 (covers: S2)
- [x] T5: UI 绑定增强+全量编译+面试文档 — acceptance: 构建 0 error；`docs/interview_agri.md` 完成 (covers: S2)

## S2 偏差说明（Review 后修订）

- 本地历史曲线：环形缓冲 API 已齐；GUI `lv_chart` 未嵌入现有 GUI Guider 布局，面试以 `history_ring` 双用途讲解为准。
- 真机 L610 的 CMQTTPUB 命令串仍为简化格式，默认 `APP_CLOUD_SIM=1`；接真模组时需按模组手册补全 topic/clientId 字段。
- ACK 无应用层“云端已确认”回执：`cmd_ack_timeout` 仅在 ACK `mqtt_publish` 失败时累加。
- 真机上行 in-flight 期间到达的下行 ACK 会失败并计数（指标噪音）；功能靠 seq 去重保证。
