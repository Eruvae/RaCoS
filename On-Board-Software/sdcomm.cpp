#include "sdcomm.h"

//SD-Command-List
        // Abbreviation         // Index        // Functionality                        Argument        Response
//---------------------------------------------------------------------------------------------------------------
#define GO_IDLE_STATE           0b01000000      // Software reset.                      None(0)         R1
#define SEND_OP_COND            0b01000001      // Initiate initialization process.     None(0)         R1
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
    retry = 0;
    while(retry < 100) {	//wait for ack.
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
    sendCommand(GO_IDLE_STATE, 0, 0x95);
    sendCommand(SEND_OP_COND, 0, 0xFF);
}

void SDComm::write()
{

}

void SDComm::read()
{

}
