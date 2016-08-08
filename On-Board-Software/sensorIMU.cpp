#include "sensorIMU.h"

#define DEBUG_DUMMY_DATA

SensorIMU sensorIMU;

#define GYRO_OUT		0x43
#define ACC_OUT			0x3B

#define CONFIG			0x1A
#define GYRO_CONFIG		0x1B
#define ACCEL_CONFIG	0x1C
#define ACCEL_CONFIG2	0x1D

uint8_t readGyroAddr[] = {GYRO_OUT};
uint8_t readAccAddr[] = {ACC_OUT};

uint8_t gyroConfig[] = {GYRO_CONFIG, 0b00000000};
// 7-5: X/Y/Z Gyro self-test; 4-3: Full Scale (00 - 250, 11 - 2000); 2: reserved; 1-0: DLPF

uint8_t accelConfig[] = {ACCEL_CONFIG, 0b00011000};
// 7-5: X/Y/Z Accel self-test; 4-3: Full Scale (00 - 2g, 11 - 16g)

#define GYRO_FACTOR         0.00762939453125    // Max. Scale: 250 deg/s
#define ACC_FACTOR          0.00048828125       // Max. Scale: 16g

#ifdef DEBUG_DUMMY_DATA

double dummy_gyro1_roll[] = {30.1, 28.6, 25.4, 22.3, 20.1, 26.7};
double dummy_gyro2_roll[] = {30.5, 27.8, 25.6, 22.1, 19.9, 27.0};
double dummy_gyro1_pitch[] = {5.9, 6.8, 7.2, 9.8, 8.2, 6.3};
double dummy_gyro2_pitch[] = {6.0, 6.5, 7.0, 10.5, 8.7, 6.0};
double dummy_gyro1_yaw[] = {4.1, 5.5, 6.3, 5.9, 4.5, 4.0};
double dummy_gyro2_yaw[] = {4.3, 5.2, 6.0, 6.1, 4.7, 4.0};
double dummy_acc1_x[] = {5.6, 2.4, 3.9, 8.5, 1.2, 4.6};
double dummy_acc2_x[] = {5.8, 2.1, 3.5, 8.0, 1.3, 4.7};
double dummy_acc1_y[] = {2.5, 3.6, 2.4, 1.7, 2.5, 3.0};
double dummy_acc2_y[] = {2.2, 3.7, 2.3, 1.8, 2.1, 3.1};
double dummy_acc1_z[] = {14.5, 16.0, 15.6, 14.2, 15.3, 14.4};
double dummy_acc2_z[] = {14.3, 16.1, 15.2, 14.0, 15.6, 14.1};

int dummy_cycle = 0;

#endif

int SensorIMU::configIMUs()
{
    spi_comm_running.enter();

    spiHelper.selectSlave(IMU1);
    if (spi_bus.write(gyroConfig, 2) == -1)
        return -1;
    if (spi_bus.write(accelConfig, 2) == -1)
        return -1;

    spiHelper.disableSlaves();

    spiHelper.selectSlave(IMU2);
    if (spi_bus.write(gyroConfig, 2) == -1)
		return -1;
    if (spi_bus.write(accelConfig, 2) == -1)
        return -1;
	
    spiHelper.disableSlaves();

    spi_comm_running.leave();

	return 0;
}

int SensorIMU::getIMU1(uint16_t *bufferGyro, uint16_t *bufferAcc)
{
    spi_comm_running.enter();

    spiHelper.selectSlave(IMU1);

    if (spi_bus.writeRead(readGyroAddr, 1, (uint8_t*)bufferGyro, 6) == -1)
    {
        spiHelper.disableSlaves();
		return -1;
    }
    if (spi_bus.writeRead(readAccAddr, 1, (uint8_t*)bufferAcc, 6) == -1)
    {
        spiHelper.disableSlaves();
        return -1;
    }
	
    spiHelper.disableSlaves();

    spi_comm_running.leave();

	return 0;
}

int SensorIMU::getIMU2(uint16_t *bufferGyro, uint16_t *bufferAcc)
{
    spi_comm_running.enter();

    spiHelper.selectSlave(IMU2);

    if (spi_bus.writeRead(readGyroAddr, 1, (uint8_t*)bufferGyro, 6) == -1)
    {
        spiHelper.disableSlaves();
		return -1;
    }
    if (spi_bus.writeRead(readAccAddr, 1, (uint8_t*)bufferAcc, 6) == -1)
    {
        spiHelper.disableSlaves();
        return -1;
    }
	
    spiHelper.disableSlaves();

    spi_comm_running.leave();

	return 0;
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
        getIMU1(gyroBuffer1, accBuffer1);
        getIMU2(gyroBuffer2, accBuffer2);
		
		memcpy(imu.gyroData1, gyroBuffer1, 6);
		memcpy(imu.gyroData2, gyroBuffer2, 6);
		memcpy(imu.accData1, accBuffer1, 6);
		memcpy(imu.accData2, accBuffer2, 6);
		
		#ifdef DEBUG_DUMMY_DATA

		imu.gyroData1[0] = (uint16_t)(dummy_gyro1_roll[dummy_cycle] / GYRO_FACTOR);
		imu.gyroData1[1] = (uint16_t)(dummy_gyro1_pitch[dummy_cycle] / GYRO_FACTOR);
		imu.gyroData1[2] = (uint16_t)(dummy_gyro1_yaw[dummy_cycle] / GYRO_FACTOR);
		imu.gyroData2[0] = (uint16_t)(dummy_gyro2_roll[dummy_cycle] / GYRO_FACTOR);
		imu.gyroData2[1] = (uint16_t)(dummy_gyro2_pitch[dummy_cycle] / GYRO_FACTOR);
		imu.gyroData2[2] = (uint16_t)(dummy_gyro2_yaw[dummy_cycle] / GYRO_FACTOR);

		imu.accData1[0] = (uint16_t)(dummy_acc1_x[dummy_cycle] / ACC_FACTOR);
		imu.accData1[1] = (uint16_t)(dummy_acc1_y[dummy_cycle] / ACC_FACTOR);
		imu.accData1[2] = (uint16_t)(dummy_acc1_z[dummy_cycle] / ACC_FACTOR);
		imu.accData2[0] = (uint16_t)(dummy_acc2_x[dummy_cycle] / ACC_FACTOR);
		imu.accData2[1] = (uint16_t)(dummy_acc2_y[dummy_cycle] / ACC_FACTOR);
		imu.accData2[2] = (uint16_t)(dummy_acc2_z[dummy_cycle] / ACC_FACTOR);

		dummy_cycle++;
		if (dummy_cycle > 5) dummy_cycle = 0;

		#endif

		// TODO Filter
		
		imu.sysTime = NOW();
		
		IMUTopic.publish(imu);
		
		suspendUntilNextBeat();
	}
}
