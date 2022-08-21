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
    puts("\tclear - clear the screen\r\n");
    puts("\tcharset - show character set\r\n");
    puts("\tcolor <#> - set foreground color\r\n");
    puts("\tdump (stack|<start_addr>) (<end_addr>|+<size>) - memory dump\r\n");
    puts("\tgfx (fb|prop) - initialise the graphical display using framebuffer or property channel\r\n");
    puts("\thalt - halt the shell\r\n");
    puts("\thelp - this text\r\n");
    puts("\tmailbox (list|<method_name|method_id>) [...] - mailbox methods\r\n");
    puts("\tmemory - show memory usage\r\n");
    puts("\tmode <width> <height> <depth> - set display mode\r\n");
    puts("\tout (fb|uart) - select output\r\n");
    puts("\tpalette (<palette-mode> <rgb-mode>) - show/set colour palette (modes are integers 0-4)\r\n");
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

/*
 * Shell commands follow here - switch off unused parameter warnings for these
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void background_color(int argc, char **argv);
static void cmd_bgcolor(int argc, char **argv)
{
    background_color(argc, argv);
}

void show_charset();
static void cmd_charset(int argc, char **argv)
{
    show_charset();
}

static void cmd_clear(int argc, char **argv)
{
    putc(12);
}

void foreground_color(int argc, char **argv);
static void cmd_color(int argc, char **argv)
{
    foreground_color(argc, argv);
}

void init_graphics(int argc, char **argv);
static void cmd_gfx(int argc, char **argv)
{
    init_graphics(argc, argv);
}

void show_dump(int argc, char **argv);
static void cmd_dump(int argc, char **argv)
{
    show_dump(argc, argv);
}

static void cmd_help(int argc, char **argv)
{
    show_help_text();
}

void mailbox_options(int argc, char **argv);
static void cmd_mailbox(int argc, char **argv)
{
    mailbox_options(argc, argv);
}

void show_memory_usage();
static void cmd_memory(int argc, char **argv)
{
    show_memory_usage();
}

void display_mode(int argc, char **argv);
static void cmd_mode(int argc, char **argv)
{
    display_mode(argc, argv);
}

void set_output(int argc, char **argv);
static void cmd_out(int argc, char **argv)
{
    set_output(argc, argv);
}

void show_palette(int argc, char **argv);
static void cmd_palette(int argc, char **argv)
{
    putc(12);
    show_palette(argc, argv);
}

void run_tests();
static void cmd_test(int argc, char **argv)
{
    run_tests();
}

void show_timers();
static void cmd_timers(int argc, char **argv)
{
    show_timers();
}

/*
 * Shell commands end here - switch unused parameter warnings back on
 */
#pragma GCC diagnostic pop

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
        { .name = "bgcolor", .function = &cmd_bgcolor },
        { .name = "charset", .function = &cmd_charset },
        { .name = "clear",   .function = &cmd_clear },
        { .name = "color",   .function = &cmd_color },
        { .name = "dump",    .function = &cmd_dump },
        { .name = "gfx",     .function = &cmd_gfx },
        { .name = "halt",    .function = NULL },
        { .name = "mailbox", .function = &cmd_mailbox },
        { .name = "memory",  .function = &cmd_memory },
        { .name = "mode",    .function = &cmd_mode },
        { .name = "out",     .function = &cmd_out },
        { .name = "palette", .function = &cmd_palette },
        { .name = "reset",   .function = NULL },
        { .name = "test",    .function = &cmd_test },
        { .name = "timers",  .function = &cmd_timers },
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