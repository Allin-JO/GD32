#ifndef GD32H7XX_LIBOPT_H
#define GD32H7XX_LIBOPT_H

/* Compatibility with newer CMSIS versions used by ARMCLANG 6. */
#ifndef ARM_MPU_AP_NONE
#define ARM_MPU_AP_NONE       ((uint8_t)0x00U)
#define ARM_MPU_AP_PRIV       ((uint8_t)0x01U)
#define ARM_MPU_AP_URO        ((uint8_t)0x02U)
#define ARM_MPU_AP_FULL       ((uint8_t)0x03U)
#define ARM_MPU_AP_PRO        ((uint8_t)0x05U)
#define ARM_MPU_AP_RO         ((uint8_t)0x06U)
#define ARM_MPU_REGION_SIZE_32B   ((uint8_t)0x04U)
#define ARM_MPU_REGION_SIZE_64B   ((uint8_t)0x05U)
#define ARM_MPU_REGION_SIZE_128B  ((uint8_t)0x06U)
#define ARM_MPU_REGION_SIZE_256B  ((uint8_t)0x07U)
#define ARM_MPU_REGION_SIZE_512B  ((uint8_t)0x08U)
#define ARM_MPU_REGION_SIZE_1KB   ((uint8_t)0x09U)
#define ARM_MPU_REGION_SIZE_2KB   ((uint8_t)0x0AU)
#define ARM_MPU_REGION_SIZE_4KB   ((uint8_t)0x0BU)
#define ARM_MPU_REGION_SIZE_8KB   ((uint8_t)0x0CU)
#define ARM_MPU_REGION_SIZE_16KB  ((uint8_t)0x0DU)
#define ARM_MPU_REGION_SIZE_32KB  ((uint8_t)0x0EU)
#define ARM_MPU_REGION_SIZE_64KB  ((uint8_t)0x0FU)
#define ARM_MPU_REGION_SIZE_128KB ((uint8_t)0x10U)
#define ARM_MPU_REGION_SIZE_256KB ((uint8_t)0x11U)
#define ARM_MPU_REGION_SIZE_512KB ((uint8_t)0x12U)
#define ARM_MPU_REGION_SIZE_1MB   ((uint8_t)0x13U)
#define ARM_MPU_REGION_SIZE_2MB   ((uint8_t)0x14U)
#define ARM_MPU_REGION_SIZE_4MB   ((uint8_t)0x15U)
#define ARM_MPU_REGION_SIZE_8MB   ((uint8_t)0x16U)
#define ARM_MPU_REGION_SIZE_16MB  ((uint8_t)0x17U)
#define ARM_MPU_REGION_SIZE_32MB  ((uint8_t)0x18U)
#define ARM_MPU_REGION_SIZE_64MB  ((uint8_t)0x19U)
#define ARM_MPU_REGION_SIZE_128MB ((uint8_t)0x1AU)
#define ARM_MPU_REGION_SIZE_256MB ((uint8_t)0x1BU)
#define ARM_MPU_REGION_SIZE_512MB ((uint8_t)0x1CU)
#define ARM_MPU_REGION_SIZE_1GB   ((uint8_t)0x1DU)
#define ARM_MPU_REGION_SIZE_2GB   ((uint8_t)0x1EU)
#define ARM_MPU_REGION_SIZE_4GB   ((uint8_t)0x1FU)
#endif
#include "gd32h7xx_adc.h"
#include "gd32h7xx_gpio.h"
#include "gd32h7xx_misc.h"
#include "gd32h7xx_pmu.h"
#include "gd32h7xx_rcu.h"
#include "gd32h7xx_timer.h"
#include "gd32h7xx_syscfg.h"
#include "gd32h7xx_usart.h"

#endif /* GD32H7XX_LIBOPT_H */


