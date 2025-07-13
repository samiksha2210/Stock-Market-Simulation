#ifndef TRADER_QT_H
#define TRADER_QT_H

#ifdef __cplusplus
#include <string>
// C++ declaration
void sendOrderWrapper(char type, const std::string& symbol, int quantity);
extern "C" {
#endif

// C declarations (if any)

#ifdef __cplusplus
}
#endif

#endif // TRADER_QT_H
