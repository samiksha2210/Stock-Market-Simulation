#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <string>

// Forward declaration of TradeListener
class TradeListener;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void onSendOrderClicked();
    void onTradeExecuted(const QString &message);
    
private:
    QLineEdit *symbolEntry;
    QLineEdit *quantityEntry;
    QComboBox *typeCombo;
    QListWidget *tradeHistoryList;
    QLabel *statusLabel;
    TradeListener *tradeListener;
};

#endif // MAINWINDOW_H
