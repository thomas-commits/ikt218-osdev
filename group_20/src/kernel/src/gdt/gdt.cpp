#include "gdt.h"
extern "C"{
    #include "screen.h"
}


extern "C" {
  extern void gdt_flush(uint32_t gdt_ptr);
}

void start_gdt() {

    // Sets the GDT limit
    gdt_ptr.limit = sizeof(struct gdt_entry_t) * GDT_ENTRIES - 1;
    gdt_ptr.base = (uint32_t) &gdt;

    set_gdt_gate(0, 0, 0, 0, 0);                // Null segment
    set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    // Since GDT is the first thing that is called in boot.asm before main
    // We clear the screen to get rid of trash

    // Adds feedback to the terminal
    screenWrite("");
    screenWrite("Initializing GDT!......");

    // Load the GDT
    gdt_flush((uint32_t)&gdt_ptr);

    screenWrite("Initialized GDT!");
}

// Set the value of one GDT entry.
void set_gdt_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {

    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
    
}

// SOURCES: https://github.com/uiaict/ikt218-osdev/pull/1