//
// descriptor_tables.c - Initialises the GDT and IDT, and defines the
// default ISR and IRQ handler.
// Based on code from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
// http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html

#include "common.h"
#include "gdt.h"

// Defined in gdt.asm, flushes the new GDT
extern void gdt_flush(u32int);

// Internal function prototypes.
static void gdt_set_gate(s32int,u32int,u32int,u8int,u8int);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

// Sets up the GDT pointer structure and flushes the new GDT
void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;   // 5 entries (code and data segment for both kernel and user mode, plus null segment)
   gdt_ptr.base  = (u32int)&gdt_entries;

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   gdt_flush((u32int)&gdt_ptr);
}

// Set the value of one GDT entry.
static void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);       // Lower 16 bits of the base
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;   // Middle 8 bits of the base
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;   // Upper 8 bits of the base

   gdt_entries[num].limit_low   = (limit & 0xFFFF);      // Lower 16 bits of the limit
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;  // Set granularity and upper 4 bits of limit

   gdt_entries[num].granularity |= gran & 0xF0;          // Set high 4 bits of granularity
   gdt_entries[num].access      = access;                // Set access byte
} 