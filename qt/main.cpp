#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application metadata
    QApplication::setApplicationName("Stock Market Trader");
    QApplication::setOrganizationName("StockSim");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
