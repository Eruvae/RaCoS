#include "commandwindow.h"
#include "ui_commandwindow.h"

commandWindow::commandWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::commandWindow)
{
    ui->setupUi(this);
}

commandWindow::~commandWindow()
{
    delete ui;
}
