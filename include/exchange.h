#ifndef EXCHANGE_H
#define EXCHANGE_H
#include <stdbool.h>

#define MAX_SYMBOL_LEN 10

// Structure for an order
typedef struct Order {
    char symbol[MAX_SYMBOL_LEN]; // Stock symbol
    int quantity;                // Quantity of the order
    double price;                // Price of the order
    struct Order *next;          // Pointer to the next order in the list
} Order;

// Structure for the order book
typedef struct {
    Order *buy_orders;  // Linked list of buy orders
    Order *sell_orders; // Linked list of sell orders
} OrderBook;

// Function prototypes
void add_order(Order **head, char type, char *symbol, int qty, double price);
void match_orders();
void *order_processing_thread(void *arg);
void *market_update_thread(void *arg);
double get_live_stock_price(const char *symbol);

// Function to start the exchange
void start_exchange();

#endif // EXCHANGE_H
