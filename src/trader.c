#include "trader.h"
#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <mqueue.h>

// Send an Order to the Exchange
void send_order(char type, const char *symbol, int quantity) {
    mqd_t mq = mq_open(MQ_NAME, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed");
        return;
    }
    char buffer[MQ_MSG_SIZE];
    snprintf(buffer, sizeof(buffer), "%c %s %d", type, symbol, quantity);
    if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("mq_send failed");
    } else {
        printf("Order Sent: %c %s %d\n", type, symbol, quantity);
    }
    mq_close(mq);
}

// Market Listener to Receive Updates
void *market_listener(void *arg) {
    (void)arg;
    int client_fd;
    struct sockaddr_un addr;
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket failed");
        return NULL;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/market_update_socket");
    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect failed");
        close(client_fd);
        return NULL;
    }
    printf("Connected to Market Update Server...\n");
    char buffer[MQ_MSG_SIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_fd, buffer, MQ_MSG_SIZE, 0) > 0) {
            printf("Market Update: %s\n", buffer);
        }
    }
    close(client_fd);
    return NULL;
}

// Function to start the trader
void start_trader() {
    pthread_t listener_thread;
    if (pthread_create(&listener_thread, NULL, market_listener, NULL) != 0) {
        perror("Failed to create listener thread");
        exit(1);
    }
    char type;
    char symbol[10];
    int quantity;
    while (1) {
        printf("Enter order (B/S Symbol Quantity): ");
        if (scanf(" %c %s %d", &type, symbol, &quantity) != 3) {
            printf("Invalid input, try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        send_order(type, symbol, quantity);
    }
    pthread_join(listener_thread, NULL);
}
