# Directory structure
SRC_DIR = .
INCLUDE_DIR = ../include
OBJ_DIR = ../obj
BIN_DIR = ../bin
QT_DIR = ../qt

# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -pthread -lm -I$(INCLUDE_DIR)
CXXFLAGS = -Wall -Wextra -fPIC -I$(INCLUDE_DIR) `pkg-config --cflags Qt5Widgets`
LDFLAGS = -lcurl -ljansson `pkg-config --libs Qt5Widgets`

# Source files
EXCHANGE_SRCS = exchange.c exchange_main.c ipc.c stock_data.c
TRADER_SRCS = trader.c trader_main.c ipc.c
QT_SRCS = $(QT_DIR)/main.cpp $(QT_DIR)/mainwindow.cpp $(QT_DIR)/trade_listener.cpp trader_qt.cpp $(QT_DIR)/moc_mainwindow.cpp $(QT_DIR)/moc_trade_listener.cpp

# Object files
EXCHANGE_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(EXCHANGE_SRCS))
TRADER_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(TRADER_SRCS))
QT_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(QT_SRCS)))

# Targets
all: directories exchange trader qt_app

# Create necessary directories
directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Exchange Binary
exchange: $(EXCHANGE_OBJS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ $(LDFLAGS)

# Trader Binary
trader: $(TRADER_OBJS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ $(LDFLAGS)

# Generate UI Header
$(QT_DIR)/ui_mainwindow.h: $(QT_DIR)/mainwindow.ui
	uic $< -o $@

# Generate MOC files
$(QT_DIR)/moc_mainwindow.cpp: $(QT_DIR)/mainwindow.h
	moc $< -o $@

$(QT_DIR)/moc_trade_listener.cpp: $(QT_DIR)/trade_listener.h
	moc $< -o $@

# Qt Application
qt_app: $(QT_OBJS) $(OBJ_DIR)/trader.o $(OBJ_DIR)/ipc.o
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ $^ $(LDFLAGS)

# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(QT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

.PHONY: all directories clean exchange trader qt_app
