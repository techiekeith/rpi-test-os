/*
 * io.c
 */

#include <kernel/io.h>

static int input_channel, output_channel;

int get_input_channel()
{
    return input_channel;
}

int get_output_channel()
{
    return output_channel;
}

void set_input_channel(int channel)
{
    input_channel = channel;
}

void set_output_channel(int channel)
{
    output_channel = channel;
}
