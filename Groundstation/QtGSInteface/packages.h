#ifndef PACKAGES_H
#define PACKAGES_H

#include <stdint.h>

#define SYNC_IMU    (uint8_t)0xA8 // 0b10101000
#define SYNC_PT     (uint8_t)0xA4 // 0b10100100
#define SYNC_CALC   (uint8_t)0xA2 // 0b10100010
#define SYNC_COMM   (uint8_t)0xA1 // 0b10100001

#define DELETE_SD         0x00
#define SHUTDOWN        0x01
#define RESTART         0x02
#define MODE_STANDBY    0x03
#define MODE_FLIGHT     0x04
#define MODE_SECURE     0x05
#define MODE_BD         0x06
#define TEST_ACTIVATE   0x07
#define TEST_LEAVE      0x08

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

    uint32_t check;
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

    uint32_t check;
};

struct dpCalc
{
    uint8_t sync;
    uint8_t time[3];
    uint16_t counter;

    uint8_t mode;
    uint16_t modulStates;
    uint16_t vot1;
    uint16_t vot2;
    uint8_t valveState;

    uint32_t check;
};

struct dpCommand
{
    uint8_t sync;
    uint8_t id;
    uint16_t counter;
    uint32_t check;
};

#pragma pack(pop)

#endif // PACKAGES_H
