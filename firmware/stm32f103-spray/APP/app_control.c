#include <rtthread.h>
#include "app_control.h"
#include "app_weight.h"
#include "Host.h"
#include "Servo.h"
#include "Spray.h"
#include "Pump.h"

#define CONTROL_THREAD_STACK 1024
#define CONTROL_THREAD_PRIO 8
#define CONTROL_PERIOD_TICKS 10

#define SERVO1_IDLE_POS 400
#define SERVO1_WORK_POS 350
#define SERVO2_IDLE_POS 700
#define SERVO2_WORK_POS 350
#define SERVO_MOVE_MS 1500
#define SERVO_SETTLE_MS 200

#define PUMP_INTERVAL_MS 50000
#define PUMP_ON_MS 200

typedef enum { SERVO_AT_IDLE = 0, SERVO_MOVING_TO_WORK, SERVO_MOVING_TO_IDLE } ServoState;

static volatile uint8_t s_work_mode = 0;
static volatile uint8_t s_desired_mode = 0;
static uint8_t s_pump_state = 0;
static ServoState s_servo_state = SERVO_AT_IDLE;
static rt_tick_t s_servo_deadline = 0;
static rt_tick_t s_pump_tick = 0;

uint8_t app_control_get_mode(void) { return s_work_mode; }

static void app_control_start_work(void) {
    Spray_OFF();
    Pump_OFF();
    s_pump_state = 0;
    s_work_mode = 0;

    SERVO_MoveTimeWrite(1, SERVO1_WORK_POS, SERVO_MOVE_MS);
    SERVO_MoveTimeWrite(2, SERVO2_WORK_POS, SERVO_MOVE_MS);

    s_servo_deadline = rt_tick_get() + rt_tick_from_millisecond(SERVO_MOVE_MS + SERVO_SETTLE_MS);
    s_servo_state = SERVO_MOVING_TO_WORK;
}

static void app_control_stop_work(void) {
    Spray_OFF();
    Pump_OFF();
    s_pump_state = 0;
    s_work_mode = 0;

    SERVO_MoveTimeWrite(1, SERVO1_IDLE_POS, SERVO_MOVE_MS);
    SERVO_MoveTimeWrite(2, SERVO2_IDLE_POS, SERVO_MOVE_MS);

    s_servo_deadline = rt_tick_get() + rt_tick_from_millisecond(SERVO_MOVE_MS + SERVO_SETTLE_MS);
    s_servo_state = SERVO_MOVING_TO_IDLE;
}

static void app_control_entry(void *parameter) {
    rt_tick_t next_tick = rt_tick_get();
    rt_tick_t now;

    (void)parameter;

    while (1) {
        if (Host_GetNewCmd()) {
            Host_ClearNewCmd();

            if (Host_GetTareRequest()) {
                Host_ClearTareRequest();
                app_weight_request_tare();
            }

            s_desired_mode = Host_GetControl();
        }

        if (s_desired_mode && s_servo_state == SERVO_AT_IDLE && !s_work_mode) {
            app_control_start_work();
        } else if (!s_desired_mode && (s_work_mode || s_servo_state == SERVO_MOVING_TO_WORK)) {
            app_control_stop_work();
        }

        now = rt_tick_get();
        if (s_servo_state == SERVO_MOVING_TO_WORK && (rt_int32_t)(now - s_servo_deadline) >= 0) {
            Spray_ON();
            Pump_ON();
            s_pump_state = 1;
            s_pump_tick = now;
            s_work_mode = 1;
            s_servo_state = SERVO_AT_IDLE;
        } else if (s_servo_state == SERVO_MOVING_TO_IDLE && (rt_int32_t)(now - s_servo_deadline) >= 0) {
            s_servo_state = SERVO_AT_IDLE;
        }

        if (s_work_mode) {
            if (s_pump_state) {
                if ((rt_tick_get() - s_pump_tick) >= rt_tick_from_millisecond(PUMP_ON_MS)) {
                    Pump_OFF();
                    s_pump_state = 0;
                    s_pump_tick = rt_tick_get();
                }
            } else if ((rt_tick_get() - s_pump_tick) >= rt_tick_from_millisecond(PUMP_INTERVAL_MS)) {
                Pump_ON();
                s_pump_state = 1;
                s_pump_tick = rt_tick_get();
            }
        }

        rt_thread_delay_until(&next_tick, CONTROL_PERIOD_TICKS);
    }
}

rt_thread_t app_control_start(void) {
    rt_thread_t tid =
        rt_thread_create("control", app_control_entry, RT_NULL, CONTROL_THREAD_STACK, CONTROL_THREAD_PRIO, 10);
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
    }
    return tid;
}
