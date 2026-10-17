#ifndef __APP_WEIGHT_H__
#define __APP_WEIGHT_H__

#include <rtthread.h>
#include <stdint.h>

rt_thread_t app_weight_start(void);
void app_weight_request_tare(void);
int32_t app_weight_get(void);

#endif /* __APP_WEIGHT_H__ */
