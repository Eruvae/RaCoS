#include "sdcomm.h"

//SD-Command-List
        // Abbreviation         // Index        // Functionality                        Argument        Response
//---------------------------------------------------------------------------------------------------------------
#define GO_IDLE_STATE           0b01000000      // Software reset.                      None(0)         R1
#define SEND_OP_COND            0b01000001      // Initiate initialization process.     None(0)         R1
#define APP_SEND_OP_COND        0b01101001      // Initiate initialization process(SD). Bit 30: HCS     R1
#define SET_BLOCKLEN            0b01010000      // Change R/W block size.               Block length    R1
#define READ_SINGLE_BLOCK       0b01010001      // Read a block.                        Address[31:0]   R1
#define READ_MULTIPLE_BLOCK     0b01010010      // Read multiple blocks.                Address[31:0]   R1
#define WRITE_BLOCK             0b01011000      // Write a block.                       Address[31:0]   R1
#define WRITE_MULTIPLE_BLOCK    0b01011001      // Write multiple blocks.               Address[31:0]   R1

SDComm::SDComm()
{
}

int SDComm::sendCommand(uint8_t command, uint32_t argument, uint8_t crc)
{   
    uint8_t buff[6];
    buff[0] = command;
    *(uint32_t*)&buff[1] = argument;
    /*uint8_t crc = 0xFF; // TODO add crc, not necessarily necessary
    buff[5] = (crc << 1) & 1;*/
    buff[5] = crc;

    spi_comm_running.enter();

    spiHelper.selectSlave(SD);
    spi_bus.write(buff, 6);
    spiHelper.disableSlaves();

    int result = -1;
    uint8_t ff = 0xFF;
    for (int i = 0; i < 100; i++) {	//wait for ack.
        uint8_t rec;
        spi_bus.writeRead(&ff, 1, &rec, 1);
        if (rec != 0xFF) // Respond received, TODO (maybe): R1
        {
            result = 0;
            break;
        }
    }

    spi_comm_running.leave();

    return result;
}

void SDComm::init()
{
    // TODO: Might be necessary to change Baudrate/Clock Speed; maybe sleep
    sendCommand(GO_IDLE_STATE, 0, 0x95);
    sendCommand(SEND_OP_COND, 0, 0xFF);
    //Alt: sendCommand(APP_SEND_OP_COND, 1<<30, 0xFF)
    sendCommand(SET_BLOCKLEN, 512, 0xFF);

}

int SDComm::read_sector_segment(uint32_t sector, uint8_t *buf)
{
    int result = -1;

    spi_comm_running.enter();
    spiHelper.selectSlave(SD);

    for (int i = 0; i < 10; i++)
    {
        if (sendCommand(READ_SINGLE_BLOCK, sector<<9, 0xFF) == 0x00) // <<9 == *512
        {
            result = 0;
            break;
        }
    }

    if (result == -1)
    {
        spiHelper.disableSlaves();
        spi_comm_running.leave();
        return -1;
    }

    result = -1;

    uint8_t ff = 0xFF;
    for (int i = 0; i < 100; i++) {	//wait for ack.
        uint8_t rec;
        spi_bus.writeRead(&ff, 1, &rec, 1);
        if (rec != 0xFF) // Respond received, TODO (maybe): R1
        {
            result = 0;
            break;
        }
    }

    if (result == -1)
    {
        spiHelper.disableSlaves();
        spi_comm_running.leave();
        return -1;
    }

    spi_bus.read(buf, 512);

    spi_bus.write(&ff, 1);                   //false 16-bits crc
    spi_bus.write(&ff, 1);

    spiHelper.disableSlaves();
    spi_comm_running.leave();

    return 0;
}

int SDComm::write_sector_segment(uint32_t sector, uint8_t *buf)
{
    int result = -1;

    spi_comm_running.enter();
    spiHelper.selectSlave(SD);

    for (int i = 0; i < 10; i++)
    {
        if (sendCommand(WRITE_BLOCK, sector<<9, 0xFF) == 0x00) // <<9 == *512
        {
            result = 0;
            break;
        }
    }

    if (result == -1)
    {
        spiHelper.disableSlaves();
        spi_comm_running.leave();
        return -1;
    }

    spi_bus.write(buf, 512);

    return 0;
}
