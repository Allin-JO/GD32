#include "Host.h"
#include <rtthread.h>
#include <rthw.h>

#define HOST_BUF_SIZE  64

static uint8_t g_RxBuf[HOST_BUF_SIZE];
static uint8_t g_RxLen = 0;
static volatile uint8_t g_FrameReady = 0;
static char s_ParseBuf[HOST_BUF_SIZE];

static struct rt_semaphore s_RxSem;
static volatile uint8_t g_Control = 0;
static volatile uint8_t g_NewCmd = 0;
static volatile uint8_t g_TareRequest = 0;

static uint8_t Host_IsAscii(uint8_t ch)
{
    return (ch >= 0x20 && ch <= 0x7E) ? 1 : 0;
}

static char *Host_FindStr(char *haystack, const char *needle)
{
    char *h;
    const char *n;

    while (*haystack)
    {
        h = haystack;
        n = needle;
        while (*h && *n && *h == *n)
        {
            h++;
            n++;
        }
        if (*n == '\0')
        {
            return haystack;
        }
        haystack++;
    }
    return 0;
}

static int32_t Host_GetJsonValue(const char *key)
{
    char *p;
    int32_t value = -1;

    p = Host_FindStr(s_ParseBuf, key);
    if (p == 0)
    {
        return -1;
    }

    while (*p && *p != ':')
    {
        p++;
    }
    if (*p != ':')
    {
        return -1;
    }
    p++;
    while (*p == ' ' || *p == '\t')
    {
        p++;
    }
    if (*p >= '0' && *p <= '9')
    {
        value = 0;
        while (*p >= '0' && *p <= '9')
        {
            value = value * 10 + (*p - '0');
            p++;
        }
    }
    return value;
}

static void Host_ParseLine(void)
{
    int32_t v;

    v = Host_GetJsonValue("control");
    if (v == 0 || v == 1)
    {
        g_Control = (uint8_t)v;
        g_NewCmd = 1;
    }

    v = Host_GetJsonValue("tare");
    if (v == 1)
    {
        g_TareRequest = 1;
        g_NewCmd = 1;
    }
}

void Host_Init(void)
{
    GPIO_InitTypeDef gpio;
    USART_InitTypeDef usart;
    NVIC_InitTypeDef nvic;

    rt_sem_init(&s_RxSem, "hostrx", 0, RT_IPC_FLAG_FIFO);
    g_RxLen = 0;
    g_FrameReady = 0;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &usart);

    nvic.NVIC_IRQChannel = USART2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

void Host_IRQHandler(void)
{
    uint8_t ch;

    rt_interrupt_enter();
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        ch = (uint8_t)USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);

        if (g_FrameReady)
        {
            rt_interrupt_leave();
            return;
        }

        if (Host_IsAscii(ch) || ch == '\r' || ch == '\n')
        {
            if (ch == '\n' || ch == '}')
            {
                if (ch == '}' && g_RxLen < HOST_BUF_SIZE - 1)
                {
                    g_RxBuf[g_RxLen++] = ch;
                }
                g_RxBuf[g_RxLen] = '\0';
                if (g_RxLen > 0)
                {
                    g_FrameReady = 1;
                    rt_sem_release(&s_RxSem);
                }
                g_RxLen = 0;
            }
            else if (g_RxLen < HOST_BUF_SIZE - 1)
            {
                g_RxBuf[g_RxLen++] = ch;
            }
            else
            {
                g_RxLen = 0;
            }
        }
    }
    rt_interrupt_leave();
}

void Host_Process(void)
{
    rt_base_t level;

    if (rt_sem_take(&s_RxSem, RT_WAITING_FOREVER) != RT_EOK)
    {
        return;
    }

    level = rt_hw_interrupt_disable();
    rt_memcpy(s_ParseBuf, g_RxBuf, HOST_BUF_SIZE);
    g_FrameReady = 0;
    rt_hw_interrupt_enable(level);
    s_ParseBuf[HOST_BUF_SIZE - 1] = '\0';

    Host_ParseLine();
}

static void Host_SendByte(uint8_t byte)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, byte);
}

static void Host_SendString(const char *str)
{
    while (*str)
    {
        Host_SendByte((uint8_t)*str++);
    }
}

static void Host_SendInt32(int32_t num)
{
    char buf[12];
    uint8_t i = 0;
    uint8_t neg = 0;

    if (num < 0)
    {
        neg = 1;
        num = -num;
    }
    do
    {
        buf[i++] = (char)(num % 10) + '0';
        num /= 10;
    } while (num > 0 && i < (sizeof(buf) - 1));

    if (neg)
    {
        Host_SendByte('-');
    }
    while (i > 0)
    {
        Host_SendByte((uint8_t)buf[--i]);
    }
}

void Host_SendWeight(int32_t weight)
{
    Host_SendString("{\"weight\":");
    Host_SendInt32(weight);
    Host_SendString("}\r\n");
}

uint8_t Host_GetControl(void)
{
    return g_Control;
}

uint8_t Host_GetNewCmd(void)
{
    return g_NewCmd;
}

void Host_ClearNewCmd(void)
{
    g_NewCmd = 0;
}

uint8_t Host_GetTareRequest(void)
{
    return g_TareRequest;
}

void Host_ClearTareRequest(void)
{
    g_TareRequest = 0;
}
