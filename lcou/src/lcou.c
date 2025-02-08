#include "lcou.h"


#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

static int socket_fd = -1;
static struct sockaddr_in remote_addr;

lcou_handler_t command_handler = NULL;

/**
 * @brief Calculate checksum
 */
uint8_t lcou_calculate_checksum(const lcou_packet_t *packet)
{
    uint8_t sum = 0;

    sum += (uint8_t)(packet->flag >> 8);
    sum += (uint8_t)(packet->flag & 0xFF);
    sum += packet->len;
    sum += packet->addr;
    sum += packet->cmd;

    for (uint8_t i = 0; i < packet->len - 3; i++)
    {
        sum += packet->data[i];
    }

    return sum;
}

/**
 * @brief Initialize LCOU protocol
 */
int lcou_init(const char *bind_ip, uint16_t port)
{
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        return -1;
    }

    struct sockaddr_in local_addr = {0};
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(bind_ip);
    local_addr.sin_port = htons(port);

    if (bind(socket_fd, (const struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    {
        return -2;
    }

    return 0;
}

/**
 * @brief Receive data and parse it into an LCOU packet
 */
int lcou_receive(lcou_packet_t *packet)
{
    uint8_t buffer[128] = {0};
    socklen_t addr_len = sizeof(remote_addr);

    ssize_t received = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&remote_addr, &addr_len);

    if (received <= 0)
    {
        return -1;
    }

    packet->flag = (uint16_t)buffer[0] << 8 | buffer[1];
    packet->len = buffer[2];
    packet->addr = buffer[3];
    packet->cmd = buffer[4];

    size_t data_size = packet->len - 3;

    if (data_size > LCOU_MAX_DATA_SIZE)
    {
        return -2;
    }

    memcpy(packet->data, &buffer[5], data_size);
    packet->sum = buffer[2 + packet->len];

    uint8_t calculated_sum = lcou_calculate_checksum(packet);

    return (calculated_sum == packet->sum) ? 0 : -3;
}

/**
 * @brief Send an LCOU packet
 */
int lcou_send(const lcou_packet_t *packet)
{
    uint8_t buffer[128] = {0};

    buffer[0] = (uint8_t)(packet->flag >> 8);
    buffer[1] = (uint8_t)(packet->flag & 0xFF);
    buffer[2] = packet->len;
    buffer[3] = packet->addr;
    buffer[4] = packet->cmd;

    memcpy(&buffer[5], packet->data, packet->len - 3);

    buffer[packet->len + 2] = packet->sum;

    ssize_t sent = sendto(socket_fd, buffer, packet->len + 3, 0, (struct sockaddr *)&remote_addr, sizeof(remote_addr));

    if (send <= 0)
    {
        return -1;
    }

    return 0;
}

/**
 * @brief Register command handler callback function
 */
void lcou_register_handler(lcou_handler_t handler)
{
    command_handler = handler;
}
