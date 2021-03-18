#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "disk/streamer.h"
#include "fs/pparser.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour)
{
    if (c == '\n')
    {
        terminal_row++;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col++;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row++;
    }
    return;
}
void terminal_initialize()
{
    video_mem = (uint16_t *)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

void print(const char *str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

static struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main()
{
    terminal_initialize();
    print("Hello World!!\n");

    // Initialize the heap
    kheap_init();

    // Search and initialize the disks
    disk_search_and_init();

    // Initialize the interrupt descriptor table
    idt_init();

    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    // Switch to kernel paging chunk
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    // char * ptr = kzalloc(4096); 
    // paging_set(paging_4gb_chunk_get_directory(kernel_chunk), (void*)0x1000, (uint32_t)ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE);

    // Enable paging
    enable_paging();
//#########################################################
//            PAGINF TESTING
    // char * ptr2 = (char*) 0x1000;
    // ptr2[0] = 'A';
    // ptr2[1] = 'B';
    // print(ptr2);

    // print(ptr);
    // e.g
    // ptr = 0x100000 from kzalloc and we map 0x1000 to that
    // the if we change 0x1000 then it will affect 0x100000 directly
    // ptr  0x100000-> 0x100000
    // ptr3 0x1000  -> 0x100000  
//##########################################################


    // Enable the system interrupts
    enable_interrupts();

    // struct path_root* root_path = pathparser_parse("0:/bin/os.bin",NULL);
    // if(root_path)
    // {}
    // struct disk_stream* stream = diskstreamer_new(0);
    // diskstreamer_seek(stream, 0x201);
    // unsigned char c = 0;
    // diskstreamer_read(stream, &c, 1);
    // while(1) {}

}