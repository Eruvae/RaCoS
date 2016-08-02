#include "telemetry.h"

Telemetry telemetry;

void Telemetry::run()
{
	setPeriodicBeat(0, 500*MILLISECONDS);
	while(1)
	{
		char imuBuf[34];
		char ptBuf[26];
		char calcBuf[14];
		encodeIMU(imuBuf);
		encodePresTemp(ptBuf);
		encodeCalc(calcBuf);
		
        teleUART.write(imuBuf, 34);
        teleUART.write(ptBuf, 26);
        teleUART.write(calcBuf, 14);
		
		suspendUntilNextBeat();
	}
}

int Telemetry::encodeIMU(char *buffer)
{
	IMUBuffer.get(imu);
	
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_IMU;
	
	//uint64_t time = NOW() / MILLISECONDS;
	buffer[1] = imu.sysTime & 0xFF;
	buffer[2] = imu.sysTime & 0xFF00;
	buffer[3] = imu.sysTime & 0xFF0000;
	
	*(uint16_t*)&buffer[4] = counter;
	
	memcpy(&buffer[6], &imu, 24);
	
	/**(uint16_t*)&buffer[6] = imu.gyroData1[0];
	*(uint16_t*)&buffer[8] = imu.gyroData1[1];
	*(uint16_t*)&buffer[10] = imu.gyroData1[2];
	*(uint16_t*)&buffer[12] = imu.accData1[0];
	*(uint16_t*)&buffer[14] = imu.accData1[1];
	*(uint16_t*)&buffer[16] = imu.accData1[2];
	*(uint16_t*)&buffer[18] = imu.gyroData2[0];
	*(uint16_t*)&buffer[20] = imu.gyroData2[1];
	*(uint16_t*)&buffer[22] = imu.gyroData2[2];
	*(uint16_t*)&buffer[24] = imu.accData2[0];
	*(uint16_t*)&buffer[26] = imu.accData2[1];
	*(uint16_t*)&buffer[28] = imu.accData2[2];*/
	
	*(int32_t*)&buffer[30] = generateChecksum(buffer, 30);
	
	counter++;
	
	return 34;
}

int Telemetry::encodePresTemp(char *buffer)
{
	//PressureBuffer.get(pres);
	//TemperatureBuffer.get(temp);
	hkBuffer.get(hk);
	
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_PT;
	
	//uint64_t time = NOW() / MILLISECONDS;
	buffer[1] = hk.sysTime & 0xFF;
	buffer[2] = hk.sysTime & 0xFF00;
	buffer[3] = hk.sysTime & 0xFF0000;
	
	*(uint16_t*)&buffer[4] = counter;
	
	//memcpy(&buffer[6], &pres, sizeof(pres));
	//memcpy(&buffer[10], &temp, sizeof(temp));
    memcpy(&buffer[6], &hk, 14);
	
	*(int32_t*)&buffer[22] = generateChecksum(buffer, 22);
	
	counter++;
	
	return 26;
}

int Telemetry::encodeCalc(char *buffer)
{
	controlBuffer.get(calc);
	//ModeBuffer.get(mode);
	
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_CALC;
	
	uint64_t time = NOW() / MILLISECONDS;
	buffer[1] = time & 0xFF;
	buffer[2] = time & 0xFF00;
	buffer[3] = time & 0xFF0000;
	
	*(uint16_t*)&buffer[4] = counter;
	
	buffer[5] = mode;
	
	memcpy(&buffer[6], &calc, 5);
	
	
	*(int32_t*)&buffer[10] = generateChecksum(buffer, 10);
	
	counter++;
	
	return 14;
	
}

int Telemetry::generateChecksum(char *buffer, int size)
{
    return Murmur::mm_hash_32((uint8_t*)buffer, size);
}
