/**
 * @file app_config.h
 * @brief 智慧农业业务配置宏（与 CubeMX 生成面无关）
 */
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* ---- 传感器 ---- */
#define APP_SENSOR_SIM              1     /* 1=仿真数据（默认可演示），0=接真实 DHT11 等 */
#define APP_SMA_WINDOW              8     /* 滑动平均窗口 */
#define APP_SENSOR_PERIOD_MS        1000

/* ---- 控制 ---- */
#define APP_CONTROL_PERIOD_MS       200
#define APP_HYST_TEMP_C             1.0f  /* 温度滞回带宽 */
#define APP_HYST_HUMI_PCT           5.0f
#define APP_HYST_LIGHT_PCT          10
#define APP_HYST_CO2_PPM            50.0f
/* 软 PWM：control 200ms 一次 tick，10 相位 → 视觉调光约 0.5Hz（演示用） */
#define APP_SOFT_PWM_PHASES         10

/* ---- 历史环形缓冲 ---- */
#define APP_HISTORY_CAP             64

/* ---- 健康监控 ---- */
#define APP_HEALTH_PERIOD_MS        500
#define APP_HEARTBEAT_TIMEOUT_MS    4000  /* 超时视为任务卡死 → 不喂狗 */
#define APP_IWDG_TIMEOUT_MS         4000

/* ---- 云 / L610 ---- */
#define APP_CLOUD_SIM               1     /* 1=无模组时用仿真 AT 应答跑通状态机 */
#define APP_CLOUD_DEVICE_ID         "agri_dev_demo01"
#define APP_CLOUD_PASSWORD          "demo_secret"
#define APP_CLOUD_BROKER            "127.0.0.1"
#define APP_CLOUD_PORT              1883
#define APP_CLOUD_TOPIC_PROP        "$oc/devices/" APP_CLOUD_DEVICE_ID "/sys/properties/report"
#define APP_CLOUD_TOPIC_CMD_SUB     "$oc/devices/" APP_CLOUD_DEVICE_ID "/sys/commands/#"
#define APP_CLOUD_TOPIC_CMD_RSP     "$oc/devices/" APP_CLOUD_DEVICE_ID "/sys/commands/response"
#define APP_BACKOFF_MIN_MS          1000
#define APP_BACKOFF_MAX_MS          60000
/* ACK 无应用层回执：仅 mqtt_publish 失败时计入 cmd_ack_timeout */
#define APP_CLOUD_STACK_WORDS       768

/* ---- UART ---- */
#define APP_L610_BAUD               115200
#define APP_UART_RX_BUF             512
#define APP_UART_TX_BUF             512

/* ---- 心跳槽位 ---- */
#define APP_HB_SENSOR               0
#define APP_HB_CONTROL              1
#define APP_HB_CLOUD                2
#define APP_HB_GUI                  3
#define APP_HB_NUM                  4

#endif /* __APP_CONFIG_H__ */
