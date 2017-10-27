#ifndef STRUCTS_H
#define STRUCTS_H

#include "rodos.h"

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

#define TEMP_DIVIDER        16.0
#define PRES_HIGH_FACTOR	0.15513203925 // bar/bit
#define PRES_HIGH_OFFSET	24.841810875 // bar
#define PRES_LOW_FACTOR		0.012927675 // bar/bit
#define PRES_LOW_OFFSET		2.1546125 // bar

enum Mode
{
	STAND_BY = MODE_STANDBY, FLIGHT = MODE_FLIGHT, SECURE = MODE_SECURE, BLOW_DOWN = MODE_BD, TEST_SPINUP = MODE_SPINUP
};

enum Status
{
	OK = 0, NO_INIT = 1, INIT_FAILED = 2, COMM_ERROR = 3, MINOR_ERROR = 4, CRITICAL_ERROR = 5, SD_DL_PROGRESS = 6
};


struct __attribute__((packed)) IMUdata
{
	int16_t gyroData1[3];
	int16_t accData1[3];
	int16_t gyroData2[3];
	int16_t accData2[3];
	double gyroFiltered[3];
	int8_t imu_config;
	uint64_t sysTime;
};

struct __attribute__((packed)) HkData
{
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

struct __attribute__((packed)) CmdData
{
	uint32_t vot;
	uint8_t valveState;
	uint8_t signals;
	uint16_t deltaOmegaStart;
	uint16_t deltaOmegaOperation;
	uint16_t deltaOmegaShutdown;
};

struct __attribute__((packed)) DownlinkIMU
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

struct __attribute__((packed)) DownlinkPresTemp
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

struct __attribute__((packed)) DownlinkCalc
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

struct __attribute__((packed)) TcData
{
	short moduleID;
	int payload;
};

/*struct __attribute__((packed)) Status
{
	short id; 
	bool systemBad;
	int errorcode;
};*/

struct __attribute__((packed)) DpCommand
{
    uint8_t sync;
    uint8_t id;
    uint32_t argument;
    uint32_t check;
};

struct __attribute__((packed)) DpGeneric
{
	uint8_t sync;
	uint8_t data[63];
};

struct __attribute__((packed)) DpFinfo
{
    uint8_t sync;
    uint32_t counter;
    char fname[13];
    uint32_t check;
};

struct __attribute__((packed)) DpFdata
{
    uint8_t sync;
    uint32_t counter;
    uint8_t fdata[55];
    uint32_t check;
};

struct __attribute__((packed)) DpFend
{
    uint8_t sync;
    uint32_t counter;
    uint8_t fdata[54];
    uint8_t remaining_bytes;
    uint32_t check;
};

struct __attribute__((packed)) DpDLcomplete
{
	 uint8_t sync;
	 uint32_t counter;
	 uint32_t check;
};

struct __attribute__((packed)) DpDebug
{
	uint8_t sync;
	char text[59];
	uint32_t check;
};

#endif
