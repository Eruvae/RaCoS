#ifndef STRUCTS_H
#define STRUCTS_H

#include "rodos.h"

#define SYNC_IMU    (uint8_t)0xA8 // 0b10101000
#define SYNC_PT     (uint8_t)0xA4 // 0b10100100
#define SYNC_CALC   (uint8_t)0xA2 // 0b10100010
#define SYNC_COMM   (uint8_t)0xA1 // 0b10100001

#define DELETE_SD       0x00
#define SHUTDOWN        0x01
#define RESTART         0x02
#define MODE_STANDBY    0x03
#define MODE_FLIGHT     0x04
#define MODE_SECURE     0x05
#define MODE_BD         0x06
#define TEST_ACTIVATE   0x07
#define TEST_LEAVE      0x08


enum Mode
{
	STAND_BY = 0x03, FLIGHT = 0x04, SECURE = 0x05, BLOW_DOWN = 0x06
};


struct __attribute__((packed)) IMUdata
{
	int16_t gyroData1[3];
	int16_t accData1[3];
	int16_t gyroData2[3];
	int16_t accData2[3];
	float gyroFiltered[3];
	float accFiltered[3];
	uint64_t sysTime;
};

struct __attribute__((packed)) HkData
{
	uint16_t presTank;
	uint16_t presValves;
	int16_t tempTank;
	int16_t tempNoz1;
	int16_t tempNoz2;
	int16_t tempNoz3;
	int16_t tempNoz4;
	int16_t tempPDU;
	uint64_t sysTime;
};

struct __attribute__((packed)) CmdData
{
	uint8_t valveState;
	uint16_t vot1;
	uint16_t vot2;
};

struct __attribute__((packed)) TcData
{
	short moduleID;
	int payload;
};

struct __attribute__((packed)) Status
{
	short id; 
	bool systemBad;
	int errorcode;
};

struct __attribute__((packed)) DpCommand
{
    uint8_t sync;
    uint8_t id;
    uint16_t counter;
    uint32_t check;
};

#endif
