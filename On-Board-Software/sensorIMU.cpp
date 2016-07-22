#include "sensorIMU.h"

#define GYRO_OUT		0x43
#define ACC_OUT			0x3B

#define CONFIG			0x1A
#define GYRO_CONFIG		0x1B
#define ACCEL_CONFIG	0x1C
#define ACCEL_CONFIG2	0x1D


HAL_SPI imu1(SPI_IDX1);
HAL_SPI imu2(SPI_IDX2);

uint8_t readGyroAddr[] = {GYRO_OUT};
uint8_t readAccAddr[] = {ACC_OUT};

uint8_t gyroConfig[] = {GYRO_CONFIG, 0b00000000}
// 7-5: X/Y/Z Gyro self-test; 4-3: Full Scale (00 - 250, 11 - 2000); 2: reserved; 1-0: DLPF

uint8_t accelConfig[] = {ACCEL_CONFIG, 0b00011000}
// 7-5: X/Y/Z Accel self-test; 4-3: Full Scale (00 - 2g, 11 - 16g)

int SensorIMU::initIMUS()
{
	if (imu1.init() == -1)	// TODO (optional): Buadrade angeben, Mode per config
		return -1;			// TODO: error handling
	if (imu2.init() == -1)
		return -1;
		
	return 0;
}

int SensorIMU::configIMUS(int scale)
{
	if (imu1.write(gyroConfig, 2) == -1)
		return -1;
	if (imu1.write(accelConfig, 2) == -1)
		return -1;
	if (imu2.write(gyroConfig, 2) == -1)
		return -1;
	if (imu2.write(accelConfig, 2) == -1)
		return -1;
	
	return 0;
}

int SensorIMU::getIMU1(uint16_t *buffer)
{
	if (imu1.writeRead(readGyroAddr, 1, (uint8_t*)buffer, 6) == -1)
		return -1;
	
	return 0;
}

int SensorIMU::getIMU2(uint16_t *buffer)
{
	if (imu2.writeRead(readGyroAddr, 1, (uint8_t*)buffer, 6) == -1)
		return -1;
	
	return 0;
}

void init()
{
	initIMUs();
}

void SensorIMU::run()
{
	configIMUs();
	setPeriodicBeat(0, 50*MILLISECONDS);
	uint16_t gyroBuffer1[3];
	uint16_t accBuffer1[3];
	uint16_t gyroBuffer2[3];
	uint16_t accBuffer2[3];
	while(1)
	{
		getIMU1(gyroBuffer1);
		getIMU1(accBuffer1);
		getIMU2(gyroBuffer2);
		getIMU2(accBuffer2);
		
		memcpy(imu.gyroData1, gyroBuffer1, 6);
		memcpy(imu.gyroData2, gyroBuffer2, 6);
		memcpy(imu.accData1, accBuffer1, 6);
		memcpy(imu.accData2, accBuffer2, 6);
		
		// TODO Filter
		
		imu.sysTime = NOW();
		
		IMUTopic.publish(imu);
		
		suspendUntilNextBeat();
	}
}