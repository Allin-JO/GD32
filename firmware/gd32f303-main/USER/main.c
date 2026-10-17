#include <rtthread.h>
#include "app.h"
#include "sys.h"

/* GD32F303RCT6 main controller: 256 KiB Flash, 48 KiB SRAM */
int Count = 0;
u8 Motor_Mode = 0;
u16 adc_val;
u32 voltage;
int encoder_A, encoder_B, encoder_C, encoder_D;

/* Incremental PI speed-loop gains */
float Velocity_KP = 300.0f;
float Velocity_KI = 300.0f;

int main(void)
{
    /* RT-Thread has already started the scheduler when main() is entered. */
    app_init();

    /* The application work is done by the created threads. */
    return 0;
}

