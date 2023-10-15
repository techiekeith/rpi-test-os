/*
 * shell.c
 */

#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/io.h"

DEBUG_INIT("shell");

#define COMMAND_BUFFER_SIZE 1024
#define MAX_ARGS 127
static char command_buffer[COMMAND_BUFFER_SIZE];
static char *args[MAX_ARGS + 1];

const char *divider = "\u2014\u2014\u2014\u2014\u2014\u2014\u2014\u2014"
                      "\u2014\u2014\u2014\u2014\u2014\u2014\u2014\u2014"
                      "\u2014\u2014\u2014\u2014\u2014\u2014\u2014\u2014"
                      "\u2014\u2014\u2014\u2014\u2014\u2014\u2014\u2014"
                      "\u2014\u2014\u2014\u2014\u2014\u2014\u2014\u2014"
                      "\u2014\u2014\u2014\u2014\u2014\u2014\u2014\u2014"
                      "\u2014\u2014\u2014\u2014\u2014\u2014\u2014\u2014"
                      "\u2014\u2014\u2014\u2014\u2014\u2014\u2014\u2014";

static void show_help_text()
{
    puts("\r\nCommands:\r\n");
    puts("\tbgcolor <#> - set background color\r\n");
    puts("\tborder (off|<#>) - set the display border\r\n");
    puts("\tclear - clear the screen\r\n");
    puts("\tcharset [<#>] - show/set character set\r\n");
    puts("\tcolor <#> - set foreground color\r\n");
    puts("\tdmacopy <src> <dest> <size> - copy <size> bytes from <src> to <dest>\r\n");
    puts("\tdump (stack|<start_addr>) (<end_addr>|+<size>) - memory dump\r\n");
    puts("\tgfxdma [(on|off)] - show/set graphics DMA\r\n");
    puts("\thalt - halt the shell\r\n");
    puts("\thelp - this text\r\n");
    puts("\tmailbox (list|<method_name|method_id>) [...] - mailbox methods\r\n");
    puts("\tmemory - show memory usage\r\n");
    puts("\tmode [<width> <height> <depth>] - show/set display mode\r\n");
    puts("\tout [(fb|uart|all)] - show/set output\r\n");
    puts("\tpalette [<palette-mode> <rgb-mode>] - show/set colour palette\r\n");
    puts("\tpeek[32] <addr> - read memory at <addr>\r\n");
    puts("\tpixels [(rgb|bgr)] - show/set pixel order\r\n");
    puts("\tpoke[32] <addr> <value> - write memory at <addr>\r\n");
    puts("\treset - reset the shell\r\n");
    puts("\ttest - run tests\r\n");
    puts("\ttimers - get system timer information\r\n");
}

static void bad_command()
{
    puts("\r\nBad command\r\n");
}

static void escape()
{
    puts("\r\nEscape\r\n");
}

static int getline(char *buf, int buflen)
{
    int i = 0;
    char c;
    do
    {
        c = getc();
        if ((c == 8 || c == 127) && i > 0)
        {
            putc(127);
            i--;
        }
        if (c == 27)
        {
            i = -1;
        }
        if ((c >= 32 && c < 127) && i < buflen - 1)
        {
            putc(c);
            buf[i] = c;
            i++;
        }
    }
    while (c != '\n' && c != '\r' && c != 27);
    if (i >= 0) buf[i] = 0;
    putc('\r');
    putc('\n');
    return i;
}

static int split(char *str, int delim, int max_args, char **argp)
{
    int arg_count = 0;
    char *next_arg;
    while (*str == delim) str++;
    while (arg_count < (max_args - 1) && ((next_arg = strchr(str, delim)) != NULL))
    {
        *next_arg++ = '\0';
        while (*next_arg == delim) next_arg++;
        argp[arg_count++] = str;
        str = next_arg;
    }
    argp[arg_count++] = str;
    argp[arg_count] = NULL;
    return arg_count;
}

static void cmd_help(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    show_help_text();
}

static void cmd_clear(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    putc('\f');
}

void background_color(int argc, char **argv);
void show_or_set_charset(int argc, char **argv);
void foreground_color(int argc, char **argv);
void border_color(int argc, char **argv);
void dma_copy_bytes(int argc, char **argv);
void show_dump(int argc, char **argv);
void set_graphics_dma(int argc, char **argv);
void mailbox_options(int argc, char **argv);
void show_memory_usage(int argc, char **argv);
void display_mode(int argc, char **argv);
void set_output(int argc, char **argv);
void show_palette(int argc, char **argv);
void peek_memory_8bit(int argc, char **argv);
void peek_memory_32bit(int argc, char **argv);
void set_pixels(int argc, char **argv);
void poke_memory_8bit(int argc, char **argv);
void poke_memory_32bit(int argc, char **argv);
void run_tests(int argc, char **argv);
void show_timers(int argc, char **argv);

static void prompt()
{
    putc('*');
}

typedef struct {
    char *name;
    void (*function)(int, char **);
} command_t;

command_t commands[] = {
        { .name = "help",    .function = &cmd_help },
        { .name = "bgcolor", .function = &background_color },
        { .name = "border",  .function = &border_color },
        { .name = "charset", .function = &show_or_set_charset },
        { .name = "clear",   .function = &cmd_clear },
        { .name = "color",   .function = &foreground_color },
        { .name = "dmacopy", .function = &dma_copy_bytes },
        { .name = "dump",    .function = &show_dump },
        { .name = "gfxdma",  .function = &set_graphics_dma },
        { .name = "halt",    .function = NULL },
        { .name = "mailbox", .function = &mailbox_options },
        { .name = "memory",  .function = &show_memory_usage },
        { .name = "mode",    .function = &display_mode },
        { .name = "out",     .function = &set_output },
        { .name = "palette", .function = &show_palette },
        { .name = "peek",    .function = &peek_memory_8bit },
        { .name = "peek32",  .function = &peek_memory_32bit },
        { .name = "pixels",  .function = &set_pixels },
        { .name = "poke",    .function = &poke_memory_8bit },
        { .name = "poke32",  .function = &poke_memory_32bit },
        { .name = "reset",   .function = NULL },
        { .name = "test",    .function = &run_tests },
        { .name = "timers",  .function = &show_timers },
        { .name = NULL,      .function = &bad_command }, /* this one goes last */
};

int shell()
{
    DEBUG_START("shell");
    command_buffer[0] = '\0';
    while (1)
    {
        prompt();
        int i = getline(command_buffer, COMMAND_BUFFER_SIZE);
        int arg_count = split(command_buffer, ' ', MAX_ARGS, args);
        if (i < 0)
        {
            escape();
            continue;
        }
        if (!strlen(command_buffer)) continue;
        for (i = 0; commands[i].name != NULL && strcmp(commands[i].name, command_buffer); i++);
        if (commands[i].function == NULL) break;
        debug_printf("Command: %s arg count: %d\r\n", command_buffer, arg_count);
        (*(commands[i].function))(arg_count, args);
    }
    DEBUG_END();
    return strcmp(command_buffer, "reset");
}