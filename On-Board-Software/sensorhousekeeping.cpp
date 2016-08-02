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
#define CONVERT_T           0x44    // Initiates temperature conversion, DS18B20 transmits conversion status to master (not applicable for parasitepowered DS18B20s)
#define READ_SCRATCHPAD     0xBE    // Reads the entire scratchpad including the CRC byte, DS18B20 transmits up to 9 data bytes to master
#define WRITE_SCRATCHPAD    0x4E    // Writes data into scratchpad bytes 2, 3, and 4 (TH, TL, and configuration registers), Master transmits 3 data bytes to DS18B20
#define COPY_SCRATCHPAD     0x48    // Copies TH, TL, and configuration register data from the scratchpad to EEPROM
#define RECALL_E            0xB8    // Recalls TH, TL, and configuration register data from EEPROM to the scratchpad, DS18B20 transmits recall status to master
#define READ_POWER_SUPPLY   0xB4    // Signals DS18B20 power supply mode to the master, DS18B20 transmits supply status to master

SensorHousekeeping::SensorHousekeeping()
{
}

void SensorHousekeeping::getTemperatureData()
{

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
    while(1)
    {
        if (!presCycle)
            getTankPressure(&(hk.presTank));
        else
        {
            getValvesPressure(&(hk.presValves));
            hkTopic.publish(hk);
        }
        suspendUntilNextBeat();
    }
}
