#ifndef SERIALRECEIVER_H
#define SERIALRECEIVER_H

#include <QObject>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

class SerialReceiver : public QObject
{
    Q_OBJECT
public:
    explicit SerialReceiver(QObject *parent = 0);
    static QList<QSerialPortInfo> getSerialPorts();
    static QList<QString> getSerialPortNames();
    void setPort(const QSerialPortInfo &port);
    void setPortName(const QString &portName);


signals:

public slots:

private:
    QSerialPort serialPort;
};

#endif // SERIALRECEIVER_H
