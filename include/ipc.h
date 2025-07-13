#ifndef IPC_H
#define IPC_H

#include <mqueue.h>

// Constants for message queue
#define MQ_NAME "/market_queue"
#define MQ_MSG_SIZE 128

// Socket paths
#define MARKET_SOCKET_PATH "/tmp/market_update_socket"
#define TRADE_SOCKET_PATH "/tmp/trade_notification_socket"

// Function prototypes
mqd_t create_message_queue();
int send_market_update(int socket_fd, const char *message);
int setup_market_update_socket();
int setup_trade_notification_socket();
int send_trade_notification(int socket_fd, const char *message);

#endif // IPC_H
