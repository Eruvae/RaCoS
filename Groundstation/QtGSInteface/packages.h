#ifndef PACKAGES_H
#define PACKAGES_H

#include <stdint.h>

struct __attribute__((packed, aligned(1))) dpIMU
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

struct __attribute__((packed, aligned(1))) dpPresTemp
{
    uint8_t sync;
    uint32_t time;
    uint16_t presTank;
    uint16_t presPV;
    uint16_t presRp;
    uint16_t presRm;
    uint16_t tempTank;
    uint16_t tempNoz1;
    uint16_t tempNoz2;
    uint16_t tempNoz3;
    uint16_t tempNoz4;
    uint16_t tempPDU;
    uint32_t check;
};

struct __attribute__((packed, aligned(1))) dpCalc
{
    uint8_t sync;
    uint32_t time;
    uint8_t mode;
    uint16_t vot1;
    uint16_t vot2;
    uint8_t valveState;
    uint32_t check;
};

#endif // PACKAGES_H
