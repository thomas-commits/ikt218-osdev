#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <descriptor_tables.h>
#include <stdint.h>

#define ISR1 1
#define ISR2 2
#define ISR3 3
#define ISR4 4
#define ISR5 5
#define ISR6 6
#define ISR7 7
#define ISR8 8
#define ISR9 9
#define ISR10 10
#define ISR11 11
#define ISR12 12
#define ISR13 13
#define ISR14 14
#define ISR15 15
#define ISR16 16
#define ISR17 17
#define ISR18 18
#define ISR19 19
#define ISR20 20
#define ISR21 21
#define ISR22 22
#define ISR23 23
#define ISR24 24
#define ISR25 25
#define ISR26 26
#define ISR27 27
#define ISR28 28
#define ISR29 29
#define ISR30 30
#define ISR31 31


extern "C"{
    extern void isr0();
    extern void isr1();
    extern void isr2();
    extern void isr3();
    extern void isr4();
    extern void isr5();
    extern void isr6();
    extern void isr7();
    extern void isr8();
    extern void isr9();
    extern void isr10();
    extern void isr11();
    extern void isr12();
    extern void isr13();
    extern void isr14();
    extern void isr15();
    extern void isr16();
    extern void isr17();
    extern void isr18();
    extern void isr19();
    extern void isr20();
    extern void isr21();
    extern void isr22();
    extern void isr23();
    extern void isr24();
    extern void isr25();
    extern void isr26();
    extern void isr27();
    extern void isr28();
    extern void isr29();
    extern void isr30();
    extern void isr31();
}


typedef struct registers {
      uint32_t ds;                  // Data segment selector
      uint32_t edi, esi, ebp, useless_value, ebx, edx, ecx, eax; // Pushed by pusha.
      uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
      uint32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically.
  } registers_t;


// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef void (*isr_t)(registers*, void*);

void init_interrupts();
void register_interrupt_handler(uint8_t vector, isr_t handler, void* context);

struct int_handler {
  int num;
  isr_t handler;
  void *data;
};

static int_handler int_handlers[IDT_ENTRIES];

#endif