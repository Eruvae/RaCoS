#include "datasimulation.h"

DataSimulation::DataSimulation(QObject *parent) : QObject(parent)
{
    imu1_roll = 2361;
    imu1_yaw = 234;
    imu1_pitch = 254;
    imu1_accx = 30;
    imu1_accy = 23;
    imu1_accz = 156;
    imu2_roll = 2343;
    imu2_yaw = 245;
    imu2_pitch = 250;
    imu2_accx = 31;
    imu2_accy = 20;
    imu2_accz = 149;

    presTank;
    presValves;
    tempTank;
    tempNoz1;
    tempNoz2;
    tempNoz3;
    tempNoz4;
    tempPDU;

    mode;
    vot1;
    vot2;
    valveState;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(500);

    time.start();
}


void DataSimulation::update()
{
    int elapsed = time.elapsed();
    imu1_roll += 5 * (rand() % 3 - 1);

}
