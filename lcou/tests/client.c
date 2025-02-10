#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "lcou.h"

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(12345);

    // Construct LDOU packet
    lcou_packet_t packet;
    packet.flag = 0x1234;
    packet.len = 5;
    packet.addr = 1;
    packet.cmd = 2;
    packet.data[0] = 3;
    packet.data[1] = 4;
    packet.sum = lcou_calculate_checksum(&packet);

    // Construct send buffer
    uint8_t buffer[128] = {0};
    buffer[0] = (uint8_t)(packet.flag >> 8);
    buffer[1] = (uint8_t)(packet.flag & 0xFF);
    buffer[2] = packet.len;
    buffer[3] = packet.addr;
    buffer[4] = packet.cmd;
    memcpy(&buffer[5], packet.data, packet.len - 3);
    buffer[2 + packet.len] = packet.sum;

    // Send packet
    ssize_t sent = sendto(sockfd, buffer, 3 + packet.len, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if (sent < 0) {
        perror("Failed to send data");
        close(sockfd);
        return -1;
    }

    printf("Packet sent successfully\n");

    // Receive response
    socklen_t len = sizeof(cliaddr);
    size_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
    if (n < 0) {
        perror("Failed to receive data");
        close(sockfd);
        return -1;
    }

    printf("Received response: ");
    
    for (ssize_t i = 0; i < n; i++)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    close(sockfd);
    return 0;
}