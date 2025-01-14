#ifndef LDOU_H
#define LDOU_H

#include <stdint.h>

#define LDOU_FLAG1 0x7E7E
#define LDOU_FLAG2 0xE7E7
#define LDOU_BROADCAST_ADDR 0xFF
#define LDOU_MAX_DATA_SIZE 64

/**
 * @brief LDOU packet structure
 */
typedef struct {
    uint16_t flag;                    /// < Packet flag
    uint8_t len;                      /// < Packet length (including address, command, data, and checksum)
    uint8_t addr;                     /// < Target address
    uint8_t cmd;                      /// < Command
    uint8_t data[LDOU_MAX_DATA_SIZE]; /// < Data section
    uint8_t sum;                      /// < Checksum
} ldou_packet_t;

typedef void (*ldou_handler_t)(const ldou_packet_t *request, ldou_packet_t *response);

extern ldou_handler_t command_handler;

extern uint8_t ldou_calculate_checksum(const ldou_packet_t *packet);

// Initialization interface
extern int ldou_init(const char *bind_ip, uint16_t port);

// Data receiving interface
extern int ldou_receive(ldou_packet_t *packet);

// Data sending interface
extern int ldou_send(const ldou_packet_t *packet);

// Register command handler callback interface
void ldou_register_handler(ldou_handler_t handler);

#endif // LDOU_H