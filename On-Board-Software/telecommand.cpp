#include "telecommand.h"

Telecommand telecommand;

Telecommand::Telecommand()
{
}

int Telecommand::decodeCommand(dpCommand &comPack)
{
    if (comPack.sync != SYNC_COMM)
        return -1;

    if (Murmur::mm_hash_32((uint8_t*)&comPack, 4) != comPack.sync)
    	return -1;

    //TODO command counter

    switch(comPack.id)
    {
    case DELETE_SD:
        break;
    case SHUTDOWN:
        break;
    case RESTART:
        break;
    case MODE_STANDBY:
        break;
    case MODE_FLIGHT:
        break;
    case MODE_SECURE:
        break;
    case MODE_BD:
        break;
    case TEST_ACTIVATE:
        break;
    case TEST_LEAVE:
        break;
    default:
        return -1;
    }

    return 0;
}

void Telecommand::run()
{
    dpCommand comPack;
    while(1)
    {
        teleUART.suspendUntilDataReady();
        int c = teleUART.getcharNoWait();
        if (c == SYNC_COMM)
        {
            comPack.sync = c;
            teleUART.suspendUntilDataReady();
            c = teleUART.getcharNoWait();
            comPack.id = c;
            teleUART.suspendUntilDataReady();
            c = teleUART.getcharNoWait();
            comPack.counter = c << 8;
            teleUART.suspendUntilDataReady();
            c = teleUART.getcharNoWait();
            comPack.counter &= c;
            teleUART.suspendUntilDataReady();
            c = teleUART.getcharNoWait();
            comPack.check = c << 8;
            teleUART.suspendUntilDataReady();
            c = teleUART.getcharNoWait();
            comPack.check &= c;

            decodeCommand(comPack);
        }
    }
}
