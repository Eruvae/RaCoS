#include "sensorhousekeeping.h"
#include "stdio.h"
#include "telemetry.h"

//#define DEBUG_TEMP_DUMMY_DATA
//#define DEBUG_PRES_DUMMY_DATA

SensorHousekeeping sensorHousekeeping;

// ADC-Defines:
// Adress of ADC; Ground: 0b1001000, VDD: 0b1001001, SDA: 0b1001010, SCL: 0b1001011
#define ADC_ITC_ADDR 0b1001000

#define ADC_CONV_REG 0b00
#define ADC_CONFIG_REG 0b01

#define ADC_CONFIG_PT_MSB 0b01000000
#define ADC_CONFIG_PV_MSB 0b01010000
//B15: 0-no effect, B14-12: 101-Ain1, 110-Ain2, B11-9: 000-FS+-6.144V, B8: 0-continous conversion

#define ADC_CONFIG_PTV_LSB 0b10000011
//B7-5: 100-1600SPS, B4: 0-traditional comparator, B3: 0-Alert/Ready active low, B2: 0-non-latching comparator, B1-0: 11-disable comparator

uint8_t adc_config_pt[] = {ADC_CONFIG_REG, ADC_CONFIG_PT_MSB, ADC_CONFIG_PTV_LSB};
uint8_t adc_config_pv[] = {ADC_CONFIG_REG, ADC_CONFIG_PV_MSB, ADC_CONFIG_PTV_LSB};
uint8_t adc_read_mode[] = {ADC_CONV_REG};

// TODO: Update these values with ROM-Codes
const uint8_t TS_TANK_ROM[8] = {0x28, 0xCD, 0x2A, 0x36, 0x07, 0x00, 0x00, 0xB5};//{0x28, 0x88, 0x37, 0x36, 0x07, 0x00, 0x00, 0xBB};
const uint8_t TS_PDU_ROM[8]  = {0x28, 0x46, 0x33, 0x36, 0x07, 0x00, 0x00, 0x28};//{0x28, 0x3A, 0x7D, 0x35, 0x07, 0x00, 0x00, 0x0E};
const uint8_t TS_VALVERP_ROM[8] = {0x28, 0xEF, 0x3B, 0x36, 0x07, 0x00, 0x00, 0xDC};//{0x28, 0x46, 0x33, 0x36, 0x07, 0x00, 0x00, 0x28};
const uint8_t TS_VALVERM_ROM[8] = {0x28, 0x88, 0x37, 0x36, 0x07, 0x00, 0x00, 0xBB};//{0x28, 0xD9, 0x93, 0x35, 0x07, 0x00, 0x00, 0x06};
const uint8_t TS_VALVEEC_ROM[8] = {0x28, 0x3A, 0x7D, 0x35, 0x07, 0x00, 0x00, 0x0E};//{0x28, 0xCD, 0x2A, 0x36, 0x07, 0x00, 0x00, 0xB5};
const uint8_t TS_NOZ1_ROM[8] = {0x28, 0xC3, 0xA5, 0x36, 0x07, 0x00, 0x00, 0x02};//{0x28, 0xC3, 0xA5, 0x36, 0x07, 0x00, 0x00, 0x02};
const uint8_t TS_NOZ2_ROM[8] = {0x28, 0xD9, 0x93, 0x35, 0x07, 0x00, 0x00, 0x06};//{0x28, 0xEF, 0x3B, 0x36, 0x07, 0x00, 0x00, 0xDC};

#define TS_TH               0b01010101 // High-Temperature alarm, 85 degrees celsius
#define TS_TL               0b11001001 // Low-Temperature alarm, -55 degrees celsius
#define TS_CONFIG           0b01111111 // 9-bit configuration (93.75ms conversion time)



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

// Get ADC value (of tank pressure) and config ADC for valve read
// stores value in presTank, returns positive integer on success
int SensorHousekeeping::getTankPressure(int16_t *presTank)
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

    int16_t tmp = swap16(*presTank) >> 4;
    *presTank = (tmp & 0x0800) ? (tmp | 0xF800) : tmp;

    // config ADC for valves pressure
    if ((result = i2c_bus.write(ADC_ITC_ADDR, adc_config_pv, 3)) < 0)
    {
    	reset_i2c();
    	return result;
    }

    return result;
}

// Get ADC value (of valve pressure) and config ADC for tank read
// stores value in presValves, returns positive integer on success
int SensorHousekeeping::getValvesPressure(int16_t *presValves)
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

    int16_t tmp = swap16(*presValves) >> 4;
    *presValves = (tmp & 0x0800) ? (tmp | 0xF800) : tmp;

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
    	return result;
    }
    return result;
}

void SensorHousekeeping::run()
{
	/*
	uint8_t rom_to_read[8] = {0};

	while(1)
	{
		oneWire.reset();
		oneWire.readROM(rom_to_read);
		for(int i = 0; i < 8; i++)
			PRINTF("%x ", rom_to_read[i]);
		PRINTF("\n");
		suspendCallerUntil(NOW() + 1*SECONDS);
	}
	*/

	/*
	int found_devices = 0;
	uint8_t rom_list[7][8];
	oneWire.reset_search();
	oneWire.target_search(0x28);
	for (int i = 0; i < 7; i++)
	{
		if (!oneWire.search(rom_list[i]))
			break;

		found_devices++;
	}

	while(1)
	{
		char id_str[59];
		char temp[10];
		sprintf(id_str, "Found devices: %d\n", found_devices);
		telemetry.dl_debug(id_str);
		for (int i = 0; i < found_devices; i++)
		{
			sprintf(id_str,"Device %d: %x %x %x %x %x %x %x %x \n", i,
					rom_list[i][0], rom_list[i][1], rom_list[i][2], rom_list[i][3],
					rom_list[i][4], rom_list[i][5], rom_list[i][6], rom_list[i][7]);

			telemetry.dl_debug(id_str);
		}
		suspendCallerUntil(NOW() + 1*SECONDS);
	}
	*/

	housekeepingStatusTopic.publishConst(NO_INIT);
	int result;

	for (int i = 0; i < 10 && (result = configADC()) < 0; i++)
	{
		//PRINTF("Config ADC failed: %d!\n", result);
	}

	PRINTF("Config ADC result: %d!\n", result);
	if (result < 0)
	{
		housekeepingStatusTopic.publishConst(INIT_FAILED);
		suspendCallerUntil();
	}
	else
	{
		housekeepingStatusTopic.publishConst(OK);
	}


	configTempSensor(TS_TANK_ROM);
	configTempSensor(TS_PDU_ROM);
	configTempSensor(TS_VALVERP_ROM);
	configTempSensor(TS_VALVERM_ROM);
	configTempSensor(TS_VALVEEC_ROM);
	configTempSensor(TS_NOZ1_ROM);
	configTempSensor(TS_NOZ2_ROM);
    setPeriodicBeat(0, 100*MILLISECONDS);
    bool presCycle = false;
    int tempCycle = 0;
    while(1)
    {
        if (!presCycle)
        {
            if ((result = getTankPressure(&(hk.presTank))) < 0)
            {
            	//PRINTF("Getting tank pressure failed: %d\n", result);
            }
            //PRINTF("Tank Pressure: %d\n", hk.presTank);
        }
        else
        {
            if ((result = getValvesPressure(&(hk.presValves))) < 0)
            {
            	//PRINTF("Getting valves pressure failed: %d\n", result);
            }

			#ifdef DEBUG_PRES_DUMMY_DATA
            

            hk.presTank = (dummy_pres_tank[dummy_pres_cycle] + PRES_HIGH_OFFSET) / PRES_HIGH_FACTOR;
            hk.presValves = (dummy_pres_valves[dummy_pres_cycle] + PRES_LOW_OFFSET) / PRES_LOW_FACTOR;

            dummy_pres_cycle++;
            if (dummy_pres_cycle > 3)
            	dummy_pres_cycle = 0;

			#endif
            //PRINTF("Valves Pressure: %d\n", hk.presValves);
            
            //Validate Valve-Pressure
            double valvesPressure = hk.presValves * PRES_LOW_FACTOR - PRES_LOW_OFFSET;
            if(valvesPressure > 9.0){
                housekeepingStatusTopic.publishConst(CRITICAL_ERROR);
            }
            
            hk.sysTime = NOW();
            hkTopic.publish(hk);
        }

        if (tempCycle == 0)
        {
            initTemperatureConv();
        }
        else if (tempCycle == 9)
        {
        	hk.tempTank = getTemperatureData(TS_TANK_ROM);
			hk.tempPDU = getTemperatureData(TS_PDU_ROM);
			hk.tempValveRP = getTemperatureData(TS_VALVERP_ROM);
			hk.tempValveRM = getTemperatureData(TS_VALVERM_ROM);
			hk.tempValveEC = getTemperatureData(TS_VALVEEC_ROM);
            hk.tempNoz1 = getTemperatureData(TS_NOZ1_ROM);
            hk.tempNoz2 = getTemperatureData(TS_NOZ2_ROM);


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
