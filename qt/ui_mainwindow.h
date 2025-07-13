/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *symbolEntry;
    QLineEdit *quantityEntry;
    QComboBox *typeCombo;
    QPushButton *sendButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        symbolEntry = new QLineEdit(centralWidget);
        symbolEntry->setObjectName(QString::fromUtf8("symbolEntry"));

        verticalLayout->addWidget(symbolEntry);

        quantityEntry = new QLineEdit(centralWidget);
        quantityEntry->setObjectName(QString::fromUtf8("quantityEntry"));

        verticalLayout->addWidget(quantityEntry);

        typeCombo = new QComboBox(centralWidget);
        typeCombo->addItem(QString());
        typeCombo->addItem(QString());
        typeCombo->setObjectName(QString::fromUtf8("typeCombo"));

        verticalLayout->addWidget(typeCombo);

        sendButton = new QPushButton(centralWidget);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));

        verticalLayout->addWidget(sendButton);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Stock Market Simulator", nullptr));
        symbolEntry->setPlaceholderText(QCoreApplication::translate("MainWindow", "Symbol", nullptr));
        quantityEntry->setPlaceholderText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        typeCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Buy", nullptr));
        typeCombo->setItemText(1, QCoreApplication::translate("MainWindow", "Sell", nullptr));

        sendButton->setText(QCoreApplication::translate("MainWindow", "Send Order", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
