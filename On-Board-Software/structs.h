#ifndef STRUCTS_H
#define STRUCTS_H

#include "rodos.h"

#define SYNC_IMU    (char)0xA8 // 0b10101000
#define SYNC_PT     (char)0xA4 // 0b10100100
#define SYNC_CALC   (char)0xA2 // 0b10100010

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
	uint16_t tempTank;
	uint16_t tempNoz1;
	uint16_t tempNoz2;
	uint16_t tempNoz3;
	uint16_t tempNoz4;
	uint16_t presTank;
	uint16_t presValves;
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

#endif
