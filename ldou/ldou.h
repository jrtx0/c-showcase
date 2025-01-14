#ifndef LDOU_H
#define LDOU_H

#include <stdint.h>
#include <stddef.h>

#define LDOU_FLAG1 0x7E7E
#define LDOU_FLAG2 0xE7E7
#define LDOU_BROADCAST_ADDR 0xFF
#define LDOU_MAX_DATA_SIZE 64

/**
 * @brief LDOU 数据包结构
 */
typedef struct {
    uint16_t flag;                    /// < 报文标志位
    uint8_t len;                      /// < 报文长度（含地址、命令、数据和校验和）
    uint8_t addr;                     /// < 目标地址
    uint8_t cmd;                      /// < 命令字
    uint8_t data[LDOU_MAX_DATA_SIZE]; /// 数据部分
    uint8_t sum;                      /// 校验和
} ldou_packet_t;

typedef void (*ldou_handler_t)(const ldou_packet_t *request, ldou_packet_t *response);

extern ldou_handler_t command_handler;

extern uint8_t ldou_calculate_checksum(const ldou_packet_t *packet);

// 初始化接口
extern int ldou_init(const char *bind_ip, uint16_t port);

// 接收数据接口
extern int ldou_receive(ldou_packet_t *packet);

// 发送数据接口
extern int ldou_send(const ldou_packet_t *packet);

// 注册命令处理回调接口
void ldou_register_handler(ldou_handler_t handler);

// 清理资源接口
void ldou_cleanup();



#endif // LDOU_H