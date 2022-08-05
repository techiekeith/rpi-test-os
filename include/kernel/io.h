/*
 * io.h
 */

#pragma once

#define OUTPUT_CHANNEL_UART     0
#define OUTPUT_CHANNEL_GPU      1

int get_input_channel();
int get_output_channel();
void set_input_channel(int channel);
void set_output_channel(int channel);
