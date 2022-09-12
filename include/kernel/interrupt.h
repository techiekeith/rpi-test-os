/*
 * interrupt.h
 */

#pragma once

#include "../common/stdint.h"
#include "peripheral.h"

#define INTERRUPTS_BASE             (peripheral_base + INTERRUPTS_OFFSET)
#define INTERRUPTS_PENDING          (INTERRUPTS_BASE + 0x200)

#define NUM_IRQS 72
#define IRQ_IS_BASIC(irq_number)    ((irq_number) & 0x40) /* IRQs 64-71 */
#define IRQ_IS_GPU2(irq_number)     ((irq_number) & 0x20) /* IRQs 32-63 */
#define IRQ_IS_GPU1(irq_number)     ((irq_number) < 0x20) /* IRQs  0-31 */

typedef enum {
    SYSTEM_TIMER_1      = 0x01,
    SYSTEM_TIMER_3      = 0x03,
    USB_CONTROLLER      = 0x09,
    DMA0_INT            = 0x10,
    GPU0_AUX            = 0x1d,
    I2C_SPI_SLV_INT     = 0x2b,
    GPU1_PWA0           = 0x2d,
    GPU1_PWA1           = 0x2e,
    GPU1_SMI            = 0x30,
    GPIO_INT0           = 0x31,
    GPIO_INT1           = 0x32,
    GPIO_INT2           = 0x33,
    GPIO_INT3           = 0x34,
    I2C_INT             = 0x35,
    SPI_INT             = 0x36,
    PCM_INT             = 0x37,
    UART_INT            = 0x39,
    ARM_TIMER           = 0x40,
    ARM_MAILBOX         = 0x41,
    ARM_DOORBELL_0      = 0x42,
    ARM_DOORBELL_1      = 0x43,
    ARM_GPU_0           = 0x44,
    ARM_GPU_1           = 0x45,
    ARM_ACCESS_ERROR_1  = 0x46,
    ARM_ACCESS_ERROR_0  = 0x47,
} irq_number_t;

typedef struct {
    uint32_t irq_basic_pending;
    uint32_t irq_gpu_pending1;
    uint32_t irq_gpu_pending2;
    uint32_t fiq_control;
    uint32_t irq_gpu_enable1;
    uint32_t irq_gpu_enable2;
    uint32_t irq_basic_enable;
    uint32_t irq_gpu_disable1;
    uint32_t irq_gpu_disable2;
    uint32_t irq_basic_disable;
} interrupt_registers_t;

__inline__ int INTERRUPTS_ENABLED(void) {
    int res;
    __asm__ __volatile__("mrs %[res], CPSR": [res] "=r" (res)::);
    return ((res >> 7) & 1) == 0;
}

__inline__ void ENABLE_INTERRUPTS(void) {
    if (!INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsie i");
    }
}

__inline__ void DISABLE_INTERRUPTS(void) {
    if (INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsid i");
    }
}

void interrupts_init();
typedef void (*interrupt_handler_f)(void *);
typedef void (*interrupt_clearer_f)(void *);
void register_irq_handler(irq_number_t irq_num, interrupt_handler_f handler, void *handler_param,
                          interrupt_clearer_f clearer, void *clearer_param);
void unregister_irq_handler(irq_number_t irq_num);
