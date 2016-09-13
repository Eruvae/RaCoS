/********************************************************************************
** Form generated from reading UI file 'commandwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDWINDOW_H
#define UI_COMMANDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_commandWindow
{
public:
    QTextEdit *textEdit;

    void setupUi(QDialog *commandWindow)
    {
        if (commandWindow->objectName().isEmpty())
            commandWindow->setObjectName(QStringLiteral("commandWindow"));
        commandWindow->resize(400, 300);
        textEdit = new QTextEdit(commandWindow);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(110, 110, 201, 51));

        retranslateUi(commandWindow);

        QMetaObject::connectSlotsByName(commandWindow);
    } // setupUi

    void retranslateUi(QDialog *commandWindow)
    {
        commandWindow->setWindowTitle(QApplication::translate("commandWindow", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class commandWindow: public Ui_commandWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDWINDOW_H
