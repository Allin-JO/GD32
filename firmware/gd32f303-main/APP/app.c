#include <rtthread.h>
#include "app.h"
#include "app_motion.h"
#include "app_display.h"
#include "sys.h"
#include "usart_x.h"

#define CMD_THREAD_STACK_SIZE 1536
#define CMD_THREAD_PRIORITY 9

static void app_command_entry(void *parameter) {
    (void)parameter;

    while (1) {
        /* Blocks on the UART frame semaphore, so no polling is needed. */
        USART2_Process();
    }
}

static void app_hardware_init(void) {
    delay_init(72);
    LED_Init();
    ADC1_Init();
    Key_Init();

    USART_RX_Init();          /* before enabling USART2/3 RX IRQ */
    Usart2_Init(USART2_BAUD); /* ESP8266 / JSON command link */
    Usart3_Init(USART3_BAUD); /* Bluetooth / spare command link */

    OLED_Init();
    Motor_Init();

    Encoder_TIM2_Init();
    Encoder_TIM3_Init();
    Encoder_TIM4_Init();
    Encoder_TIM5_Init();

    TIM8_Init(7199, 0); /* 10 kHz PWM */

    MOTOR_A.Target = 0.0f;
    MOTOR_B.Target = 0.0f;
    MOTOR_C.Target = 0.0f;
    MOTOR_D.Target = 0.0f;

    Car_State_Set(CAR_STOP, MOTOR_SPEED_DEFAULT);
}

int app_init(void) {
    rt_thread_t tid;

    app_hardware_init();

    tid = rt_thread_create("command", app_command_entry, RT_NULL, CMD_THREAD_STACK_SIZE, CMD_THREAD_PRIORITY, 10);
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
    } else {
        rt_kprintf("[app] command thread create failed\n");
    }

    if (app_motion_start() == RT_NULL) {
        rt_kprintf("[app] motion thread create failed\n");
    }

    if (app_display_start() == RT_NULL) {
        rt_kprintf("[app] display thread create failed\n");
    }

    rt_kprintf("\n[app] JingXun WeiShi RT-Thread firmware started\n");
    rt_kprintf("[app] kernel %d.%d.%d, tick=%d Hz\n", (RT_VERSION >> 16) & 0xFF, (RT_VERSION >> 8) & 0xFF,
               RT_VERSION & 0xFF, RT_TICK_PER_SECOND);

    return 0;
}
