#include "comminterfaces.h"
#include "userconfs/racosconfig.h"

HAL_SPI spi_bus(SPI_IDX2, GPIO_029, GPIO_030, GPIO_031); // SCK: PB13, MISO: PB14, MOSI: PB15
HAL_I2C i2c_bus(I2C_IDX1, GPIO_022, GPIO_023); // SCL: PB6, SDA: PB7
#ifdef DEBUG_UART_BLUETOOTH
HAL_UART teleUART(UART_IDX4, GPIO_000, GPIO_001); // TX: PA0, RX: PA1
//#define UART_DEBUG	UART_IDX1, GPIO_009, GPIO_010, GPIO_012, GPIO_011
#else
HAL_UART teleUART(UART_IDX1, GPIO_009, GPIO_010, GPIO_012, GPIO_011); // TX: PA9, RX: PA10
//#define UART_DEBUG	UART_IDX4, GPIO_000, GPIO_001
//HAL_UART teleUART(UART_IDX1);
//HAL_UART teleUART(UART_IDX1, GPIO_022, GPIO_023, GPIO_012, GPIO_011); // TX: PA9, RX: PA10
#endif
HAL_UART onewire_uart(UART_IDX3, GPIO_042, GPIO_043, GPIO_030, GPIO_059); // TX: PC10, RX: PC11
OneWire oneWire(&onewire_uart);

// UART_DEBUG: UART_IDX5,  // TX: PC12, RX: PD2

CommInterfaces commInit;

CommInterfaces::CommInterfaces()
{
}

void CommInterfaces::init()
{
    spi_bus.init(SPI_BAUDRATE); // TODO (optional): Buadrade angeben, Mode per config; TODO: error handling
    spi_bus.config(SPI_PARAMETER_MODE, 0);
    i2c_bus.init();
    teleUART.init();
    oneWire.init();
}
