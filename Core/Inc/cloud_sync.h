/**
 * @file cloud_sync.h
 * @brief 云同步：周期上报、断线续传、命令去重 ACK、双端状态同步
 */
#ifndef __CLOUD_SYNC_H__
#define __CLOUD_SYNC_H__

void cloud_sync_init(void);
void StartCloudTask(void *argument);

#endif /* __CLOUD_SYNC_H__ */
