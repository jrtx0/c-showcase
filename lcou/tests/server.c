#include "lcou.h"

void example_command_handler(const lcou_packet_t *request, lcou_packet_t *response)
{
    printf("received command: 0x%02X\n", request->cmd);
    printf("data length: %d\n", request->len - 3);

    for (int i = 0; i < request-> len - 3; i++)
    {
        printf("Data[%d]: 0x%02X\n", i, request->data[i]);
    }

    // Fill the response packet
    response->flag = LDOU_FLAG2;
    response->len = request->addr;
    response->cmd = request->cmd;
    response->len = 4;
    response->data[0] = 0x00;
    response->sum = lcou_calculate_checksum(response);
}

int main()
{
    const char *bind_ip = "0.0.0.0";
    uint16_t port = 12345;

    if (lcou_init(bind_ip, port) != 0)
    {
        printf("failed to initialize LDOU protocol.\n");
        return -1;
    }

    printf("LDOU protocol initialized. Waiting for commands on %s:%d...\n", bind_ip, port);

    // Register command handler function
    lcou_register_handler(example_command_handler);

    while (1)
    {
        lcou_packet_t request;
        lcou_packet_t response;

        int result = lcou_receive(&request);

        if (result == 0)
        {
            printf("packet received successfully.\n");

            // Call the command handler function
            command_handler(&request, &response);
            
            if (lcou_send(&response) == 0)
            {
                printf("response sent successfully.\n");
            }
            else
            {
                printf("failed to send response.\n");
            }
        }
        else
        {
            printf("error receiving packet: %d\n", result);
        }
    }

    return 0;
}