#include "../../../drivers/VGA/VGA_text.h"
#include "gdt.h"

// Lets us access our ASM functions from our C code.
extern void gdt_flush(uint32_t);

// Internal function prototypes.
static void set_gdt_entry(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);
static void init_gdt_pointer();
static void init_gdt_entries();

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

void init_gdt()
{
   vga_write_string("Initializing GDT...\n");

   init_gdt_pointer();
   init_gdt_entries();
   gdt_flush((uint32_t) & gdt_ptr);

   vga_write_string("GDT was initialized successfully!\n");
}

/**
 * This method inits the gdt pointer
 */
static void init_gdt_pointer()
{
   gdt_ptr.size = (sizeof(gdt_entry_t) * 5) - 1; // The size of the table minus 1.
   gdt_ptr.base  = (uint32_t) & gdt_entries;
}

/**
 * This method inserts all the gdt entries into the table
 */
static void init_gdt_entries()
{
   set_gdt_entry(0, 0, 0, 0, 0);                // Null segment
   set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   set_gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   set_gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
}

/**
 * This method adds a new gdt entry to the table
 */
static void set_gdt_entry(int32_t interrupt_number, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
   gdt_entries[interrupt_number].base_low    = (base & 0xFFFF);
   gdt_entries[interrupt_number].base_middle = (base >> 16) & 0xFF;
   gdt_entries[interrupt_number].base_high   = (base >> 24) & 0xFF;

   gdt_entries[interrupt_number].limit_low   = (limit & 0xFFFF);

   gdt_entries[interrupt_number].flags = (limit >> 16) & 0x0F;
   gdt_entries[interrupt_number].flags |= gran & 0xF0;

   gdt_entries[interrupt_number].access      = access;
}