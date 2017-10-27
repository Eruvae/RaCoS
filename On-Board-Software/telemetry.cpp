#include "telemetry.h"
#include "sensorIMU.h"

//#define DEBUG_SEND_NO_TELEMETRY

Telemetry telemetry;

void Telemetry::run()
{
	int sendCycle = 0;
	setPeriodicBeat(0, 50*MILLISECONDS);
	char imuBuf[40];
	char ptBuf[28];
	char calcBuf[25];
	telemetryStatusTopic.publishConst(OK);
	while(1)
	{
		#ifndef DEBUG_SEND_NO_TELEMETRY

		Status stStatus;
		storageControllerStatusBuffer.get(stStatus);
		if (stStatus != SD_DL_PROGRESS)
		{
			encodeIMU(imuBuf);
			teleUART.write(imuBuf, 41);

			if ((sendCycle % 4) == 0)
			{
				encodePresTemp(ptBuf);
				teleUART.write(ptBuf, 28);
			}

			if ((sendCycle % 10) == 0)
			{
				encodeCalc(calcBuf);
				teleUART.write(calcBuf, 25);
			}

			sendCycle++;
		}

		#ifdef ACTIVATE_DEBUG_DL
		DpDebug dbg;
		while (dlDebugFifo.get(dbg))
		{
			teleUART.write((char*)&dbg, sizeof(DpDebug));
		}
		#endif


		#endif

		suspendUntilNextBeat();
	}
}

int Telemetry::encodeIMU(char *buffer)
{
	IMUBuffer.get(imu);
	
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_IMU;
	
	uint32_t time = imu.sysTime / MILLISECONDS;
	buffer[1] = time & 0xFF;
	buffer[2] = (time & 0xFF00) >> 8;
	buffer[3] = (time & 0xFF0000) >> 16;

	*(uint16_t*)&buffer[4] = counter;
	
	memcpy(&buffer[6], &imu, 24);

	*(int16_t*)&buffer[30] = (int16_t)(imu.gyroFiltered[0] / sensorIMU.getScale());
	*(int16_t*)&buffer[32] = (int16_t)(imu.gyroFiltered[1] / sensorIMU.getScale());
	*(int16_t*)&buffer[34] = (int16_t)(imu.gyroFiltered[2] / sensorIMU.getScale());
	buffer[36] = imu.imu_config;

	//PRINTF("Filtered IMU: X:%f Y:%f Z:%f\n", imu.gyroFiltered[0], imu.gyroFiltered[1], imu.gyroFiltered[2]);
	
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
	
	*(uint32_t*)&buffer[37] = generateChecksum(buffer, 37);
	
	counter++;
	
	DownlinkIMU *dataPub = (DownlinkIMU*)buffer;
	dlIMUTopic.publish(*dataPub);

	return 41;
}

int Telemetry::encodePresTemp(char *buffer)
{
	//PressureBuffer.get(pres);
	//TemperatureBuffer.get(temp);
	hkBuffer.get(hk);
	
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_PT;
	
	uint64_t time = hk.sysTime / MILLISECONDS;
	buffer[1] = time & 0xFF;
	buffer[2] = (time & 0xFF00) >> 8;
	buffer[3] = (time & 0xFF0000) >> 16;
	
	*(uint16_t*)&buffer[4] = counter;
	
    memcpy(&buffer[6], &hk, 18);
	
	*(uint32_t*)&buffer[24] = generateChecksum(buffer, 24);
	
	counter++;
	
	DownlinkPresTemp *dataPub = (DownlinkPresTemp*)buffer;
	dlPTTopic.publish(*dataPub);

	return 28;
}

int Telemetry::encodeCalc(char *buffer)
{
	uint16_t modulState = 0;
	controlBuffer.get(calc);
	modeBuffer.get(mode);
	testBuffer.get(testMode);
	modulStateBuffer.get(modulState);
	
	static uint16_t counter = 0;
	
	buffer[0] = SYNC_CALC;
	
	uint64_t time = NOW() / MILLISECONDS;
	buffer[1] = time & 0xFF;
	buffer[2] = (time & 0xFF00) >> 8;
	buffer[3] = (time & 0xFF0000) >> 16;
	
	*(uint16_t*)&buffer[4] = counter;
	
	buffer[6] = (uint8_t)mode | (testMode << 7);

	*(uint16_t*)&buffer[7] = modulState;

	memcpy(&buffer[9], &calc, 12);
	
	
	*(uint32_t*)&buffer[21] = generateChecksum(buffer, 21);
	
	counter++;
	
	DownlinkCalc *dataPub = (DownlinkCalc*)buffer;
	dlCalcTopic.publish(*dataPub);

	return 25;
	
}

uint32_t Telemetry::generateChecksum(char *buffer, int size)
{
    return Murmur::mm_hash_32((uint8_t*)buffer, size);
}
