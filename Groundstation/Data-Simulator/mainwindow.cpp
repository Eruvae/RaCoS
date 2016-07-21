#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));

    menuSelectCOM = ui->menuBar->addMenu("Serial COM");
    menuConnect = menuSelectCOM->addMenu("Connect");

    menuDisconnect = menuSelectCOM->addAction("Disconnect");
    connect(menuDisconnect, SIGNAL(triggered()), this, SLOT(serialDisconnect()));

    QList<QString> serialPortNames = getSerialPortNames();
    foreach (const QString &name, serialPortNames)
    {
        menuConnectCOMS.append(menuConnect->addAction(name));
        connect(menuConnectCOMS.last(), SIGNAL(triggered()), this, SLOT(serialConnect()));
        menuConnectCOMS.last()->setCheckable(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<QString> MainWindow::getSerialPortNames()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QString> portNames;
    foreach (const QSerialPortInfo &portInfo, ports)
    {
        portNames.append(portInfo.portName());
    }
    return portNames;
}


void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    // TODO
}

void MainWindow::on_pushButton_clicked()
{

}
