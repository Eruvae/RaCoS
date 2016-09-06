#include "sensorhousekeeping.h"

//#define DEBUG_TEMP_DUMMY_DATA
//#define DEBUG_PRES_DUMMY_DATA

SensorHousekeeping sensorHousekeeping;

// ADC-Defines:
// Adress of ADC; Ground: 0b1001000, VDD: 0b1001001, SDA: 0b1001010, SCL: 0b1001011
#define ADC_ITC_ADDR 0b1001000

#define ADC_CONV_REG 0b00
#define ADC_CONFIG_REG 0b01

#define ADC_CONFIG_PT_MSB 0b01000001
#define ADC_CONFIG_PV_MSB 0b01010001
//B15: 0-no effect, B14-12: 100-Ain0, 101-Ain1, B11-9: 000-FS+-6.144V, B8: 1-continuous conversion

#define ADC_CONFIG_PTV_LSB 0b10000011

uint8_t adc_config_pt[] = {ADC_CONFIG_REG, ADC_CONFIG_PT_MSB, ADC_CONFIG_PTV_LSB};
uint8_t adc_config_pv[] = {ADC_CONFIG_REG, ADC_CONFIG_PV_MSB, ADC_CONFIG_PTV_LSB};
uint8_t adc_read_mode[] = {ADC_CONV_REG};

// TODO: Update these values with ROM-Codes
const uint8_t TS_NOZ1_ROM[8] = {0x28, 0x76, 0x6C, 0xD7, 0x04, 0x00, 0x00, 0x1D};
const uint8_t TS_NOZ2_ROM[8] = {0x28, 0x40, 0x3E, 0xAC, 0x04, 0x00, 0x00, 0x68};
const uint8_t TS_NOZ3_ROM[8] = {0x28, 0x43, 0x24, 0x9A, 0x04, 0x00, 0x00, 0xB1};
const uint8_t TS_NOZ4_ROM[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TS_TANK_ROM[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TS_PDU_ROM[8]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define TS_TH               0b01010101 // High-Temperature alarm, 85 degrees celsius
#define TS_TL               0b11001001 // Low-Temperature alarm, -55 degrees celsius
#define TS_CONFIG           0b01111111 // 9-bit configuration (93.75ms conversion time)

#define TEMP_DIVIDER        16.0
#define PRES_HIGH_FACTOR	0.009695752453125 // bar/bit
#define PRES_HIGH_OFFSET	24.841810875 // bar
#define PRES_LOW_FACTOR		0.0008079796875 // bar/bit
#define PRES_LOW_OFFSET		2.1546125 // bar

#ifdef DEBUG_TEMP_DUMMY_DATA

double dummy_temp_noz1[] = {20.3, 18.5, 21.4, 20.0};
double dummy_temp_noz2[] = {14.2, 15.3, 16.8, 15.0};
double dummy_temp_noz3[] = {22.4, 23.1, 24.5, 22.1};
double dummy_temp_noz4[] = {17.5, 18.2, 16.3, 16.9};
double dummy_temp_tank[] = {-13.4, -10.2, -11.2, -12.7};
double dummy_temp_pdu[] = {40.5, 38.2, 39.1, 42.3};

int dummy_temp_cycle = 0;

#endif

#ifdef DEBUG_PRES_DUMMY_DATA

double dummy_pres_valves[] = {3.6, 3.5, 3.7, 3.4};
double dummy_pres_tank[] = {100.2, 101.3, 95.6, 98.6};

int dummy_pres_cycle = 0;

#endif

SensorHousekeeping::SensorHousekeeping()
{
}

void SensorHousekeeping::reset_i2c(){
	i2c_bus.reset();
	suspendCallerUntil(NOW() + 0.5*MILLISECONDS);
	i2c_bus.init();
}

void SensorHousekeeping::configTempSensor(const uint8_t *rom_code)
{
    uint8_t sendBuf[13];
    sendBuf[0] = MATCH_ROM;
    memcpy(&sendBuf[1], rom_code, 8);
    sendBuf[9] = WRITE_SCRATCHPAD;
    sendBuf[10] = TS_TH;
    sendBuf[11] = TS_TL;
    sendBuf[12] = TS_CONFIG;

    oneWire.reset();
    oneWire.write(sendBuf, 13);

}

void SensorHousekeeping::initTemperatureConv()
{
    uint8_t sendBuf[2];
    sendBuf[0] = SKIP_ROM;
    sendBuf[1] = CONVERT_T;

    oneWire.reset();
    oneWire.write(sendBuf, 2);
}

int16_t SensorHousekeeping::getTemperatureData(const uint8_t *rom_code)
{
	uint8_t sendBuf[10];
	uint8_t recBuf[9];
	sendBuf[0] = MATCH_ROM;
	memcpy(&sendBuf[1], rom_code, 8);
	sendBuf[9] = READ_SCRATCHPAD;

	for (int i = 0; i < 3; i++)
	{
		oneWire.reset();
		oneWire.write(sendBuf, 10);

		oneWire.read(recBuf, 9);
		if (oneWire.crc8(recBuf, 8) == recBuf[9])
			break;
	}

    int16_t temp = *(int16_t*)recBuf;
    return temp;

}

int SensorHousekeeping::getTankPressure(uint16_t *presTank)
{
	int result;
    // read tank pressure
	if ((result = i2c_bus.write(ADC_ITC_ADDR, adc_read_mode, 1)) < 0)
	{
		reset_i2c();
		return result;
	}

    if ((result = i2c_bus.read(ADC_ITC_ADDR, (uint8_t*)presTank, 2)) < 0)
    {
    	reset_i2c();
    	return result;
    }

    *presTank = swap16(*presTank);

    // config ADC for valves pressure
    if ((result = i2c_bus.write(ADC_ITC_ADDR, adc_config_pv, 3)) < 0)
    {
    	reset_i2c();
    	return result;
    }

    return result;
}

int SensorHousekeeping::getValvesPressure(uint16_t *presValves)
{   
	int result;
    // read valves pressure
    if ((result = i2c_bus.write(ADC_ITC_ADDR, adc_read_mode, 1)) < 0)
    {
    	reset_i2c();
    	return result;
    }

    if ((result = i2c_bus.read(ADC_ITC_ADDR,(uint8_t*)presValves, 2)) < 0)
    {
    	reset_i2c();
    	return result;
    }

    *presValves = swap16(*presValves);

    // config ADC for tank pressure
    if ((result = i2c_bus.write(ADC_ITC_ADDR, adc_config_pt, 3)) < 0)
    {
    	reset_i2c();
    	return result;
    }

    return true;
}

int SensorHousekeeping::configADC()
{
	int result;
    // config ADC for tank pressure
    if ((result = i2c_bus.write(ADC_ITC_ADDR, adc_config_pt, 3)) < 0)
    {
    	reset_i2c();
    	return false;
    }
    return result;
}

void SensorHousekeeping::run()
{
	/*
	uint8_t sendBuf[20] = {0};
	uint8_t readBuf[20] = {0};
	uint8_t readBuf2[20] = {0};

	uint8_t rom_to_read[8];

	while(1)
	{
		oneWire.reset();
		//oneWire.selectROM(TS_NOZ1_ROM);
		oneWire.skipROM();
		oneWire.writeScratchpad(0x4B, 0x46, 0x3F);
		//configTempSensor(TS_NOZ1_ROM);

		PRINTF("Reset %d\n", oneWire.reset());
		//oneWire.selectROM(TS_NOZ1_ROM);
		oneWire.skipROM();
		oneWire.convertT();

		suspendCallerUntil(NOW() + 1500*MILLISECONDS);

		PRINTF("Reset %d\n", oneWire.reset());
		//oneWire.selectROM(TS_NOZ1_ROM);
		oneWire.skipROM();
		oneWire.readScratchpad(readBuf);

		int16_t temp = *(int16_t*)readBuf;

		PRINTF("%f\n", temp/TEMP_DIVIDER);

		for(int i = 0; i < 9; i++)
			PRINTF("%x ", readBuf[i]);

		PRINTF("\n");

		PRINTF("CRC result: %x\n", oneWire.crc8(readBuf, 8));

		suspendCallerUntil(NOW() + 0.5*SECONDS);
	}
	*/
	int result;

	for (int i = 0; i < 10 && (result = configADC()) < 0; i++)
	{
		PRINTF("Config ADC failed: %d!\n", result);
	}

	configTempSensor(TS_NOZ1_ROM);
	configTempSensor(TS_NOZ2_ROM);
	configTempSensor(TS_NOZ3_ROM);
	configTempSensor(TS_NOZ4_ROM);
	configTempSensor(TS_TANK_ROM);
	configTempSensor(TS_PDU_ROM);
    setPeriodicBeat(0, 100*MILLISECONDS);
    bool presCycle = false;
    int tempCycle = 0;
    while(1)
    {
        if (!presCycle)
        {
            if ((result = getTankPressure(&(hk.presTank))) < 0)
            {
            	PRINTF("Getting tank pressure failed: %d\n", result);
            }
        }
        else
        {
            if ((result = getValvesPressure(&(hk.presValves))) < 0)
            {
            	PRINTF("Getting valves pressure failed: %d\n", result);
            }

			#ifdef DEBUG_PRES_DUMMY_DATA

            hk.presTank = (dummy_pres_tank[dummy_pres_cycle] + PRES_HIGH_OFFSET) / PRES_HIGH_FACTOR;
            hk.presValves = (dummy_pres_valves[dummy_pres_cycle] + PRES_LOW_OFFSET) / PRES_LOW_FACTOR;

            dummy_pres_cycle++;
            if (dummy_pres_cycle > 3)
            	dummy_pres_cycle = 0;

			#endif

            hk.sysTime = NOW();
            hkTopic.publish(hk);
        }

        if (tempCycle == 0)
        {
            initTemperatureConv();
        }
        else if (tempCycle == 9)
        {
            hk.tempNoz1 = getTemperatureData(TS_NOZ1_ROM);
            hk.tempNoz2 = getTemperatureData(TS_NOZ2_ROM);
            hk.tempNoz3 = getTemperatureData(TS_NOZ3_ROM);
            hk.tempNoz4 = getTemperatureData(TS_NOZ4_ROM);
            hk.tempTank = getTemperatureData(TS_TANK_ROM);
            hk.tempPDU = getTemperatureData(TS_PDU_ROM);

			#ifdef DEBUG_TEMP_DUMMY_DATA

            hk.tempNoz1 = dummy_temp_noz1[dummy_temp_cycle] * TEMP_DIVIDER;
			hk.tempNoz2 = dummy_temp_noz2[dummy_temp_cycle] * TEMP_DIVIDER;
			hk.tempNoz3 = dummy_temp_noz3[dummy_temp_cycle] * TEMP_DIVIDER;
			hk.tempNoz4 = dummy_temp_noz4[dummy_temp_cycle] * TEMP_DIVIDER;
			hk.tempTank = dummy_temp_tank[dummy_temp_cycle] * TEMP_DIVIDER;
			hk.tempPDU = dummy_temp_pdu[dummy_temp_cycle] * TEMP_DIVIDER;

			dummy_temp_cycle++;
			if (dummy_temp_cycle > 3)
				dummy_temp_cycle = 0;

			#endif

            hk.sysTime = NOW();
            hkTopic.publish(hk);
        }

        presCycle = !presCycle;
        tempCycle = ++tempCycle < 10 ? tempCycle : 0;
        suspendUntilNextBeat();

    }

}
