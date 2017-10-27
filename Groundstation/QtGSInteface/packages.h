#ifndef PACKAGES_H
#define PACKAGES_H

#include <stdint.h>
#include <QString>

#define SYNC_IMU    (uint8_t)0xA8 // 0b10101000
#define SYNC_PT     (uint8_t)0xA4 // 0b10100100
#define SYNC_CALC   (uint8_t)0xA2 // 0b10100010
#define SYNC_COMM   (uint8_t)0xA1 // 0b10100001

#define SYNC_DL_NF	(uint8_t)0xB1
#define SYNC_DL_FD	(uint8_t)0xB2
#define SYNC_DL_FC	(uint8_t)0xB4
#define SYNC_DL_SC	(uint8_t)0xB8

#define FORMAT_SD		0x00
#define MODE_STANDBY    0x04
#define MODE_FLIGHT     0x05
#define MODE_SECURE     0x06
#define MODE_BD         0x07
#define TEST_ACTIVATE   0x08
#define TEST_LEAVE      0x09
#define DOWNLINK_SD     0x10
#define REQUEST_PCK		0x11
#define RESET_SD        0x12
#define CALIBRATE_IMU   0x13
#define MODE_SPINUP		0x14
#define RESET_CONTROL	0x15
#define DL_DEBUG_ID		0x16

#define OK              0
#define NO_INIT         1
#define INIT_FAILED     2
#define COMM_ERROR      3
#define MINOR_ERROR     4
#define CRITICAL_ERROR  5
#define SD_DL_PROGRESS  6

#pragma pack(push,1)

struct dpIMU
{
    uint8_t sync;
    uint8_t time[3];
    uint16_t counter;

    int16_t imu1_pitch;
    int16_t imu1_yaw;
    int16_t imu1_roll;
    int16_t imu1_accx;
    int16_t imu1_accy;
    int16_t imu1_accz;
    int16_t imu2_pitch;
    int16_t imu2_yaw;
    int16_t imu2_roll;
    int16_t imu2_accx;
    int16_t imu2_accy;
    int16_t imu2_accz;
    int16_t imufiltered_pitch;
    int16_t imufiltered_yaw;
    int16_t imufiltered_roll;

    uint8_t imu_config;

    uint32_t check;
};

struct dpPresTemp
{
    uint8_t sync;
    uint8_t time[3];
    uint16_t counter;

    int16_t presTank;
    int16_t presValves;
    int16_t tempTank;
    int16_t tempPDU;
    int16_t tempValveRP;
    int16_t tempValveRM;
    int16_t tempValveEC;
    int16_t tempNoz1;
    int16_t tempNoz2;


    uint32_t check;
};

struct dpCalc
{
    uint8_t sync;
    uint8_t time[3];
    uint16_t counter;

    uint8_t mode;
    uint16_t modulStates;
    uint32_t vot;
    uint8_t valveState;
    uint8_t rocket_signals;
    uint16_t deltaOmegaStart;
    uint16_t deltaOmegaOperation;
    uint16_t deltaOmegaShutdown;

    uint32_t check;
};

struct dpCommand
{
    uint8_t sync;
    uint8_t id;
    uint32_t argument;
    uint32_t check;
};

struct DpFinfo
{
    uint8_t sync;
    uint32_t counter;
    char fname[13];
    uint32_t check;
};

struct DpFdata
{
    uint8_t sync;
    uint32_t counter;
    uint8_t fdata[55];
    uint32_t check;
};

struct DpFend
{
    uint8_t sync;
    uint32_t counter;
    uint8_t fdata[54];
    uint8_t remaining_bytes;
    uint32_t check;
};

struct DpDLcomplete
{
     uint8_t sync;
     uint32_t counter;
     uint32_t check;
};

/*
struct ObcIMUdata
{
    uint8_t sync;
    int16_t imu1_pitch;
    int16_t imu1_yaw;
    int16_t imu1_roll;
    int16_t imu1_accx;
    int16_t imu1_accy;
    int16_t imu1_accz;
    int16_t imu2_pitch;
    int16_t imu2_yaw;
    int16_t imu2_roll;
    int16_t imu2_accx;
    int16_t imu2_accy;
    int16_t imu2_accz;
    double gyroFiltered[3];
    int8_t imu_config;
    uint64_t sysTime;
};

struct ObcHkData
{
    uint8_t sync;
    int16_t presTank;
    int16_t presValves;
    int16_t tempTank;
    int16_t tempPDU;
    int16_t tempValveRP;
    int16_t tempValveRM;
    int16_t tempValveEC;
    int16_t tempNoz1;
    int16_t tempNoz2;
    uint64_t sysTime;
};

struct ObcCmdData
{
    uint8_t sync;
    uint16_t vot;
    uint8_t valveState;
};
*/

struct DpDebug
{
    uint8_t sync;
    char text[59];
    uint32_t check;
};

#pragma pack(pop)

struct State
{
    double presTank = 0;
    double presValves = 0;

    double tempTank = 0;
    double tempPDU = 0;
    double tempValveRP = 0;
    double tempValveRM = 0;
    double tempValveEC = 0;
    double tempNoz1 = 0;
    double tempNoz2 = 0;


    double imu1_pitch = 0;
    double imu1_yaw = 0;
    double imu1_roll = 0;
    double imu1_accx = 0;
    double imu1_accy = 0;
    double imu1_accz = 0;
    double imu2_pitch = 0;
    double imu2_yaw = 0;
    double imu2_roll = 0;
    double imu2_accx = 0;
    double imu2_accy = 0;
    double imu2_accz = 0;
    double imufiltered_pitch = 0;
    double imufiltered_yaw = 0;
    double imufiltered_roll = 0;

    double time = 0;

    QString mode = "";
    QString imuState = "";
    QString hkState = "";
    QString controlState = "";
    QString storageState = "";
    QString tmState = "";
    uint32_t vot = 0;
    double deltaOmegaStart = 0;
    double deltaOmegaOperation = 0;
    double deltaOmegaShutdown = 0;
    Qt::GlobalColor valveRM = Qt::red;
    Qt::GlobalColor valveRP = Qt::red;
    Qt::GlobalColor valveEC = Qt::red;
    Qt::GlobalColor testMode = Qt::red;
    Qt::GlobalColor LO = Qt::red;
    Qt::GlobalColor SOE = Qt::red;
    Qt::GlobalColor SODS = Qt::red;
    Qt::GlobalColor DCDC = Qt::red;

    size_t start_point = 0;
    size_t used_plotpoints = 0;
    double plot_min = -50;
    double plot_max = 50;

};

#endif // PACKAGES_H
