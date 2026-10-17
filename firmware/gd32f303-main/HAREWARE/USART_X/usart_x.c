#include "usart_x.h"
#include <rtthread.h>
#include <rthw.h>

u8 USART2_RX_BUF[USART2_RX_LEN];
u16 USART2_RX_CNT = 0;
u8 USART2_RX_FLAG = 0;

static struct rt_semaphore s_rx_sem;
static char s_rx_frame[USART2_RX_LEN];

void USART_RX_Init(void) {
    rt_sem_init(&s_rx_sem, "urx", 0, RT_IPC_FLAG_FIFO);
    USART2_RX_CNT = 0;
    USART2_RX_FLAG = 0;
    USART2_RX_BUF[0] = '\0';
}

void usart1_send(u8 data) {
    usart_data_transmit(USART0, data);
    while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
}

void usart2_send(u8 data) {
    usart_data_transmit(USART1, data);
    while (usart_flag_get(USART1, USART_FLAG_TBE) == RESET);
}

void usart3_send(u8 data) {
    usart_data_transmit(USART2, data);
    while (usart_flag_get(USART2, USART_FLAG_TBE) == RESET);
}

static void Usart_Config(uint32_t usart_periph, uint32_t bound) {
    usart_baudrate_set(usart_periph, bound);
    usart_word_length_set(usart_periph, USART_WL_8BIT);
    usart_stop_bit_set(usart_periph, USART_STB_1BIT);
    usart_parity_config(usart_periph, USART_PM_NONE);
    usart_hardware_flow_rts_config(usart_periph, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(usart_periph, USART_CTS_DISABLE);
    usart_receive_config(usart_periph, USART_RECEIVE_ENABLE);
    usart_transmit_config(usart_periph, USART_TRANSMIT_ENABLE);
    usart_enable(usart_periph);
}

void Usart1_Init(u32 bound) {
    GPIO_InitTypeDef gpio;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);

    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    Usart_Config(USART0, bound);
}

void Usart2_Init(u32 bound) {
    GPIO_InitTypeDef gpio;
    NVIC_InitTypeDef nvic;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART1);

    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    Usart_Config(USART1, bound);
    usart_interrupt_enable(USART1, USART_INT_RBNE);

    nvic.NVIC_IRQChannel = USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void Usart3_Init(u32 bound) {
    GPIO_InitTypeDef gpio;
    NVIC_InitTypeDef nvic;

    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_USART2);

    gpio_pin_remap_config(GPIO_USART2_PARTIAL_REMAP, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &gpio);

    Usart_Config(USART2, bound);
    usart_interrupt_enable(USART2, USART_INT_RBNE);

    nvic.NVIC_IRQChannel = USART2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

static void usart_rx_byte(u8 res) {
    if (USART2_RX_FLAG != 0) {
        return;
    }

    if ((USART2_RX_CNT == 0) && (res == '\r' || res == '\n' || res == ' ' || res == '\t')) {
        return;
    } else if (res == '}') {
        if (USART2_RX_CNT < (USART2_RX_LEN - 1)) {
            USART2_RX_BUF[USART2_RX_CNT++] = res;
        }
        USART2_RX_BUF[USART2_RX_CNT] = '\0';
        USART2_RX_CNT = 0;
        USART2_RX_FLAG = 1;
        rt_sem_release(&s_rx_sem);
    } else if (res == '\n' || res == '\r') {
        if (USART2_RX_CNT > 0) {
            USART2_RX_BUF[USART2_RX_CNT] = '\0';
            USART2_RX_CNT = 0;
            USART2_RX_FLAG = 1;
            rt_sem_release(&s_rx_sem);
        }
    } else if ((USART2_RX_CNT == 0) && (res >= '0') && (res <= '4')) {
        USART2_RX_BUF[0] = res;
        USART2_RX_BUF[1] = '\0';
        USART2_RX_FLAG = 1;
        rt_sem_release(&s_rx_sem);
    } else {
        if (USART2_RX_CNT < (USART2_RX_LEN - 1)) {
            USART2_RX_BUF[USART2_RX_CNT++] = res;
        } else {
            USART2_RX_CNT = 0;
        }
    }
}

int USART1_IRQHandler(void) {
    u8 res;

    rt_interrupt_enter();
    if (usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE) != RESET) {
        res = (u8)usart_data_receive(USART1);
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
        usart_rx_byte(res);
    }
    rt_interrupt_leave();
    return 0;
}

int USART2_IRQHandler(void) {
    u8 res;

    rt_interrupt_enter();
    if (usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE) != RESET) {
        res = (u8)usart_data_receive(USART2);
        usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE);
        usart_rx_byte(res);
    }
    rt_interrupt_leave();
    return 0;
}

void usart2_send_string(u8 *str) {
    while (*str) {
        usart2_send(*str++);
    }
}

void usart3_send_string(u8 *str) {
    while (*str) {
        usart3_send(*str++);
    }
}

void usart_send_ack(u8 *str) {
    usart2_send_string(str);
    usart3_send_string(str);
}

void usart_send_ack_byte(u8 data) {
    usart2_send(data);
    usart3_send(data);
}

void usart_send_ack_int(int num) {
    u8 buf[12];
    u8 i = 0;

    if (num < 0) {
        usart_send_ack_byte('-');
        num = -num;
    }
    do {
        buf[i++] = (u8)(num % 10) + '0';
        num /= 10;
    } while ((num > 0) && (i < 11));

    while (i > 0) {
        usart_send_ack_byte(buf[--i]);
    }
}

void usart2_send_int(int num) {
    u8 buf[12];
    u8 i = 0;

    if (num < 0) {
        usart2_send('-');
        num = -num;
    }
    do {
        buf[i++] = (u8)(num % 10) + '0';
        num /= 10;
    } while ((num > 0) && (i < 11));

    while (i > 0) {
        usart2_send(buf[--i]);
    }
}

u8 USART2_Parse_Command(u8 *cmd, u8 max_len) {
    u8 *p;
    u8 i = 0;

    p = (u8 *)strstr(s_rx_frame, "\"command\"");
    if (p == NULL) {
        return 1;
    }
    p += 9;

    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
    if (*p != ':') return 1;
    p++;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
    if (*p != '"') return 1;
    p++;

    while (*p && *p != '"' && i < (max_len - 1)) {
        cmd[i++] = *p++;
    }
    cmd[i] = '\0';

    return (*p == '"') ? 0 : 1;
}

u8 USART2_Parse_Int(u8 *key, int *val) {
    u8 *p;
    int sign = 1;
    int num = 0;
    u8 has_digit = 0;

    p = (u8 *)strstr(s_rx_frame, (char *)key);
    if (p == NULL) {
        return 1;
    }
    p += strlen((char *)key);

    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
    if (*p != ':') return 1;
    p++;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;

    if (*p == '-') {
        sign = -1;
        p++;
    }
    while ((*p >= '0') && (*p <= '9')) {
        num = num * 10 + (*p - '0');
        has_digit = 1;
        p++;
    }
    if (has_digit == 0) return 1;

    *val = sign * num;
    return 0;
}

void USART2_Process(void) {
    u8 cmd[16];
    int spd;
    u8 has_cmd, has_spd;
    u8 current_cmd;
    int current_speed;
    u8 update = 0;
    rt_base_t level;

    if (rt_sem_take(&s_rx_sem, RT_WAITING_FOREVER) != RT_EOK) {
        return;
    }

    level = rt_hw_interrupt_disable();
    rt_memcpy(s_rx_frame, USART2_RX_BUF, USART2_RX_LEN);
    USART2_RX_FLAG = 0;
    rt_hw_interrupt_enable(level);
    s_rx_frame[USART2_RX_LEN - 1] = '\0';

    rt_kprintf("USART2 RX: %s\n", s_rx_frame);
    Car_State_Get(&current_cmd, &current_speed);

    if ((s_rx_frame[0] >= '0') && (s_rx_frame[0] <= '4') && (s_rx_frame[1] == '\0')) {
        switch (s_rx_frame[0]) {
        case '0':
            current_cmd = CAR_STOP;
            break;
        case '1':
            current_cmd = CAR_FORWARD;
            break;
        case '2':
            current_cmd = CAR_LEFT;
            break;
        case '3':
            current_cmd = CAR_RIGHT;
            break;
        case '4':
            current_cmd = CAR_BACKWARD;
            break;
        }
        Car_State_Set(current_cmd, current_speed);
        usart_send_ack((u8 *)"OK:");
        usart_send_ack_byte(s_rx_frame[0]);
        usart_send_ack((u8 *)"\r\n");
        return;
    }

    has_spd = (USART2_Parse_Int((u8 *)"\"speed\"", &spd) == 0);
    if (has_spd) {
        if (spd < 0) spd = 0;
        if (spd > SPEED_MAX) spd = SPEED_MAX;
        current_speed = spd;
        update = 1;
        usart_send_ack((u8 *)"SPD:");
        usart_send_ack_int(current_speed);
        usart_send_ack((u8 *)"\r\n");
    }

    has_cmd = (USART2_Parse_Command(cmd, sizeof(cmd)) == 0);
    if (has_cmd) {
        if (strcmp((char *)cmd, "forward") == 0) {
            current_cmd = CAR_FORWARD;
            update = 1;
            usart_send_ack((u8 *)"OK:forward\r\n");
        } else if (strcmp((char *)cmd, "backward") == 0) {
            current_cmd = CAR_BACKWARD;
            update = 1;
            usart_send_ack((u8 *)"OK:backward\r\n");
        } else if (strcmp((char *)cmd, "left") == 0) {
            current_cmd = CAR_LEFT;
            update = 1;
            usart_send_ack((u8 *)"OK:left\r\n");
        } else if (strcmp((char *)cmd, "right") == 0) {
            current_cmd = CAR_RIGHT;
            update = 1;
            usart_send_ack((u8 *)"OK:right\r\n");
        } else if (strcmp((char *)cmd, "stop") == 0) {
            current_cmd = CAR_STOP;
            update = 1;
            usart_send_ack((u8 *)"OK:stop\r\n");
        } else {
            usart_send_ack((u8 *)"ERR:unknown command\r\n");
        }
    }

    if (!has_cmd && !has_spd) {
        usart_send_ack((u8 *)"ERR:json format error\r\n");
        rt_kprintf("  -> json format error\n");
    }

    if (update) {
        Car_State_Set(current_cmd, current_speed);
        rt_kprintf("  -> command=%d speed=%d\n", current_cmd, current_speed);
    }
}
