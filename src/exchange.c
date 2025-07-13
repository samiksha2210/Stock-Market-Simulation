#include "exchange.h"
#include "ipc.h"
#include "stock_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <time.h>

volatile bool running = true;
OrderBook order_book = {NULL, NULL};
pthread_mutex_t book_mutex = PTHREAD_MUTEX_INITIALIZER;
int market_socket_fd;     // Market update socket
int trade_socket_fd = -1; // Trade notification socket

// Order Processing Thread
void *order_processing_thread(void *arg) {
    (void)arg;
    printf("Order Processing Thread Started...\n");

    mqd_t mq = create_message_queue();
    char buffer[MQ_MSG_SIZE];

    while (running) {
        memset(buffer, 0, sizeof(buffer));
        if (mq_receive(mq, buffer, MQ_MSG_SIZE, NULL) != -1) {
            char type;
            char symbol[MAX_SYMBOL_LEN];
            int qty;
            if (sscanf(buffer, "%c %s %d", &type, symbol, &qty) == 3) {
                double price = get_live_stock_price(symbol);
                if (price > 0) {
                    pthread_mutex_lock(&book_mutex);
                    add_order((type == 'B') ? &order_book.buy_orders : &order_book.sell_orders, type, symbol, qty, price);
                    match_orders();
                    pthread_mutex_unlock(&book_mutex);
                }
            }
        }
    }
    mq_close(mq);
    return NULL;
}

// Market Update Thread
void *market_update_thread(void *arg) {
    (void)arg;
    printf("Market Update Server Started...\n");

    // Ensure stale sockets are removed before binding
    unlink(MARKET_SOCKET_PATH);
    unlink(TRADE_SOCKET_PATH);

    market_socket_fd = setup_market_update_socket();
    if (market_socket_fd == -1) {
        perror("Failed to set up market update socket");
        return NULL;
    }

    // Setup the trade notification socket
    trade_socket_fd = setup_trade_notification_socket();
    if (trade_socket_fd == -1) {
        perror("Failed to set up trade notification socket");
        // Continue without trade notifications if it fails
    }

    while (running) {
        char update_msg[MQ_MSG_SIZE];
        snprintf(update_msg, sizeof(update_msg), "Market update: No recent trades.");

        // Ensure a valid connection exists before sending
        if (send_market_update(market_socket_fd, update_msg) == -1) {
            perror("Market update failed: Reinitializing socket");
            close(market_socket_fd);
            unlink(MARKET_SOCKET_PATH);
            market_socket_fd = setup_market_update_socket();
        }

        sleep(5);
    }

    close(market_socket_fd);
    if (trade_socket_fd != -1) {
        close(trade_socket_fd);
    }
    return NULL;
}

// Add Order
void add_order(Order **head, char type, char *symbol, int qty, double price) {
    Order *new_order = malloc(sizeof(Order));
    if (new_order == NULL) {
        perror("Failed to allocate memory for new order");
        return;
    }
    
    strcpy(new_order->symbol, symbol);
    new_order->quantity = qty;
    new_order->price = price;
    new_order->next = NULL;

    if (*head == NULL) {
        *head = new_order;
    } else {
        Order *temp = *head;
        while (temp->next) temp = temp->next;
        temp->next = new_order;
    }
    printf("Order Added: %c %s %d @ %.2f\n", type, symbol, qty, price);
}

// Match Orders
void match_orders() {
    Order *buy_order = order_book.buy_orders;
    Order *sell_order = order_book.sell_orders;

    while (buy_order && sell_order) {
        if (strcmp(buy_order->symbol, sell_order->symbol) == 0 &&
            buy_order->price >= sell_order->price) {
            int trade_qty = (buy_order->quantity < sell_order->quantity)
                            ? buy_order->quantity
                            : sell_order->quantity;

            char trade_msg[MQ_MSG_SIZE];
            snprintf(trade_msg, sizeof(trade_msg), "Trade Executed: %s %d @ %.2f",
                     buy_order->symbol, trade_qty, sell_order->price);
            printf("%s\n", trade_msg);

            // Send to regular market updates
            if (send_market_update(market_socket_fd, trade_msg) == -1) {
                perror("Trade update failed: Reinitializing market socket");
                close(market_socket_fd);
                unlink(MARKET_SOCKET_PATH);
                market_socket_fd = setup_market_update_socket();
            }

            // Send to trade notification socket if available
            if (trade_socket_fd != -1) {
                if (send_trade_notification(trade_socket_fd, trade_msg) == -1) {
                    perror("Trade notification failed: Reinitializing trade socket");
                    close(trade_socket_fd);
                    unlink(TRADE_SOCKET_PATH);
                    trade_socket_fd = setup_trade_notification_socket();
                }
            }

            buy_order->quantity -= trade_qty;
            sell_order->quantity -= trade_qty;

            if (buy_order->quantity == 0) {
                Order *temp = buy_order;
                buy_order = buy_order->next;
                order_book.buy_orders = buy_order;
                free(temp);
            }

            if (sell_order->quantity == 0) {
                Order *temp = sell_order;
                sell_order = sell_order->next;
                order_book.sell_orders = sell_order;
                free(temp);
            }
        } else {
            if (strcmp(buy_order->symbol, sell_order->symbol) < 0) {
                buy_order = buy_order->next;
            } else {
                sell_order = sell_order->next;
            }
        }
    }
}

// Function to start the exchange
void start_exchange() {
    pthread_t order_thread, market_thread;
    if (pthread_create(&order_thread, NULL, order_processing_thread, NULL) != 0) {
        perror("Failed to create order processing thread");
        exit(1);
    }
    if (pthread_create(&market_thread, NULL, market_update_thread, NULL) != 0) {
        perror("Failed to create market update thread");
        exit(1);
    }
    printf("Press Enter to stop the exchange...\n");
    getchar();
    running = false;
    pthread_cancel(order_thread);
    pthread_cancel(market_thread);
    pthread_join(order_thread, NULL);
    pthread_join(market_thread, NULL);
    printf("Exchange stopped.\n");
}
