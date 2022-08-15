/*
 * debug.c
 */

#include "../../include/common/stdint.h"
#include "../../include/common/stdio.h"
#include "../../include/common/stdlib.h"
#include "../../include/common/string.h"
#include "../../include/kernel/framebuffer.h"
#include "../../include/kernel/mailbox.h"

extern const char *divider;

static void mailbox_syntax()
{
    puts("\r\nSyntax: mailbox (list|<method_name|method_id>) [...]\r\n");
}

void mailbox_options(int argc, char **argv)
{
    if (argc < 2)
    {
        mailbox_syntax();
        return;
    }
    if (!strcmp(argv[1], "list"))
    {
        int filter = -1;
        if (argc == 2)
        {
            puts("\r\nSyntax: mailbox list <ID>\r\n\r\n");
            puts(" ID | Group\r\n");
            printf("%.4s+%.26s\r\n", divider, divider);
            printf(" %2d | %s\r\n", 0, "VideoCore");
            printf(" %2d | %s\r\n", 1, "General Hardware");
            printf(" %2d | %s\r\n", 2, "Power");
            printf(" %2d | %s\r\n", 3, "Clocks/LEDs/Temp/Voltage");
            printf(" %2d | %s\r\n", 4, "Framebuffer");
            printf(" %2d | %s\r\n", 5, "Configuration");
            printf(" %2d | %s\r\n", 6, "DMA");
            return;
        }
        filter = atoi(argv[2]);
        // list all methods
        printf(" %2s | %33s | %|8s | %4s | %4s\r\n", "ID", "Method Name", "Tag Id", "RqLn", "RsLn");
        printf("%.4s+%.35s+%.10s+%.6s+%.6s\r\n", divider, divider, divider, divider, divider);
        for (int i = 0; mailbox_methods[i].tag != NULL_TAG; i++)
        {
            if ((filter < 0) || filter == (int)(mailbox_methods[i].tag >> 16))
            {
                printf(" %2d | %33s | %08x | %4d | %4d\r\n",
                       i + 1,
                       mailbox_methods[i].method_name,
                       mailbox_methods[i].tag,
                       mailbox_methods[i].request_size,
                       mailbox_methods[i].response_size);
            }
        }
        return;
    }
    int method_id = atoi(argv[1]);
    mailbox_method_t *method;
    if (method_id)
    {
        method = &mailbox_methods[method_id - 1];
    }
    else
    {
        method = get_mailbox_method_by_name(argv[1]);
        if (!method) {
            printf("\r\nUnknown mailbox method '%s'. Enter 'mailbox list' to list available methods.\r\n", argv[1]);
            return;
        }
    }
    int args_supplied = argc - 2;
    int args_required = method->request_size / 4;
    if (args_supplied != args_required)
    {
        printf("\r\nWrong number of arguments for mailbox method '%s'.\r\n"
               "Method needs %d argument%s, but %d %s supplied.\r\n",
               argv[1], args_required, args_required == 1 ? "" : "s",
               args_supplied, args_supplied == 1 ? "was" : "were");
        return;
    }
    puts(method->method_name);
    puts(" = ");
    property_message_tag_t tags[2];
    tags[0].proptag = method->tag;
    for (int i = 0; i < args_required; i++)
    {
        tags[0].value_buffer.data[i] = strtol(argv[i + 2], NULL, 0);
    }
    tags[1].proptag = NULL_TAG;
    int rv = send_messages(tags);
    if (rv)
    {
        printf("Error code %d\r\n", rv);
        return;
    }
    for (int i = 0; i < (int)(MIN((method->response_size + 3) / 4, 16, uint32_t)); i++)
    {
        if (fbinfo.current_column + 9 >= fbinfo.columns)
        {
            puts("\r\n");
            puts(method->method_name);
            puts(" = ");
        }
        printf("%08ux ", tags[0].value_buffer.data[i]);
    }
    if (method->response_size > 64)
    {
        puts("...");
    }
    puts("\r\n");
}
