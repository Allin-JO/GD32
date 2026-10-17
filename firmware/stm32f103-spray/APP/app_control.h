#ifndef __APP_CONTROL_H__
#define __APP_CONTROL_H__

#include <rtthread.h>

rt_thread_t app_control_start(void);
uint8_t app_control_get_mode(void);

#endif /* __APP_CONTROL_H__ */
