---
feature: agri-cloud
status: delivered
updated: 2026-07-11
branch: feat/agri-cloud
commits: 313966a..<head-pending>
---

# 端云协同智慧农业物联网系统（面试展示版）

## Report

**What was built** — 在既有 FreeRTOS+LVGL/GUI Guider 底座上补齐端云协同智慧农业业务层：滑动平均滤波采集、滞回闭环控制与声光报警、环形缓冲本地曲线+断线续传、L610 AT 状态机（指数退避 1s→60s）、MQTT/JSON 属性上报与命令 seq 去重 ACK、双端模式/执行器同步、IWDG+任务心跳健康监控。业务代码集中在 `Core/*_app.*` / `system_data` / `at_sm` / `cloud_sync`，CubeMX 重生成不覆盖。`APP_CLOUD_SIM=1` 无模组可演示全链路状态机。

**Verification** — `cmake --preset Debug && cmake --build --preset Debug`：0 error；FLASH 552428B（52.68%），RAM 121304B/128KB（92.55%）。独立 Review 后修复：堆峰值/任务栈、UART 静态 RX 缓冲、退避不重置、UI 阈值不再 100ms 回写、续传仅在 publish 成功后 mark_sent、SIM 下行改为 force_report。

**Journey log**
1. 真实残余工程是 `STM32F407_LCD_Test` 而非 `basic_example`；后者无 FreeRTOS 业务骨架。
2. HAL 包内缺 uart/iwdg 源，需从 STM32Cube_FW_F4_V1.28.3 拷贝进 Drivers。
3. FreeRTOS 堆与多任务栈峰值冲突：User_App_Init 时 defaultTask 未删，必须压栈或延后创建。
4. UI 双通道刷新若持续回写字典会冲掉云命令——改为「编辑提交 + 字典→控件回显」。
5. UI 历史曲线未接 lv_chart；Spec 中本地曲线能力以环形缓冲 API 为准，GUI 曲线控件可作后续增强。

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
| AT 状态机 | `at_sm.*` | 初始化/注册/MQTT/URC；指数退避 |
| mini JSON | `json_mini.*` | 打包属性/解析命令 |
| MQTT 客户端 | `mqtt_client.*` | 基于 L610 CMQTT 的连接/订阅/发布 |
| 云同步 | `cloud_sync.*` | 上报、命令去重 ACK、断线续传 |

### 任务模型

| 任务 | 周期 | 优先级 | 栈 |
|------|------|--------|-----|
| defaultTask（已有） | 一次 | AboveNormal | 8KB，自删除 |
| guiTask（已有） | ~5ms handler | Normal | 16KB |
| sensorTask | 1000ms | Normal | 4KB |
| controlTask | 200ms | AboveNormal | 2KB |
| cloudTask | 20ms 轮询 | BelowNormal | 4KB |
| healthTask | 500ms | High | 2KB |

共享：`g_DataMutex` 短临界区；云命令经 `cloud_sync` 写回字典。

### 云协议要点

- 主题：属性上报 `$oc/devices/{id}/sys/properties/report`；命令 `$oc/devices/{id}/sys/commands/#`
- 命令：`set_mode` / `set_threshold` / `set_actuator` / `force_report`；`seq` 去重 + ACK
- 断链：指数退避 1s→60s；恢复后按 `tx_cursor` 从环形缓冲补发
- 无真机时 `APP_CLOUD_SIM=1`：AT 层用仿真应答驱动状态机，保证代码路径可运行可讲

### HAL 挂点

- 启用 `HAL_UART_MODULE_ENABLED`、`HAL_IWDG_MODULE_ENABLED`
- UART/IWDG 初始化在 `User_App_Init` 中手写（不依赖 .ioc 重生成）；CMake 增加 `stm32f4xx_hal_uart.c`、`stm32f4xx_hal_iwdg.c`

### UI

复用 GUI Guider 三页；`custom.c` 增强：模式徽标、云状态、报警态、CO2 阈值联动、历史曲线页从环形缓冲刷 `lv_chart`（若 generated 无 chart，则用标签列表+简化曲线控件动态创建在 tab3/tab2 空位）。

### 面试重点难点栈（已确认，六位一体）

叙事主线：**裸机 AT 阻塞 → 多任务重构 → 本地闭环 → 云可靠链路 → 系统可运维**。

| # | 难点 | 挂载点 | 面试一句话 |
|---|------|--------|-----------|
| 1 | 架构隔离 + 优先级 + 互斥锁 | user_app / freertos 挂点 / system_data | 业务与 CubeMX 生成面完全隔离；按实时性排优先级；共享黑板用 Mutex 短临界区 |
| 2 | 滑动平均 + 滞回闭环 + 声光报警 | sensor_app / control_app / actuator | 窗口均值去毛刺；阈值带滞回防抖；越限本地闭环不依赖云 |
| 3 | 环形缓冲双用途 | history_ring | 同一队列：本地曲线 + 离线缓存；带 seq 与发送游标 |
| 4 | AT 状态机 + 指数退避 | at_sm / l610_uart | 应答与 URC 分流；断链 1s→60s 退避，恢复重订阅 |
| 5 | MQTT + 命令去重 ACK + 双端同步 | mqtt_client / cloud_sync / json_mini | seq 去重防重发；ACK 超时；云干预与本地自动双端状态一致 |
| 6 | 看门狗 + 任务健康监控 | health_app + IWDG | 心跳表全绿才喂狗；任一任务卡死触发复位并可观测 |

不纳入（避免突兀）：Flash 掉电存储、Host 单测框架、CPU/栈水位统计——与主叙事弱相关，按用户先前选择裁剪。

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

- 本地历史曲线：环形缓冲 API 已齐；GUI `lv_chart` 未嵌入现有 GUI Guider 布局（避免挤占三页控件），面试以 `history_ring` 双用途讲解为准。
- 真机 L610 的 CMQTTPUB 命令串仍为简化格式，默认 `APP_CLOUD_SIM=1`；接真模组时需按模组手册补全 topic/clientId 字段。
