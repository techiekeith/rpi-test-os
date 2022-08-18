/*
 * usb_info.c
 */

#include "../../include/common/stddef.h"
#include "../../include/common/stdio.h"
#include "../../include/kernel/usb/usb.h"

void usb_info(int argc, char **argv)
{
    usb_device_t *root = usb_get_root_hub();
    if (root == NULL)
    {
        printf("\r\nNo USB devices\r\n");
        return;
    }

    printf("\r\nUSB devices:\r\n");


}