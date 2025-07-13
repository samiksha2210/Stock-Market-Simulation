#ifndef TRADER_H
#define TRADER_H

#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
void send_order(char type, const char *symbol, int quantity);
void *market_listener(void *arg);

// Function to start the trader
void start_trader();

#ifdef __cplusplus
}
#endif

#endif // TRADER_H
