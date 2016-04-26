#include "serialreceiver.h"

SerialReceiver::SerialReceiver(QObject *parent) : QObject(parent)
{

}

QList<QSerialPortInfo> SerialReceiver::getSerialPorts()
{
    return QSerialPortInfo::availablePorts();
}

QList<QString> SerialReceiver::getSerialPortNames()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QString> portNames;
    foreach (const QSerialPortInfo &portInfo, ports)
    {
        portNames.append(portInfo.portName());
    }
    return portNames;
}

void SerialReceiver::setPortName(const QString &portName)
{
    serialPort.setPortName(portName);
}

