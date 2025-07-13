#include "trade_listener.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <QDebug>
#include <QDateTime>
#include <cstring>

#define TRADE_SOCKET_PATH "/tmp/trade_notification_socket"

TradeListener::TradeListener(QObject *parent)
    : QThread(parent),
      m_running(false),
      m_socketFd(-1) {
}

TradeListener::~TradeListener() {
    stop();
    wait();
}

void TradeListener::stop() {
    m_running = false;
    if (m_socketFd != -1) {
        close(m_socketFd);
        m_socketFd = -1;
    }
}

void TradeListener::run() {
    m_running = true;
    
    while (m_running) {
        struct sockaddr_un addr;
        
        // Create socket
        m_socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (m_socketFd == -1) {
            qWarning() << "Failed to create socket:" << strerror(errno);
            // Wait before retry
            sleep(5);
            continue;
        }
        
        // Connect to socket
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "/tmp/market_update_socket");        
        if (::connect(m_socketFd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            qWarning() << "Failed to connect to socket:" << strerror(errno);
            close(m_socketFd);
            // Wait before retry
            sleep(5);
            continue;
        }
        
        qDebug() << "Connected to trade notification socket";
        
        // Receive trade notifications
        const int BUFFER_SIZE = 256;
        char buffer[BUFFER_SIZE];
        
        while (m_running) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes = recv(m_socketFd, buffer, BUFFER_SIZE - 1, 0);
            
            if (bytes > 0) {
                QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                QString message = QString("[%1] %2").arg(timestamp).arg(buffer);
                emit tradeExecuted(message);
            } else if (bytes == 0) {
                qWarning() << "Connection closed by server";
                break;
            } else {
                qWarning() << "Error receiving data:" << strerror(errno);
                break;
            }
        }
        
        close(m_socketFd);
        m_socketFd = -1;
    }
}
