#ifndef PACKAGES_H
#define PACKAGES_H

#include <stdint.h>

#define SYNC_IMU    (char)0xA8 // 0b10101000
#define SYNC_PT     (char)0xA4 // 0b10100100
#define SYNC_CALC   (char)0xA2 //0b10100010

#pragma pack(push,1)
struct dpIMU
{
    uint8_t sync;
    uint8_t time[3];
    uint16_t counter;

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

    uint16_t check;
};

struct dpPresTemp
{
    uint8_t sync;
    uint8_t time[3];
    uint16_t counter;

    uint16_t presTank;
    uint16_t presValves;
    int16_t tempTank;
    int16_t tempNoz1;
    int16_t tempNoz2;
    int16_t tempNoz3;
    int16_t tempNoz4;
    int16_t tempPDU;

    uint16_t check;
};

struct dpCalc
{
    uint8_t sync;
    uint8_t time[3];
    uint16_t counter;

    uint8_t mode;
    uint16_t vot1;
    uint16_t vot2;
    uint8_t valveState;

    uint16_t check;
};
#pragma pack(pop)

#endif // PACKAGES_H
