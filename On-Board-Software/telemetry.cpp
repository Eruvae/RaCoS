#include "telemetry.h"

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
		
		// TODO send Data
		
		suspendUntilNextBeat();
	}
}

int Telemetry::encodeIMU(char *buffer)
{
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_IMU;
	
	uint64_t time = NOW() / MILLISECONDS;
	buffer[1] = time & 0xFF;
	buffer[2] = time & 0xFF00;
	buffer[3] = time & 0xFF0000;
	
	*(uint16_t*)&buffer[4] = counter;
	
	IMUBuffer.get(imu);
	
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
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_PT;
	
	uint64_t time = NOW() / MILLISECONDS;
	buffer[1] = time & 0xFF;
	buffer[2] = time & 0xFF00;
	buffer[3] = time & 0xFF0000;
	
	*(uint16_t*)&buffer[4] = counter;
	
	//PressureBuffer.get(pres);
	//TemperatureBuffer.get(temp);
	hkBuffer.get(hk);
	
	//memcpy(&buffer[6], &pres, sizeof(pres));
	//memcpy(&buffer[10], &temp, sizeof(temp));
	memcpy(&hkBuffer[6], &temp, 14);
	
	*(int32_t*)&buffer[22] = generateChecksum(buffer, 22);
	
	counter++;
	
	return 26;
}

int Telemetry::encodeCalc(char *buffer)
{
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_CALC;
	
	uint64_t time = NOW() / MILLISECONDS;
	buffer[1] = time & 0xFF;
	buffer[2] = time & 0xFF00;
	buffer[3] = time & 0xFF0000;
	
	*(uint16_t*)&buffer[4] = counter;
	
	controlBuffer.get(calc);
	//ModeBuffer.get(mode);
	
	buffer[5] = mode;
	
	memcpy(&buffer[6], &calc, 5);
	
	
	*(int32_t*)&buffer[10] = generateChecksum(buffer, 10);
	
	counter++;
	
	return 14;
	
}