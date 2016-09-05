#include "sensorIMU.h"

//#define DEBUG_IMU_DUMMY_DATA
//#define DEBUG_WHO_AM_I

SensorIMU sensorIMU;

#define READ_FLAG		0x80

#define ACC_OUT			0x3B
//#define TEMP_OUT		0x41
//#define GYRO_OUT		0x43

#define CONFIG			0x1A
#define GYRO_CONFIG		0x1B
#define ACCEL_CONFIG	0x1C
#define ACCEL_CONFIG2	0x1D
#define PWR_MGMT_1		0x6B

#define WHO_AM_I		0x75

#define BITS_FS_250DPS              0x00
#define BITS_FS_500DPS              0x08
#define BITS_FS_1000DPS             0x10
#define BITS_FS_2000DPS             0x18

#define BITS_FS_2G                  0x00
#define BITS_FS_4G                  0x08
#define BITS_FS_8G                  0x10
#define BITS_FS_16G                 0x18

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

double calibG1X = -1;
double calibG1Y = -1;
double calibG1Z = -1;
double calibG2X = -1;
double calibG2Y = -1;
double calibG2Z = -1;
int fusionCycle = 0;

int SensorIMU::resetIMU(SPI_SS id) {
	if (!(id == IMU1 || id == IMU2))
		return -1;

	uint8_t resetIMU[] = { PWR_MGMT_1, 0x80 };
	uint8_t readReset[2] = { PWR_MGMT_1 | 0x80, 0xFF };

	spiHelper.selectSlave(id);

	if (spi_bus.write(resetIMU, 2) == -1) {
		spiHelper.disableSlaves();
		return -1;
	}

	uint8_t buffer[2] = { 0xFF, 0xFF };

	int retVal = -1;

	for (int i = 0; i < 100; i++) {
		spi_bus.writeRead(readReset, 2, buffer, 2);
		if (!(buffer[1] & 0x80)) {
			retVal = 0;
			break;
		}
	}

	spiHelper.disableSlaves();

	return retVal;
}

int SensorIMU::configReg(SPI_SS id, uint8_t reg, uint8_t config) {
	if (!(id == IMU1 || id == IMU2))
		return -1;

	uint8_t regConfig[] = { reg, config };

	spiHelper.selectSlave(id);

	int retVal = 0;

	if (spi_bus.write(regConfig, 2) == -1)
		retVal = -1;

	spiHelper.disableSlaves();

	return retVal;
}

int SensorIMU::initIMU(SPI_SS id) {
	if (resetIMU(id) == -1)
		return -1;

	suspendCallerUntil(NOW()+ 1*MILLISECONDS);

	if (configReg(id, PWR_MGMT_1, 0) == -1)
		return -1;

	suspendCallerUntil(NOW()+ 1*MILLISECONDS);

	if (configReg(id, GYRO_CONFIG, BITS_FS_250DPS) == -1)
		return -1;

	suspendCallerUntil(NOW()+ 1*MILLISECONDS);

	if (configReg(id, ACCEL_CONFIG, BITS_FS_16G) == -1)
		return -1;

	suspendCallerUntil(NOW()+ 1*MILLISECONDS);

	return 0;
}

int SensorIMU::getIMU(SPI_SS id, IMUReadStruct *buffer) {
	if (!(id == IMU1 || id == IMU2))
		return -1;

	spiHelper.selectSlave(id);

	uint8_t readAddr = ACC_OUT | READ_FLAG;

	spi_bus.write(&readAddr, 1);

	if (spi_bus.read((uint8_t*) buffer, sizeof(IMUReadStruct)) == -1) {
		spiHelper.disableSlaves();
		return -1;
	}

	spiHelper.disableSlaves();

	buffer->gyroData[0] = swap16(buffer->gyroData[0]);
	buffer->gyroData[1] = swap16(buffer->gyroData[1]);
	buffer->gyroData[2] = swap16(buffer->gyroData[2]);
	// buffer->tempData = swap16(buffer->tempData); // Temperature not needed
	buffer->accData[0] = swap16(buffer->accData[0]);
	buffer->accData[1] = swap16(buffer->accData[1]);
	buffer->accData[2] = swap16(buffer->accData[2]);

	return 0;
}

int SensorIMU::calibrate() {
	IMUReadStruct imu1_buf, imu2_buf;
	double tempG1X = 0;
	double tempG1Y = 0;
	double tempG1Z = 0;
	double tempG2X = 0;
	double tempG2Y = 0;
	double tempG2Z = 0;

	for (int i = 0; i < 1000; i++) {
		getIMU(IMU1, &imu1_buf);
		getIMU(IMU2, &imu2_buf);
		tempG1X += imu1_buf.gyroData[0];
		tempG1Y += imu1_buf.gyroData[1];
		tempG1Z += imu1_buf.gyroData[2];
		tempG2X += imu2_buf.gyroData[0];
		tempG2Y += imu2_buf.gyroData[1];
		tempG2Z += imu2_buf.gyroData[2];
		suspendCallerUntil(NOW()+1*MILLISECONDS);
	}
	calibG1X = tempG1X / 1000.0;
	calibG1Y = tempG1Y / 1000.0;
	calibG1Z = tempG1Z / 1000.0;
	calibG2X = tempG2X / 1000.0;
	calibG2Y = tempG2Y / 1000.0;
	calibG2Z = tempG2Z / 1000.0;
}

void SensorIMU::fusionFilter(IMUdata &imu) {
	GxHistory1[fusionCycle] = (imu.gyroData1[0] - calibG1X) * GYRO_FACTOR;
	GyHistory1[fusionCycle] = (imu.gyroData1[1] - calibG1Y) * GYRO_FACTOR;
	GzHistory1[fusionCycle] = (imu.gyroData1[2] - calibG1Z) * GYRO_FACTOR;
	GxHistory2[fusionCycle] = (imu.gyroData2[0] - calibG2X) * GYRO_FACTOR;
	GyHistory2[fusionCycle] = (imu.gyroData2[1] - calibG2Y) * GYRO_FACTOR;
	GzHistory2[fusionCycle] = (imu.gyroData2[2] - calibG2Z) * GYRO_FACTOR;
	fusionCycle = ++fusionCycle > 4 ? 0 : fusionCycle;

	double fusionX1 = 0;
	double fusionY1 = 0;
	double fusionZ1 = 0;
	double fusionX2 = 0;
	double fusionY2 = 0;
	double fusionZ2 = 0;
	double noiseG1[3] = { 0, 0, 0 };
	double noiseG2[3] = { 0, 0, 0 };
	//Average over the last 5 values of the specified IMU in 3 Axis
	for (int i = 0; i < 5; i++) {
		if (i > 0) {
			double temp = GxHistory1[i] - GxHistory1[i - 1];
			noiseG1[0] += temp < 0 ? -temp : temp;
			temp = GyHistory1[i] - GxHistory1[i - 1];
			noiseG1[1] += temp < 0 ? -temp : temp;
			temp = GzHistory1[i] - GxHistory1[i - 1];
			noiseG1[2] += temp < 0 ? -temp : temp;
			temp = GxHistory1[i] - GxHistory1[i - 1];
			noiseG2[0] += temp < 0 ? -temp : temp;
			temp = GyHistory1[i] - GxHistory1[i - 1];
			noiseG2[1] += temp < 0 ? -temp : temp;
			temp = GzHistory1[i] - GxHistory1[i - 1];
			noiseG2[2] += temp < 0 ? -temp : temp;
		}
		fusionX1 += GxHistory1[i];
		fusionY1 += GyHistory1[i];
		fusionZ1 += GzHistory1[i];
		fusionX2 += GxHistory2[i];
		fusionY2 += GyHistory2[i];
		fusionZ2 += GzHistory2[i];
	}
	fusionX1 /= 5;
	fusionY1 /= 5;
	fusionZ1 /= 5;
	fusionX2 /= 5;
	fusionY2 /= 5;
	fusionZ2 /= 5;
	noiseG1[0] /= 4;
	noiseG1[1] /= 4;
	noiseG1[2] /= 4;
	noiseG2[0] /= 4;
	noiseG2[1] /= 4;
	noiseG2[2] /= 4;

	double prescaler[9];
	for (int i = 0; i < 3; i++) {
		prescaler[i] = 0.5 / (1.0 + noiseG1[i]);
		prescaler[i + 3] = 0.5 / (1.0 + noiseG2[i]);
		prescaler[i + 6] = 1 - prescaler[i] - prescaler[i + 3];
	}
	imu.gyroFiltered[0] = (prescaler[0] * fusionX1) + (prescaler[3] * fusionX2)
			+ (prescaler[6] * filterXLast);
	imu.gyroFiltered[1] = (prescaler[1] * fusionY1) + (prescaler[4] * fusionY2)
			+ (prescaler[7] * filterYLast);
	imu.gyroFiltered[2] = (prescaler[2] * fusionZ1) + (prescaler[5] * fusionZ2)
			+ (prescaler[8] * filterZLast);
	filterXLast = imu.gyroFiltered[0];
	filterYLast = imu.gyroFiltered[1];
	filterZLast = imu.gyroFiltered[2];
}

void SensorIMU::run() {
	initIMU(IMU1);
	initIMU(IMU2);
	calibrate();
	setPeriodicBeat(0, 10 * MILLISECONDS);
	IMUReadStruct imu1_buf, imu2_buf;
	while (1) {
		getIMU(IMU1, &imu1_buf);
		getIMU(IMU2, &imu2_buf);

		memcpy(imu.gyroData1, imu1_buf.gyroData, 6);
		memcpy(imu.gyroData2, imu2_buf.gyroData, 6);
		memcpy(imu.accData1, imu1_buf.accData, 6);
		memcpy(imu.accData2, imu2_buf.accData, 6);
		fusionFilter(imu);

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

		spiHelper.selectSlave(IMU1);

		uint8_t whoami[2] = {WHO_AM_I | READ_FLAG, 0xFF};
		uint8_t answer[2];

		spi_bus.writeRead(whoami, 2, answer, 2);

		PRINTF("Device 1: I am %d, %d\n", answer[0], answer[1]);

		spiHelper.disableSlaves();
		spiHelper.selectSlave(IMU2);

		spi_bus.writeRead(whoami, 2, answer, 2);

		PRINTF("Device 2: I am %d, %d\n", answer[0], answer[1]);

		spiHelper.disableSlaves();

#endif

		// TODO Filter

		imu.sysTime = NOW();

		IMUTopic.publish(imu);

		suspendUntilNextBeat();
	}
}
