#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

mqd_t create_message_queue() {
    struct mq_attr attr = {0, 10, MQ_MSG_SIZE, 0, {0}};
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed");
        exit(1);
    }
    return mq;
}

int send_market_update(int socket_fd, const char *message) {
    if (socket_fd < 0) {
        fprintf(stderr, "Invalid socket for market update\n");
        return -1;
    }
    
    if (send(socket_fd, message, strlen(message), 0) == -1) {
        perror("send market update failed");
        return -1;
    }
    return 0;
}

int send_trade_notification(int socket_fd, const char *message) {
    if (socket_fd < 0) {
        fprintf(stderr, "Invalid socket for trade notification\n");
        return -1;
    }
    
    if (send(socket_fd, message, strlen(message), 0) == -1) {
        perror("send trade notification failed");
        return -1;
    }
    return 0;
}

int setup_market_update_socket() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket failed for market updates");
        exit(1);
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, MARKET_SOCKET_PATH);
    
    unlink(MARKET_SOCKET_PATH);
    
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind failed for market updates");
        close(server_fd);
        exit(1);
    }
    
    if (listen(server_fd, 5) == -1) {
        perror("listen failed for market updates");
        close(server_fd);
        exit(1);
    }
    
    printf("Market Update Server Listening on %s\n", MARKET_SOCKET_PATH);
    
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept failed for market updates");
        close(server_fd);
        return -1;
    }
    
    return client_fd;
}

int setup_trade_notification_socket() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket failed for trade notifications");
        exit(1);
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, TRADE_SOCKET_PATH);
    
    unlink(TRADE_SOCKET_PATH);
    
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind failed for trade notifications");
        close(server_fd);
        exit(1);
    }
    
    if (listen(server_fd, 5) == -1) {
        perror("listen failed for trade notifications");
        close(server_fd);
        exit(1);
    }
    
    printf("Trade Notification Server Listening on %s\n", TRADE_SOCKET_PATH);
    
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept failed for trade notifications");
        close(server_fd);
        return -1;
    }
    
    return client_fd;
}
