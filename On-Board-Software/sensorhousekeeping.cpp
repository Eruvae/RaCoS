#include "sensorhousekeeping.h"

// Adress of ADC; Ground: 0b1001000, VDD: 0b1001001, SDA: 0b1001010, SCL: 0b1001011
#define ADC_ITC_READ 0b10010001
#define ADC_ITC_WRITE 0b10010000

#define ADC_CONV_REG 0b00
#define ADC_CONFIG_REG 0b01

#define ADC_CONFIG_PT_MSB 0b01000001
#define ADC_CONFIG_PV_MSB 0b01010001
//B15: 0-no effect, B14-12: 100-Ain0, 101-Ain1, B11-9: 000-FS+-6.144V, B8: 1-continuous conversion

#define ADC_CONFIG_PTV_LSB 0b10000011

uint8_t adc_config_pt = {ADC_CONFIG_REG, ADC_CONFIG_PT_MSB, ADC_CONFIG_PTV_LSB};
uint8_t adc_config_pv = {ADC_CONFIG_REG, ADC_CONFIG_PV_MSB, ADC_CONFIG_PTV_LSB};
uint8_t adc_read_mode = {ADC_CONV_REG};

SensorHousekeeping::SensorHousekeeping()
{
}

void SensorHousekeeping::getTemperatureData()
{

}

void SensorHousekeeping::getPressureData(uint16_t *presTank, uint16_t *presValve)
{
    i2c_bus.write(ADC_ITC_WRITE, adc_config_pt, 3);
    i2c_bus.write(ADC_ITC_WRITE, adc_read_mode, 1);
    i2c_bus.writeRead(ADC_ITC_READ, 0, 0, (uint8_t*)presTank, 2);

    i2c_bus.write(ADC_ITC_WRITE, adc_config_pv, 3);
    i2c_bus.write(ADC_ITC_WRITE, adc_read_mode, 1);
    i2c_bus.writeRead(ADC_ITC_READ, 0, 0, (uint8_t*)presTank, 2);

}

void SensorHousekeeping::configADC()
{
    i2c_bus.write(ADC_ITC_WRITE, adc_config_pt, 3);
}

void SensorHousekeeping::run()
{

}
