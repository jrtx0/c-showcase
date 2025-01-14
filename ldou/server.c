#include "ldou.h"

void example_command_handler(const ldou_packet_t *request, ldou_packet_t *response)
{
    printf("Received command: 0x%02X\n", request->cmd);
    printf("Data length: %d\n", request->len - 3);

    for (int i = 0; i < request-> len - 3; i++)
    {
        printf("Data[%d]: 0x%02X\n", i, request->data[i]);
    }

    // 填充应答包
    response->flag = LDOU_FLAG2;
    response->len = request->addr;
    response->cmd = request->cmd;
    response->len = 4;
    response->data[0] = 0x00;
    response->sum = ldou_calculate_checksum(response);
}

int main()
{
    const char *bind_ip = "0.0.0.0";
    uint16_t port = 12345;

    if (ldou_init(bind_ip, port) != 0)
    {
        printf("Failed to initialize LDOU protocol.\n");
        return -1;
    }

    printf("LDOU protocol initialized. Waiting for commands on %s:%d...\n", bind_ip, port);

    // 注册命令处理函数
    ldou_register_handler(example_command_handler);

    while (1)
    {
        ldou_packet_t request;
        ldou_packet_t response;

        int result = ldou_receive(&request);

        if (result == 0)
        {
            printf("Packet received successfully.\n");

            // 调用命令处理函数
            command_handler(&request, &response);
            
            if (ldou_send(&response))
            {
                printf("Response sent successfully.\n");
            }
            else
            {
                printf("Failed to send response.\n");
            }
        }
        else
        {
            printf("Error receiving packet: %d\n", result);
        }
    }

    ldou_cleanup();

    return 0;
}