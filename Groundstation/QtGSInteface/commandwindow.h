#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include <QDialog>

namespace Ui {
class commandWindow;
}

class commandWindow : public QDialog
{
    Q_OBJECT

public:
    explicit commandWindow(QWidget *parent = 0);
    ~commandWindow();

private:
    Ui::commandWindow *ui;
};

#endif // COMMANDWINDOW_H
