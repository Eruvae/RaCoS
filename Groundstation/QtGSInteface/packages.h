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
    uint32_t time;
    uint16_t imu1_roll;
    uint16_t imu1_yaw;
    uint16_t imu1_pitch;
    uint16_t imu1_accx;
    uint16_t imu1_accy;
    uint16_t imu1_accz;
    uint16_t imu2_roll;
    uint16_t imu2_yaw;
    uint16_t imu2_pitch;
    uint16_t imu2_accx;
    uint16_t imu2_accy;
    uint16_t imu2_accz;
    uint32_t check;
};

struct dpPresTemp
{
    uint8_t sync;
    uint32_t time;
    uint16_t presTank;
    uint16_t presValves;
    uint16_t tempTank;
    uint16_t tempNoz1;
    uint16_t tempNoz2;
    uint16_t tempNoz3;
    uint16_t tempNoz4;
    uint16_t tempPDU;
    uint32_t check;
};

struct dpCalc
{
    uint8_t sync;
    uint32_t time;
    uint8_t mode;
    uint16_t vot1;
    uint16_t vot2;
    uint8_t valveState;
    uint32_t check;
};
#pragma pack(pop)

#endif // PACKAGES_H
