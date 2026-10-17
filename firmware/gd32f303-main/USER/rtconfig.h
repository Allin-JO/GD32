#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* RT-Thread 4.1.1, minimal configuration for GD32F303RCT6 */
#define RT_NAME_MAX                    8
#define RT_ALIGN_SIZE                  4
#define RT_THREAD_PRIORITY_MAX         32
#define RT_TICK_PER_SECOND             1000
#define IDLE_THREAD_STACK_SIZE         256

#define RT_USING_SEMAPHORE
#define RT_USING_HEAP
#define RT_USING_SMALL_MEM
#define RT_USING_SMALL_MEM_AS_HEAP

#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE             128

#define RT_USING_LIBC
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE      1536
#define RT_MAIN_THREAD_PRIORITY        10

#define RT_USING_OVERFLOW_CHECK
#define RT_DEBUG

#define RT_VER_NUM                     0x40101
#define ARCH_ARM
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4
#define RT_USING_CPU_FFS

#endif /* RT_CONFIG_H__ */

