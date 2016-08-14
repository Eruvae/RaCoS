#include "sensorIMU.h"

//#define DEBUG_IMU_DUMMY_DATA
//#define DEBUG_WHO_AM_I

SensorIMU sensorIMU;

#define GYRO_OUT		(0x43 | 0x80)
#define ACC_OUT			(0x3B | 0x80)

#define CONFIG			0x1A
#define GYRO_CONFIG		0x1B
#define ACCEL_CONFIG	0x1C
#define ACCEL_CONFIG2	0x1D
#define PWR_MGMT_1		0x6B

#define WHO_AM_I		(0x75 | 0x80)

uint8_t readGyroAddr[] = {GYRO_OUT};
uint8_t readAccAddr[] = {ACC_OUT};

uint8_t resetIMU[] = {PWR_MGMT_1, 0x80};
uint8_t readReset[2] = {PWR_MGMT_1 | 0x80, 0xFF};

uint8_t powerConfig[] = {PWR_MGMT_1, 0};

uint8_t gyroConfig[] = {GYRO_CONFIG, 0b00000000};
// 7-5: X/Y/Z Gyro self-test; 4-3: Full Scale (00 - 250, 11 - 2000); 2: reserved; 1-0: DLPF

uint8_t accelConfig[] = {ACCEL_CONFIG, 0b00011000};
// 7-5: X/Y/Z Accel self-test; 4-3: Full Scale (00 - 2g, 11 - 16g)

#define GYRO_FACTOR         0.00762939453125    // Max. Scale: 250 deg/s
//#define ACC_FACTOR          0.00048828125       // Max. Scale: 16g
#define ACC_FACTOR          0.00006103515625

#ifdef DEBUG_IMU_DUMMY_DATA

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

int SensorIMU::configIMU(SPI_SS id)
{
	if (!(id == IMU1 || id == IMU2))
		return -1;

    spi_comm_running.enter();

    spiHelper.selectSlave(id);

    if (spi_bus.write(resetIMU, 2) == -1)
    	return -1;

    uint8_t buffer[2] = {0xFF, 0xFF};

    while(1)
    {
    	spi_bus.writeRead(readReset, 2, buffer, 2);
    	if (!(buffer[1] & 0x80))
    		break;
    }

    //suspendCallerUntil(NOW() + 1*SECONDS);

    if (spi_bus.write(powerConfig, 2) == -1)
    	return -1;

    spi_bus.suspendUntilWriteFinished();
    //suspendCallerUntil(NOW() + 1*SECONDS);

    if (spi_bus.write(gyroConfig, 2) == -1)
        return -1;

    spi_bus.suspendUntilWriteFinished();
    //suspendCallerUntil(NOW() + 1*SECONDS);

    if (spi_bus.write(accelConfig, 2) == -1)
        return -1;

    spi_bus.suspendUntilWriteFinished();
    //suspendCallerUntil(NOW() + 1*SECONDS);

    spiHelper.disableSlaves();

    spi_comm_running.leave();

	return 0;
}

int SensorIMU::getIMU(SPI_SS id, IMUReadStruct *buffer)
{
	if (!(id == IMU1 || id == IMU2))
		return -1;

    spi_comm_running.enter();

    spiHelper.selectSlave(id);

    if (spi_bus.writeRead(readAccAddr, 1, (uint8_t*)buffer, sizeof(IMUReadStruct)) == -1)
	{
		spiHelper.disableSlaves();
		return -1;
	}

    spiHelper.disableSlaves();

    spi_comm_running.leave();

    buffer->gyroData[0] = swap16(buffer->gyroData[0]);
    buffer->gyroData[1] = swap16(buffer->gyroData[1]);
    buffer->gyroData[2] = swap16(buffer->gyroData[2]);
    // buffer->tempData = swap16(buffer->tempData); // Temperature not needed
    buffer->accData[0] = swap16(buffer->accData[0]);
	buffer->accData[1] = swap16(buffer->accData[1]);
	buffer->accData[2] = swap16(buffer->accData[2]);

	return 0;
}

void SensorIMU::run()
{
	configIMU(IMU1);
	configIMU(IMU2);
	setPeriodicBeat(0, 50*MILLISECONDS);
	IMUReadStruct imu1_buf, imu2_buf;
	while(1)
	{
        getIMU(IMU1, &imu1_buf);
        getIMU(IMU2, &imu2_buf);
		
		memcpy(imu.gyroData1, imu1_buf.gyroData, 6);
		memcpy(imu.gyroData2, imu2_buf.gyroData, 6);
		memcpy(imu.accData1, imu1_buf.accData, 6);
		memcpy(imu.accData2, imu2_buf.accData, 6);

		/*PRINTF("Acc1: %d\n", imu1_buf.accData[0]);
		PRINTF("Acc2: %d\n", imu1_buf.accData[1]);
		PRINTF("Acc3: %d\n", imu1_buf.accData[2]);

		PRINTF("Gyro1: %d\n", imu1_buf.gyroData[0]);
		PRINTF("Gyro2: %d\n", imu1_buf.gyroData[1]);
		PRINTF("Gyro3: %d\n", imu1_buf.gyroData[2]);*/

		#ifdef DEBUG_IMU_DUMMY_DATA

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


		#ifdef DEBUG_WHO_AM_I
		spi_comm_running.enter();

		spiHelper.selectSlave(IMU1);

		uint8_t whoami[2] = {WHO_AM_I, 0xFF};
		uint8_t answer[2];

		spi_bus.writeRead(whoami, 2, answer, 2);

		PRINTF("Device 1: I am %d, %d\n", answer[0], answer[1]);

		spiHelper.disableSlaves();
		spiHelper.selectSlave(IMU2);

		spi_bus.writeRead(whoami, 2, answer, 2);

		PRINTF("Device 2: I am %d, %d\n", answer[0], answer[1]);

		spiHelper.disableSlaves();

		spi_comm_running.leave();
		#endif


		// TODO Filter
		
		imu.sysTime = NOW();
		
		IMUTopic.publish(imu);
		
		suspendUntilNextBeat();
	}
}
