#include "trader_qt.h"
#include "trader.h"
#include <string>

// C++ wrapper for send_order
void sendOrderWrapper(char type, const std::string& symbol, int quantity) {
    send_order(type, symbol.c_str(), quantity);
}
