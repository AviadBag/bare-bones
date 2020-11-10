#include "../../drivers/VGA/VGA_text.h"

// descriptor_tables.c - Initialises the GDT and IDT, and defines the
// default ISR and IRQ handler.
// Based on code from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.

#include "descriptor_tables.h"

// Lets us access our ASM functions from our C code.
extern void gdt_flush(uint32_t);

// Internal function prototypes.
static void init_gdt();
static void gdt_set_entry(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void init_descriptor_tables()
{
   // Initialise the global descriptor table.
    vga_write_string("Init-ing GDT...\n");
    init_gdt();
    vga_write_string("GDT was init-ed successfully!\n");
}

static void init_gdt()
{
   gdt_ptr.size = (sizeof(gdt_entry_t) * 5) - 1; // The size of the table minus 1.
   gdt_ptr.base  = (uint32_t) & gdt_entries;

   gdt_set_entry(0, 0, 0, 0, 0);                // Null segment
   gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint32_t) & gdt_ptr);
}

// Set the value of one GDT entry.
static void gdt_set_entry(int32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
   gdt_entries[index].base_low    = (base & 0xFFFF);
   gdt_entries[index].base_middle = (base >> 16) & 0xFF;
   gdt_entries[index].base_high   = (base >> 24) & 0xFF;

   gdt_entries[index].limit_low   = (limit & 0xFFFF);

   gdt_entries[index].flags = (limit >> 16) & 0x0F;
   gdt_entries[index].flags |= gran & 0xF0;

   gdt_entries[index].access      = access;
}