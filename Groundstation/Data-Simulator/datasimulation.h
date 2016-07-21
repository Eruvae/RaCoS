#ifndef DATASIMULATION_H
#define DATASIMULATION_H

#include <QObject>
#include <QTimer>
#include <QTime>

class DataSimulation : public QObject
{
    Q_OBJECT
public:
    explicit DataSimulation(QObject *parent = 0);

private:
    int16_t imu1_roll;
    int16_t imu1_yaw;
    int16_t imu1_pitch;
    int16_t imu1_accx;
    int16_t imu1_accy;
    int16_t imu1_accz;
    int16_t imu2_roll;
    int16_t imu2_yaw;
    int16_t imu2_pitch;
    int16_t imu2_accx;
    int16_t imu2_accy;
    int16_t imu2_accz;

    uint16_t presTank;
    uint16_t presValves;
    int16_t tempTank;
    int16_t tempNoz1;
    int16_t tempNoz2;
    int16_t tempNoz3;
    int16_t tempNoz4;
    int16_t tempPDU;

    uint8_t mode;
    uint16_t vot1;
    uint16_t vot2;
    uint8_t valveState;

    QTimer *timer;
    QTime time;


signals:

public slots:
    void update();
};

#endif // DATASIMULATION_H
