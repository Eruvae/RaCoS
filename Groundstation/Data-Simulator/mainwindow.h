#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QMenu *menuSelectCOM;
    QMenu *menuConnect;
    QList<QAction*> menuConnectCOMS;
    QAction *menuDisconnect;
    QList<QString> getSerialPortNames();
    void closeSerialPort();

public slots:
    void writeData(const QByteArray &data);
    void handleError(QSerialPort::SerialPortError error);
private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
