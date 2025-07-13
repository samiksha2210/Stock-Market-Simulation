#include "mainwindow.h"
#include "../include/trader_qt.h"
#include "trade_listener.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Stock Market Trader");
    
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Order entry group
    QGroupBox *orderGroup = new QGroupBox("New Order", this);
    QVBoxLayout *orderLayout = new QVBoxLayout(orderGroup);
    
    symbolEntry = new QLineEdit(this);
    symbolEntry->setPlaceholderText("Symbol (e.g., AAPL)");
    orderLayout->addWidget(symbolEntry);
    
    quantityEntry = new QLineEdit(this);
    quantityEntry->setPlaceholderText("Quantity");
    orderLayout->addWidget(quantityEntry);
    
    typeCombo = new QComboBox(this);
    typeCombo->addItem("Buy (B)");
    typeCombo->addItem("Sell (S)");
    orderLayout->addWidget(typeCombo);
    
    QPushButton *sendButton = new QPushButton("Send Order", this);
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendOrderClicked);
    orderLayout->addWidget(sendButton);
    
    mainLayout->addWidget(orderGroup);
    
    // Trade history group
    QGroupBox *historyGroup = new QGroupBox("Trade History", this);
    QVBoxLayout *historyLayout = new QVBoxLayout(historyGroup);
    
    statusLabel = new QLabel("Waiting for trades...", this);
    historyLayout->addWidget(statusLabel);
    
    tradeHistoryList = new QListWidget(this);
    historyLayout->addWidget(tradeHistoryList);
    
    mainLayout->addWidget(historyGroup);
    
    setCentralWidget(centralWidget);
    resize(400, 500);
    
    // Create and start the trade listener
    tradeListener = new TradeListener(this);
    connect(tradeListener, &TradeListener::tradeExecuted, 
            this, &MainWindow::onTradeExecuted);
    tradeListener->start();
}

MainWindow::~MainWindow() {
    if (tradeListener) {
        tradeListener->stop();
        delete tradeListener;
    }
}

void MainWindow::onSendOrderClicked() {
    std::string symbol = symbolEntry->text().toStdString();
    std::string quantityText = quantityEntry->text().toStdString();
    
    // Basic validation
    if (symbol.empty()) {
        QMessageBox::warning(this, "Error", "Please enter a stock symbol");
        return;
    }
    
    if (quantityText.empty()) {
        QMessageBox::warning(this, "Error", "Please enter a quantity");
        return;
    }
    
    // Get the first character ('B' or 'S')
    char type = typeCombo->currentIndex() == 0 ? 'B' : 'S';
    int quantity = quantityEntry->text().toInt();
    
    // Call the C++ wrapper
    sendOrderWrapper(type, symbol, quantity);
    
    // Clear fields for next order
    symbolEntry->clear();
    quantityEntry->clear();
    symbolEntry->setFocus();
}

void MainWindow::onTradeExecuted(const QString &message) {
    // Update the status label
    statusLabel->setText("Last trade: " + message);
    
    // Add the message to the history list
    QListWidgetItem *item = new QListWidgetItem(message, tradeHistoryList);
    tradeHistoryList->insertItem(0, item); // Add at the top
    
    // Limit the number of items to 100 to prevent memory issues
    while (tradeHistoryList->count() > 100) {
        delete tradeHistoryList->takeItem(tradeHistoryList->count() - 1);
    }
}
