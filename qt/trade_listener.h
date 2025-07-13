#ifndef TRADE_LISTENER_H
#define TRADE_LISTENER_H

#include <QThread>
#include <QMutex>
#include <QString>
#include <atomic>

class TradeListener : public QThread {
    Q_OBJECT

public:
    explicit TradeListener(QObject *parent = nullptr);
    ~TradeListener();
    
    void stop();

signals:
    void tradeExecuted(const QString &message);

protected:
    void run() override;

private:
    std::atomic<bool> m_running;
    QMutex m_mutex;
    int m_socketFd;
};

#endif // TRADE_LISTENER_H
