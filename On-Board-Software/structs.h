#ifndef STRUCTS_H
#define STRUCTS_H

#include "rodos.h"

#define SYNC_IMU    (char)0xA8 // 0b10101000
#define SYNC_PT     (char)0xA4 // 0b10100100
#define SYNC_CALC   (char)0xA2 // 0b10100010
#define SYNC_COMM   (char)0xA1 // 0b10100001

#define DELETE_SD         0x00
#define SHUTDOWN        0x01
#define RESTART         0x02
#define MODE_STANDBY    0x03
#define MODE_FLIGHT     0x04
#define MODE_SECURE     0x05
#define MODE_BD         0x06
#define TEST_ACTIVATE   0x07
#define TEST_LEAVE      0x08


struct __attribute__((packed)) IMUdata
{
	uint16_t gyroData1[3];
	uint16_t accData1[3];
	uint16_t gyroData2[3];
	uint16_t accData2[3];
	float gyroFiltered[3];
	float accFiltered[3];
	uint64_t sysTime;
};

struct __attribute__((packed)) hkData
{
	uint16_t presTank;
	uint16_t presValves;
	uint16_t tempTank;
	uint16_t tempNoz1;
	uint16_t tempNoz2;
	uint16_t tempNoz3;
	uint16_t tempNoz4;
	uint16_t tempPDU;
	uint64_t sysTime;
};

struct __attribute__((packed)) cmdData
{
	uint8_t valveState;
	uint16_t vot1;
	uint16_t vot2;
};

struct __attribute__((packed)) tcData
{
	short moduleID;
	int payload;
};

struct __attribute__((packed)) status
{
	short id; 
	bool systemBad;
	int errorcode;
};

struct __attribute__((packed)) dpCommand
{
    uint8_t sync;
    uint8_t id;
    uint16_t counter;
    uint32_t check;
};

#endif
