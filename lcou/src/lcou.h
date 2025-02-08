#ifndef LCOU_H
#define LCOU_H

#include <stdint.h>

#define LCOU_FLAG1 0x7E7E
#define LCOU_FLAG2 0xE7E7
#define LCOU_BROADCAST_ADDR 0xFF
#define LCOU_MAX_DATA_SIZE 64

/**
 * @brief LCOU packet structure
 */
typedef struct {
    uint16_t flag;                    /// < Packet flag
    uint8_t len;                      /// < Packet length (including address, command, data, and checksum)
    uint8_t addr;                     /// < Target address
    uint8_t cmd;                      /// < Command
    uint8_t data[LCOU_MAX_DATA_SIZE]; /// < Data section
    uint8_t sum;                      /// < Checksum
} lcou_packet_t;

typedef void (*lcou_handler_t)(const lcou_packet_t *request, lcou_packet_t *response);

extern lcou_handler_t command_handler;

extern uint8_t lcou_calculate_checksum(const lcou_packet_t *packet);

// Initialization interface
extern int lcou_init(const char *bind_ip, uint16_t port);

// Data receiving interface
extern int lcou_receive(lcou_packet_t *packet);

// Data sending interface
extern int lcou_send(const lcou_packet_t *packet);

// Register command handler callback interface
void lcou_register_handler(lcou_handler_t handler);

#endif // LCOU_H