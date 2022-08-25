/*
 * interrupt_handlers.c
 */

#include "../../include/common/string.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/io.h"

static interrupt_registers_t *interrupt_registers;
static interrupt_handler_f handlers[NUM_IRQS];
static interrupt_clearer_f clearers[NUM_IRQS];

extern void panic();

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

void move_exception_tables(); /* exception.S */
#define HALT asm("b halt")

void interrupts_init() {
    interrupt_registers = (interrupt_registers_t *)INTERRUPTS_PENDING;
    memset(handlers, 0, sizeof(interrupt_handler_f) * NUM_IRQS);
    memset(clearers, 0, sizeof(interrupt_clearer_f) * NUM_IRQS);
    interrupt_registers->irq_basic_disable = 0xffffffff; // disable all interrupts
    interrupt_registers->irq_gpu_disable1 = 0xffffffff;
    interrupt_registers->irq_gpu_disable2 = 0xffffffff;
    move_exception_tables();
    ENABLE_INTERRUPTS();
}

void register_irq_handler(irq_number_t irq_num, interrupt_handler_f handler, interrupt_clearer_f clearer) {
    uint32_t irq_pos;
    if (IRQ_IS_BASIC(irq_num)) {
        irq_pos = irq_num - 64;
        handlers[irq_num] = handler;
        clearers[irq_num] = clearer;
        interrupt_registers->irq_basic_enable |= (1 << irq_pos);
    }
    else if (IRQ_IS_GPU2(irq_num)) {
        irq_pos = irq_num - 32;
        handlers[irq_num] = handler;
        clearers[irq_num] = clearer;
        interrupt_registers->irq_gpu_enable2 |= (1 << irq_pos);
    }
    else if (IRQ_IS_GPU1(irq_num)) {
        irq_pos = irq_num;
        handlers[irq_num] = handler;
        clearers[irq_num] = clearer;
        interrupt_registers->irq_gpu_enable1 |= (1 << irq_pos);
    }
}

void undefined_instruction_handler()
{
    debug_printf("\r\n\r\n*** UNDEFINED INSTRUCTION ***");
    panic();
    HALT;
}

void software_interrupt_handler()
{
    debug_printf("\r\n\r\n*** SOFTWARE INTERRUPT ***");
    HALT;
}

void prefetch_abort_handler()
{
    debug_printf("\r\n\r\n*** PREFETCH ABORT ***");
    panic();
    HALT;
}

void data_abort_handler()
{
    debug_printf("\r\n\r\n*** DATA ABORT ***");
    panic();
    HALT;
}

static int handle_interrupts(volatile int32_t flags, int base_irq)
{
    if (flags)
    {
        int irq = base_irq;
        while (flags)
        {
            if ((flags & 1) && handlers[irq])
            {
//                debug_printf(">> Calling IRQ %d interrupt clearer\r\n", irq);
                clearers[irq]();
//                debug_printf(">> Calling IRQ %d interrupt handler\r\n", irq);
                ENABLE_INTERRUPTS();
                handlers[irq]();
                DISABLE_INTERRUPTS();
//                debug_printf("*** IRQ %d handled ***\r\n", irq);
                return 1;
            }
            flags >>= 1;
            irq++;
        }
    }
    return 0;
}

void interrupt_handler()
{
//    debug_printf("\r\n\r\n*** INTERRUPT ***\r\n");
    volatile uint32_t flags = interrupt_registers->irq_basic_pending;
    flags &= 0xff;
    if (handle_interrupts(flags, 64)) return;
    if (handle_interrupts(interrupt_registers->irq_gpu_pending2, 32)) return;
    if (handle_interrupts(interrupt_registers->irq_gpu_pending1, 0)) return;
    debug_printf("\r\n\r\n*** UNHANDLED INTERRUPT: %02x %08x %08x ***\r\n",
                 flags, interrupt_registers->irq_gpu_pending2, interrupt_registers->irq_gpu_pending1);
}

void fast_interrupt_handler()
{
    debug_printf("\r\n*** FAST INTERRUPT ***");
    HALT;
}
