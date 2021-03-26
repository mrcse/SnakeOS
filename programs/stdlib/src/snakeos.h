#ifndef SNAKEOS_H
#define SNAKEOS_H
#include <stddef.h>
#include <stdbool.h>


struct command_argument
{
    char argument[512];
    struct command_argument* next;
};

struct process_arguments
{
    int argc;
    char** argv;
};


void print(const char* filename);
int snakeos_getkey();

void* snakeos_malloc(size_t size);
void snakeos_free(void* ptr);
void snakeos_putchar(char c);
int snakeos_getkeyblock();
void snakeos_terminal_readline(char* out, int max, bool output_while_typing);
void snakeos_process_load_start(const char* filename);
struct command_argument* snakeos_parse_command(const char* command, int max);
void snakeos_process_get_arguments(struct process_arguments* arguments);
int snakeos_system(struct command_argument* arguments);
int snakeos_system_run(const char* command);

#endif