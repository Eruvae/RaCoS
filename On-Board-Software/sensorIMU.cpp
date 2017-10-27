#include "sensorIMU.h"
#include "storagecontroller.h"
#include "actuatorhandler.h"
#include "telemetry.h"
#include <stdio.h>

//#define DEBUG_IMU_DUMMY_DATA
//#define DEBUG_WHO_AM_I
//#define DEBUG_SIMULATE_DATA

#ifdef DEBUG_SIMULATE_DATA

#define SIM_CNT 6
double times_simlist[SIM_CNT] = {0, 2, 5, 6, 8, 10};
double imu_value_simlist[SIM_CNT] = {30.0, 30.0, 15.0, 5.0, -5.0, -10.0};

#endif

SensorIMU sensorIMU("IMU Thread", DEFAULT_THREAD_PRIORITY);

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

#define MPUREG_I2C_MST_CTRL        0x24
#define MPUREG_I2C_SLV0_ADDR       0x25
#define MPUREG_I2C_SLV0_REG        0x26
#define MPUREG_I2C_SLV0_CTRL       0x27
#define MPUREG_INT_PIN_CFG		   0x37
#define MPUREG_EXT_SENS_DATA_00    0x49
#define MPUREG_I2C_SLV0_DO         0x63
#define MPUREG_USER_CTRL		   0x6A

#define AK8963_I2C_ADDR             0x0C //0x18
#define AK8963_HXL                  0x03
#define AK8963_CNTL1                0x0A
#define AK8963_CNTL2                0x0B

#define BITS_FS_250DPS              0x00
#define BITS_FS_500DPS              0x08
#define BITS_FS_1000DPS             0x10
#define BITS_FS_2000DPS             0x18

#define BITS_FS_2G                  0x00
#define BITS_FS_4G                  0x08
#define BITS_FS_8G                  0x10
#define BITS_FS_16G                 0x18

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

void SensorIMU::setIMUCalibration() {
	calibrateIMU = true;
}

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

int SensorIMU::readReg(SPI_SS id, uint8_t reg, void *buf, uint32_t cnt) {
	if (!(id == IMU1 || id == IMU2))
		return -1;

	uint8_t readAddr = reg | READ_FLAG;

	spiHelper.selectSlave(id);

	spi_bus.write(&readAddr, 1);
	int result = spi_bus.read((uint8_t*) buf, cnt);

	spiHelper.disableSlaves();

	return result;
}

int SensorIMU::configReg(SPI_SS id, uint8_t reg, uint8_t config) {
	if (!(id == IMU1 || id == IMU2))
		return -1;

	uint8_t regConfig[] = { reg, config };

	spiHelper.selectSlave(id);

	int result = spi_bus.write(regConfig, 2);

	spiHelper.disableSlaves();

	return result;
}

int SensorIMU::initIMU(SPI_SS id) {
	if (resetIMU(id) < 0)
		return -1;

	if (configReg(id, PWR_MGMT_1, 0) < 0)
		return -2;

	if (configReg(id, GYRO_CONFIG, scale_cfg) < 0)
		return -3;

	if (configReg(id, ACCEL_CONFIG, BITS_FS_16G) < 0)
		return -4;

	/*
	 if (configReg(id, MPUREG_INT_PIN_CFG, 0x30) == -1)
	 return -1;

	 if (configReg(id, MPUREG_USER_CTRL, 0x20) == -1)
	 return -1;

	 if (configReg(id, MPUREG_I2C_MST_CTRL, 0x0D) == -1)
	 return -1;

	 if (configReg(id, MPUREG_I2C_SLV0_ADDR, AK8963_I2C_ADDR) == -1)
	 return -1;

	 if (configReg(id, MPUREG_I2C_SLV0_REG, AK8963_CNTL2) == -1)
	 return -1;

	 if (configReg(id, MPUREG_I2C_SLV0_DO, 0x01) == -1)
	 return -1;

	 if (configReg(id, MPUREG_I2C_SLV0_CTRL, 0x81) == -1)
	 return -1;

	 if (configReg(id, MPUREG_I2C_SLV0_REG, AK8963_CNTL1) == -1)
	 return -1;

	 if (configReg(id, MPUREG_I2C_SLV0_DO, 0x12) == -1) // 16 bit measurement
	 return -1;

	 if (configReg(id, MPUREG_I2C_SLV0_CTRL, 0x81) == -1)
	 return -1;
	 */

	return 0;
}

int SensorIMU::getIMU(SPI_SS id, IMUReadStruct *buf) {
	if (!(id == IMU1 || id == IMU2))
		return -1;

	readReg(id, ACC_OUT, buf, sizeof(IMUReadStruct));

	buf->gyroData[0] = -swap16(buf->gyroData[0]);
	buf->gyroData[1] = swap16(buf->gyroData[1]);
	buf->gyroData[2] = -swap16(buf->gyroData[2]);
	buf->tempData = swap16(buf->tempData);
	buf->accData[0] = swap16(buf->accData[0]);
	buf->accData[1] = swap16(buf->accData[1]);
	buf->accData[2] = swap16(buf->accData[2]);

	return 0;
}

/*
 int SensorIMU::getMagnetometer(SPI_SS id, void *buffer) {
 configReg(id, MPUREG_I2C_SLV0_ADDR, AK8963_I2C_ADDR | READ_FLAG); //Set the I2C slave addres of AK8963 and set for read.
 configReg(id, MPUREG_I2C_SLV0_REG, AK8963_HXL); //I2C slave 0 register address from where to begin data transfer
 configReg(id, MPUREG_I2C_SLV0_CTRL, 0x87); //Read 7 bytes from the magnetometer
 readReg(id, MPUREG_EXT_SENS_DATA_00, buffer, 6);
 }
 */

int SensorIMU::calibrate() {
	IMUReadStruct imu1_buf, imu2_buf;
	double tempG1X = 0;
	double tempG1Y = 0;
	double tempG1Z = 0;
	double tempG2X = 0;
	double tempG2Y = 0;
	double tempG2Z = 0;
	double toSave[24];
	char forDebug[50];
	sprintf(forDebug, "Calibrate...");
	telemetry.dl_debug(forDebug);
	for (int j = 0; j < 4; j++) {
		sprintf(forDebug, "Cycle: %d",j);
		telemetry.dl_debug(forDebug);
		switch (j) {
		case 0:
			scale_cfg = GYRO_FACTOR250;
			break;
		case 1:
			scale_cfg = GYRO_FACTOR500;
			break;
		case 2:
			scale_cfg = GYRO_FACTOR1000;
			break;
		case 3:
			scale_cfg = GYRO_FACTOR2000;
			break;
		}
		if (configReg(IMU1, GYRO_CONFIG, scale_cfg) < 0 || configReg(IMU1, ACCEL_CONFIG, BITS_FS_16G) < 0)
			initIMU(IMU1);
		if (configReg(IMU2, GYRO_CONFIG, scale_cfg) < 0 || configReg(IMU2, ACCEL_CONFIG, BITS_FS_16G) < 0)
			initIMU(IMU2);
		suspendCallerUntil(NOW()+ 50 * MILLISECONDS);
		for (int i = 0; i < 2000; i++) {
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
		calibG1X[j] = tempG1X / 2000.0;
		calibG1Y[j] = tempG1Y / 2000.0;
		calibG1Z[j] = tempG1Z / 2000.0;
		calibG2X[j] = tempG2X / 2000.0;
		calibG2Y[j] = tempG2Y / 2000.0;
		calibG2Z[j] = tempG2Z / 2000.0;

		toSave[(0 + (j * 6))] = calibG1X[j];
		toSave[(1 + (j * 6))] = calibG1Y[j];
		toSave[(2 + (j * 6))] = calibG1Z[j];
		toSave[(3 + (j * 6))] = calibG2X[j];
		toSave[(4 + (j * 6))] = calibG2Y[j];
		toSave[(5 + (j * 6))] = calibG2Z[j];
		sprintf(forDebug, "Cycle %d finished!",j);
		telemetry.dl_debug(forDebug);
	}
	if(!storageController.writeToFile("IMUCAL", toSave, 24 * sizeof(double),
	FA_CREATE_ALWAYS)){
		sprintf(forDebug, "Calibration finished, Saving failed!");
	}
	else sprintf(forDebug, "Calibration finished!");
	telemetry.dl_debug(forDebug);

	/*unsigned int bytes;
	 FIL fil;
	 f_open(&fil, "IMU.calib", FA_WRITE | FA_CREATE_ALWAYS);
	 f_write(&fil, toSave, 6*sizeof(double), &bytes);
	 f_close(&fil);*/

	return 0;
}

void SensorIMU::autoscale() {
	double maxValue = fabs(filterZLast[0]);
	for (int i = 1; i < 5; i++) {
		maxValue = fmax(maxValue, fabs(filterZLast[i]));
	}
	int gyroScalerState =
			maxValue >= 200 ?
					maxValue >= 350 ? maxValue >= 800 ? 4 : 3 : 2
					: 1;

	if (gyroScalerState == scale_select + 1)
		return;

	switch (gyroScalerState) {
	case 1:
		scale = GYRO_FACTOR250;
		scale_cfg = BITS_FS_250DPS;
		scale_select = 0;
		break;
	case 2:
		scale = GYRO_FACTOR500;
		scale_cfg = BITS_FS_500DPS;
		scale_select = 1;
		break;
	case 3:
		scale = GYRO_FACTOR1000;
		scale_cfg = BITS_FS_1000DPS;
		scale_select = 2;
		break;
	case 4:
		scale = GYRO_FACTOR2000;
		scale_cfg = BITS_FS_2000DPS;
		scale_select = 3;
		break;
	default:
		scale = GYRO_FACTOR2000; //Never reached; default-value is maximum scale
		scale_cfg = BITS_FS_2000DPS;
		scale_select = 3;
		break;
	}
	if (configReg(IMU1, GYRO_CONFIG, scale_cfg) < 0 || configReg(IMU1, ACCEL_CONFIG, BITS_FS_16G) < 0)
		initIMU(IMU1);
	if (configReg(IMU2, GYRO_CONFIG, scale_cfg) < 0 || configReg(IMU2, ACCEL_CONFIG, BITS_FS_16G) < 0)
		initIMU(IMU2);

}

void SensorIMU::fusionFilter(IMUdata &imu) {
	GxHistory1[fusionCycle] = (imu.gyroData1[0] - calibG1X[scale_select]) * scale;
	GyHistory1[fusionCycle] = (imu.gyroData1[1] - calibG1Y[scale_select]) * scale;
	GzHistory1[fusionCycle] = (imu.gyroData1[2] - calibG1Z[scale_select]) * scale;
	GxHistory2[fusionCycle] = (imu.gyroData2[0] - calibG2X[scale_select]) * scale;
	GyHistory2[fusionCycle] = (imu.gyroData2[1] - calibG2Y[scale_select]) * scale;
	GzHistory2[fusionCycle] = (imu.gyroData2[2] - calibG2Z[scale_select]) * scale;

	double fusionX1 = 0;
	double fusionY1 = 0;
	double fusionZ1 = 0;
	double fusionX2 = 0;
	double fusionY2 = 0;
	double fusionZ2 = 0;
	double fusionLastXramp = 0;
	double fusionLastYramp = 0;
	double fusionLastZramp = 0;
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
			temp = GxHistory2[i] - GxHistory2[i - 1];
			noiseG2[0] += temp < 0 ? -temp : temp;
			temp = GyHistory2[i] - GxHistory2[i - 1];
			noiseG2[1] += temp < 0 ? -temp : temp;
			temp = GzHistory2[i] - GxHistory2[i - 1];
			noiseG2[2] += temp < 0 ? -temp : temp;
			fusionLastXramp += filterXLast[i - 1] - filterXLast[i];
			fusionLastYramp += filterYLast[i - 1] - filterYLast[i];
			fusionLastZramp += filterZLast[i - 1] - filterZLast[i];
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
	fusionLastXramp /= 4;
	fusionLastYramp /= 4;
	fusionLastZramp /= 4;
	noiseG1[0] /= 20;
	noiseG1[1] /= 20;
	noiseG1[2] /= 20;
	noiseG2[0] /= 20;
	noiseG2[1] /= 20;
	noiseG2[2] /= 20;

	//Fehlerbetrachtung: Wenn das Rauschen exakt 0 ist stimmt was mit dem Sensor nicht; Fehler reversibel, wenn er sich wieder meldet
	IMUworking[0] =
			noiseG1[0] == 0 && noiseG1[1] == 0 && noiseG1[2] == 0 ?
					false : true;
	IMUworking[1] =
			noiseG2[0] == 0 && noiseG2[1] == 0 && noiseG2[2] == 0 ?
					false : true;
	if (!IMUworking[0] && !IMUworking[1]) {

		sensorIMUStatusTopic.publishConst(CRITICAL_ERROR);
	} else if (!IMUworking[0] || !IMUworking[1]) {
		sensorIMUStatusTopic.publishConst(MINOR_ERROR);
		if (!IMUworking[0] && retryCycle < NOW()) { //Reinitialisierung bei Ausfall der IMU 1
			PRINTF("Reinit IMU1");
			initIMU(IMU1);
			retryCycle = NOW() + 1*SECONDS;
		}
		if (!IMUworking[1] && retryCycle < NOW()) { //Reinitialisierung bei Ausfall der IMU 2
			PRINTF("Reinit IMU2");
			initIMU(IMU2);
			retryCycle = NOW() + 1*SECONDS;
		}
	} else {
		sensorIMUStatusTopic.publishConst(OK);
	}

	double prescaler[9];
	for (int i = 0; i < 3; i++) {
		prescaler[i] =
				IMUworking[0] ?
						IMUworking[1] ? 0.5 : 1 : 0 / (1.0 + noiseG1[i]);
		prescaler[i + 3] =
				IMUworking[1] ?
						IMUworking[0] ? 0.5 : 1 : 0 / (1.0 + noiseG2[i]);
		prescaler[i + 6] = 1 - prescaler[i] - prescaler[i + 3];
	}
	imu.gyroFiltered[0] = (prescaler[0] * fusionX1) + (prescaler[3] * fusionX2)
			+ (prescaler[6] * (filterXLast[fusionCycle] + fusionLastXramp));
	imu.gyroFiltered[1] = (prescaler[1] * fusionY1) + (prescaler[4] * fusionY2)
			+ (prescaler[7] * (filterYLast[fusionCycle] + fusionLastYramp));
	imu.gyroFiltered[2] = (prescaler[2] * fusionZ1) + (prescaler[5] * fusionZ2)
			+ (prescaler[8] * (filterZLast[fusionCycle] + fusionLastZramp));
	fusionCycle = ++fusionCycle > 4 ? 0 : fusionCycle;
	filterXLast[fusionCycle] = imu.gyroFiltered[0];
	filterYLast[fusionCycle] = imu.gyroFiltered[1];
	filterZLast[fusionCycle] = imu.gyroFiltered[2];
}

double SensorIMU::getScale() {
	return scale;
}

void SensorIMU::run() {

#ifdef DEBUG_SIMULATE_DATA
	/*int i = 0;
	 setPeriodicBeat(0, 10 * MILLISECONDS);
	 while (i < SIM_CNT - 1) {
	 uint64_t ctime = NOW();
	 if (ctime > times_simlist[i + 1] * SECONDS)
	 i++;
	 double delta_t = times_simlist[i + 1] - times_simlist[i];
	 double delta_t_c = ctime / SECONDS - times_simlist[i];
	 double factor = delta_t_c / delta_t;
	 imu.gyroFiltered[2] = imu_value_simlist[i] + (imu_value_simlist[i + 1] - imu_value_simlist[i]) * factor;
	 imu.sysTime = NOW();
	 IMUTopic.publish(imu);
	 suspendUntilNextBeat();
	 }*/

#define CYCLE_MAX 5
#define CYCLE_MIN -5

	//Second Implementation
	double dynamicSimulatedRate = -30.0;
	double rateInfluence = 0.03;
	int lastCycle = 0;
	int cycleVar = 0;
	retryCycle = 0;
	scale = GYRO_FACTOR250;
	scale_cfg = BITS_FS_250DPS;
	setPeriodicBeat(0, 10 * MILLISECONDS);
	while (true)
	{
		CmdData cmd;
		controlBuffer.get(cmd);
		if (cmd.valveState & 0b100)
		{
			if ((cmd.valveState & 0b1) == 0b1)
			{
				if (lastCycle != 1)
				{
					lastCycle = 1;
					cycleVar = 1;
				}
				//PRINTF("Valve 1 triggered\n");

				dynamicSimulatedRate -= rateInfluence * (double)cycleVar / (double)CYCLE_MAX;

				if (cycleVar < CYCLE_MAX)
				cycleVar++;

			}
			else if ((cmd.valveState & 0b10) == 0b10)
			{
				if (lastCycle != 2)
				{
					lastCycle = 2;
					cycleVar = 1;
				}
				//PRINTF("Valve 2 triggered\n");

				dynamicSimulatedRate += rateInfluence * (double)cycleVar / (double)CYCLE_MAX;

				if (cycleVar < CYCLE_MAX)
				cycleVar++;

			}
			else
			if (lastCycle > 0) lastCycle = -lastCycle;
		}
		else
		if (lastCycle > 0) lastCycle = -lastCycle;

		if (lastCycle == -1)
		{
			if (cycleVar > CYCLE_MIN)
			{
				cycleVar--;
				dynamicSimulatedRate -= rateInfluence * (double)(cycleVar - CYCLE_MIN) / (double)(CYCLE_MAX - CYCLE_MIN);
			}
			else
			lastCycle = 0;
		}
		else if (lastCycle == -2)
		{
			if (cycleVar > CYCLE_MIN)
			{
				cycleVar--;
				dynamicSimulatedRate += rateInfluence * (double)(cycleVar - CYCLE_MIN) / (double)(CYCLE_MAX - CYCLE_MIN);
			}
			else
			lastCycle = 0;
		}

		//dynamicSimulatedRate += (((double)rand()/RAND_MAX)-0.5)*0.1337;
		imu.gyroFiltered[2] = dynamicSimulatedRate;
		imu.imu_config = scale_cfg;
		imu.sysTime = NOW();
		IMUTopic.publish(imu);
		suspendUntilNextBeat();
	}

	suspendCallerUntil();
#endif

	sensorIMUStatusTopic.publishConst(NO_INIT);
	suspendCallerUntil(NOW()+ 1*SECONDS);
	int result1, result2;
	scale = GYRO_FACTOR2000;
	scale_cfg = BITS_FS_2000DPS;
	scale_select = 3;
	fusionCycle = 0;
	short counterInit = 0;
	result1 = initIMU(IMU1);
	result2 = initIMU(IMU2);
	while (counterInit <= 10 && result1 != 0 && result2 != 0) {
		result1 = initIMU(IMU1);
		result2 = initIMU(IMU2);
		counterInit++;
		suspendCallerUntil(NOW()+100*MILLISECONDS);
	}

	char debugIMU[50];
	sprintf(debugIMU, "IMU init result1/2: %d, %d\n", result1, result2);
	telemetry.dl_debug(debugIMU);

	for (int i = 0; i < 5 && !storageController.isReady(); i++)
		suspendCallerUntil(NOW()+ 0.2*SECONDS); // wait for storage controller

	//Read the saved calibration
	double tempIMU[24];
	if (storageController.readFromFile("IMUCAL", tempIMU, 24 * sizeof(double),
			0)) {
		for( int i = 0; i < 4; i++) {
			calibG1X[i] = tempIMU[(0+i*6)];
			calibG1Y[i] = tempIMU[(1+i*6)];
			calibG1Z[i] = tempIMU[(2+i*6)];
			calibG2X[i] = tempIMU[(3+i*6)];
			calibG2Y[i] = tempIMU[(4+i*6)];
			calibG2Z[i] = tempIMU[(5+i*6)];
		}

	} else
		calibrate();

	IMUworking[0] = result1 < 0 ? false : true;
	IMUworking[1] = result2 < 0 ? false : true;
	if (result1 < 0 || result2 < 0)
		sensorIMUStatusTopic.publishConst(INIT_FAILED);
	else
		sensorIMUStatusTopic.publishConst(OK);

	//calibrate();
	setPeriodicBeat(0, 10 * MILLISECONDS);
	IMUReadStruct imu1_buf, imu2_buf;
	while (1) {

		Status storageStat;
		storageControllerStatusBuffer.get(storageStat);

		if (storageController.isInitInProgress() || storageStat == SD_DL_PROGRESS) {
			suspendUntilNextBeat();
			continue;
		}

		if (calibrateIMU) {
			calibrateIMU = false;
			calibrate();
		}

		getIMU(IMU1, &imu1_buf);
		getIMU(IMU2, &imu2_buf);

		//int16_t magValues[3];
		//getMagnetometer(IMU1, magValues);

		// PRINTF("MagX: %d, MagY: %d, MagZ: %d\n", magValues[0], magValues[1], magValues[2]);

		memcpy(imu.gyroData1, imu1_buf.gyroData, 6);
		memcpy(imu.gyroData2, imu2_buf.gyroData, 6);
		memcpy(imu.accData1, imu1_buf.accData, 6);
		memcpy(imu.accData2, imu2_buf.accData, 6);
		fusionFilter(imu);

#ifdef DEBUG_IMU_DUMMY_DATA

			imu.gyroData1[0] = (uint16_t)(dummy_gyro1_roll[dummy_cycle] /GYRO_FACTOR);
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

		imu.imu_config = scale_cfg;
		imu.sysTime = NOW();

		/*PRINTF("Offset Gyro1: %f, %f, %f\n", calibG1X*GYRO_FACTOR, calibG1Y*GYRO_FACTOR, calibG1Z*GYRO_FACTOR);
		 PRINTF("Offset Gyro2: %f, %f, %f\n", calibG2X*GYRO_FACTOR, calibG2Y*GYRO_FACTOR, calibG2Z*GYRO_FACTOR);
		 PRINTF("Raw Gyro1: %f, %f, %f\n", imu.gyroData1[0]*GYRO_FACTOR, imu.gyroData1[1]*GYRO_FACTOR, imu.gyroData1[2]*GYRO_FACTOR);
		 PRINTF("Raw Gyro2: %f, %f, %f\n", imu.gyroData2[0]*GYRO_FACTOR, imu.gyroData2[1]*GYRO_FACTOR, imu.gyroData2[2]*GYRO_FACTOR);
		 PRINTF("Raw Acc1: %f, %f, %f\n", imu.accData1[0]*ACC_FACTOR, imu.accData1[1]*ACC_FACTOR, imu.accData1[2]*ACC_FACTOR);
		 PRINTF("Raw Acc2: %f, %f, %f\n", imu.accData2[0]*ACC_FACTOR, imu.accData2[1]*ACC_FACTOR, imu.accData2[2]*ACC_FACTOR);
		 PRINTF("Fusioned IMU: %f, %f, %f\n\n", imu.gyroFiltered[0], imu.gyroFiltered[1], imu.gyroFiltered[2]);*/

		IMUTopic.publish(imu);
		autoscale();

		suspendUntilNextBeat();
	}
}
