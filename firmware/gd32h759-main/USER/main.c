#include <rtthread.h>
#include "app.h"
#include "sys.h"

/* GD32H759IMT6 main controller: Cortex-M7, GD32H7xx platform */
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



