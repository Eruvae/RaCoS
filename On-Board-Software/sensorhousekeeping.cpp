#include "sensorhousekeeping.h"

// ADC-Defines:
// Adress of ADC; Ground: 0b1001000, VDD: 0b1001001, SDA: 0b1001010, SCL: 0b1001011
#define ADC_ITC_READ 0b10010001
#define ADC_ITC_WRITE 0b10010000

#define ADC_CONV_REG 0b00
#define ADC_CONFIG_REG 0b01

#define ADC_CONFIG_PT_MSB 0b01000001
#define ADC_CONFIG_PV_MSB 0b01010001
//B15: 0-no effect, B14-12: 100-Ain0, 101-Ain1, B11-9: 000-FS+-6.144V, B8: 1-continuous conversion

#define ADC_CONFIG_PTV_LSB 0b10000011

uint8_t adc_config_pt[] = {ADC_CONFIG_REG, ADC_CONFIG_PT_MSB, ADC_CONFIG_PTV_LSB};
uint8_t adc_config_pv[] = {ADC_CONFIG_REG, ADC_CONFIG_PV_MSB, ADC_CONFIG_PTV_LSB};
uint8_t adc_read_mode[] = {ADC_CONV_REG};

// TempSens-Defines:
#define SEARCH_ROM          0xF0    // Identify ROM-Codes of slave device
#define READ_ROM            0x33    // Only use if only one slave device, read ROM-Code
#define MATCH_ROM           0x55    // followed by 64-Bit ROM-Code to adress device
#define SKIP_ROM            0xCC    // Adress all slaves simultanously (e.g. init conversion), don't use READ_SCRATCHPAD afterwards if multiple slaves
#define ALARM_SEARCH        0xEC    // SEARCH_ROM, but only slaves with set alarm flag answer

#define CONVERT_T           0x44    // Initiates temperature conversion, DS18B20 transmits conversion status to master (not applicable for parasitepowered DS18B20s)
#define READ_SCRATCHPAD     0xBE    // Reads the entire scratchpad including the CRC byte, DS18B20 transmits up to 9 data bytes to master
#define WRITE_SCRATCHPAD    0x4E    // Writes data into scratchpad bytes 2, 3, and 4 (TH, TL, and configuration registers), Master transmits 3 data bytes to DS18B20
#define COPY_SCRATCHPAD     0x48    // Copies TH, TL, and configuration register data from the scratchpad to EEPROM
#define RECALL_E            0xB8    // Recalls TH, TL, and configuration register data from EEPROM to the scratchpad, DS18B20 transmits recall status to master
#define READ_POWER_SUPPLY   0xB4    // Signals DS18B20 power supply mode to the master, DS18B20 transmits supply status to master

// TODO: Update these values with ROM-Codes
const uint64_t TS_NOZ1_ROM = 0x0000000000000000ULL;
const uint64_t TS_NOZ2_ROM = 0x0000000000000000ULL;
const uint64_t TS_NOZ3_ROM = 0x0000000000000000ULL;
const uint64_t TS_NOZ4_ROM = 0x0000000000000000ULL;
const uint64_t TS_TANK_ROM = 0x0000000000000000ULL;

#define TS_TH               0b01010101 // High-Temperature alarm, 85 degrees celsius
#define TS_TL               0b11001001 // Low-Temperature alarm, -55 degrees celsius
#define TS_CONFIG           0b00011111 // 9-bit configuration (93.75ms conversion time)

#define TEMP_DIVIDER        16

SensorHousekeeping::SensorHousekeeping()
{
}

static const uint8_t dscrc_table[] = {
    0,94,188,226,97,63,221,131,194,156,126,32,163,253,31,65,
    157,195,33,127,252,162,64,30,95,1,227,189,62,96,130,220,
    35,125,159,193,66,28,254,160,225,191,93,3,128,222,60,98,
    190,224,2,92,223,129,99,61,124,34,192,158,29,67,161,255,
    70,24,250,164,39,121,155,197,132,218,56,102,229,187,89,7,
    219,133,103,57,186,228,6,88,25,71,165,251,120,38,196,154,
    101,59,217,135,4,90,184,230,167,249,27,69,198,152,122,36,
    248,166,68,26,153,199,37,123,58,100,134,216,91,5,231,185,
    140,210,48,110,237,179,81,15,78,16,242,172,47,113,147,205,
    17,79,173,243,112,46,204,146,211,141,111,49,178,236,14,80,
    175,241,19,77,206,144,114,44,109,51,209,143,12,82,176,238,
    50,108,142,208,83,13,239,177,240,174,76,18,145,207,45,115,
    202,148,118,40,171,245,23,73,8,86,180,234,105,55,213,139,
    87,9,235,181,54,104,138,212,149,203,41,119,244,170,72,22,
    233,183,85,11,136,214,52,106,43,117,151,201,74,20,246,168,
    116,42,200,150,21,75,169,247,182,232,10,84,215,137,107,53
};

uint8_t SensorHousekeeping::crc8(const void *vptr, int len)
{
    uint8_t *addr = (uint8_t*)vptr;
    uint8_t crc = 0;

    while (len--)
        crc = dscrc_table[crc ^ *addr++];

    return crc;
}

void  SensorHousekeeping::configTempSensor(uint64_t *rom_code)
{
    uint8_t sendBuf[13];
    sendBuf[0] = MATCH_ROM;
    memcpy(&sendBuf[1], rom_code, 8);
    sendBuf[9] = WRITE_SCRATCHPAD;
    sendBuf[10] = TS_TH;
    sendBuf[11] = TS_TL;
    sendBuf[12] = TS_CONFIG;

    // Reset, Presence (done by protocoll?)
    oneWire.write((const char*)sendBuf, 13);
    oneWire.suspendUntilWriteFinished();
    for (int i = 0; i < 13; i++) oneWire.getcharNoWait(); // safer(?) clear buffer
}

void SensorHousekeeping::initTemperatureConv()
{
    uint8_t sendBuf[2];
    sendBuf[0] = SKIP_ROM;
    sendBuf[1] = CONVERT_T;

    // Reset, Presence (done by protocoll?)
    oneWire.write((const char*)sendBuf, 2);
    oneWire.suspendUntilWriteFinished();
    //while((oneWire.getcharNoWait()) >=0); // clear buffer
    for (int i = 0; i < 2; i++) oneWire.getcharNoWait(); // safer(?) clear buffer

}

int16_t SensorHousekeeping::getTemperatureData(uint64_t *rom_code)
{
    uint8_t sendBuf[10];
    sendBuf[0] = MATCH_ROM;
    memcpy(&sendBuf[1], rom_code, 8);

    //Init single conversion below, moved to initTemperatureConv()
    /*sendBuf[9] = CONVERT_T;

    // Reset, Presence (done by protocoll?)
    oneWire.write((const char*)sendBuf, 10);
    oneWire.suspendUntilWriteFinished();
    //while((oneWire.getcharNoWait()) >=0); // clear buffer
    for (int i = 0; i < 10; i++) oneWire.getcharNoWait(); // safer(?) clear buffer
    suspendCallerUntil(NOW() + 94*MILLISECONDS); // wait for conversion (TODO: possibly change time)
    // oneWire.suspendUntilDataReady(NOW() + 10*MILLISECONDS); // or this (may work) - probably not
    */

    sendBuf[9] = READ_SCRATCHPAD;

    uint8_t recBuf[9];

    for (int i = 0; i < 3; i++) // 3: Max. read trys
    {
        // Reset, Presence (done by protocoll?)
        oneWire.write((const char*)sendBuf, 10);
        oneWire.suspendUntilWriteFinished();
        for (int i = 0; i < 10; i++) oneWire.getcharNoWait(); // safer(?) clear buffer
        oneWire.suspendUntilDataReady(NOW() + 1*MILLISECONDS); // wait for data

        oneWire.read((char*)recBuf, 9);
        while((oneWire.getcharNoWait()) >=0); // clear buffer

        if (crc8(recBuf, 8) == recBuf[9]) // CRC correct
            break;
    } // TODO: what if 3 reads not succesful? (should not happen)

    int16_t temp = *(int16_t*)recBuf;
    return temp;

}

float SensorHousekeeping::convertTemperature(int16_t temp)
{
    return (float)temp / TEMP_DIVIDER;
}

void SensorHousekeeping::getTankPressure(uint16_t *presTank)
{
    // read tank pressure
    i2c_bus.write(ADC_ITC_WRITE, adc_read_mode, 1);
    i2c_bus.read(ADC_ITC_READ, (uint8_t*)presTank, 2);

    *presTank = swapShort(*presTank);

    // config ADC for valves pressure
    i2c_bus.write(ADC_ITC_WRITE, adc_config_pv, 3);
}

void SensorHousekeeping::getValvesPressure(uint16_t *presValves)
{   
    // read valves pressure
    i2c_bus.write(ADC_ITC_WRITE, adc_read_mode, 1);
    i2c_bus.read(ADC_ITC_READ,(uint8_t*)presValves, 2);

    *presValves = swapShort(*presValves);

    // config ADC for tank pressure
    i2c_bus.write(ADC_ITC_WRITE, adc_config_pt, 3);
}

void SensorHousekeeping::configADC()
{
    // config ADC for tank pressure
    i2c_bus.write(ADC_ITC_WRITE, adc_config_pt, 3);
}

void SensorHousekeeping::run()
{
    configADC();
    setPeriodicBeat(0, 100*MILLISECONDS);
    bool presCycle = false;
    int tempCycle = 0;
    while(1)
    {
        if (!presCycle)
            getTankPressure(&(hk.presTank));
        else
        {
            getValvesPressure(&(hk.presValves));

            hk.sysTime = NOW();
            hkTopic.publish(hk);
        }

        if (tempCycle == 0)
            initTemperatureConv();
        else //if (tempCycle == 1)
        {
            hk.tempNoz1 = getTemperatureData(TS_NOZ1_ROM);
            hk.tempNoz2 = getTemperatureData(TS_NOZ2_ROM);
            hk.tempNoz3 = getTemperatureData(TS_NOZ3_ROM);
            hk.tempNoz4 = getTemperatureData(TS_NOZ4_ROM);
            hk.tempTank = getTemperatureData(TS_TANK_ROM);

            hk.sysTime = NOW();
            hkTopic.publish(hk);
        }

        presCycle = !presCycle;
        tempCycle = ++tempCycle < 2 ? tempCycle : 0;
        suspendUntilNextBeat();
    }
}
