#include "telecommand.h"

Telecommand telecommand;

Telecommand::Telecommand()
{
}

int Telecommand::decodeCommand(DpCommand &comPack)
{
    if (comPack.sync != SYNC_COMM)
    	return -1;

    //if (Murmur::mm_hash_32((uint8_t*)&comPack, 4) != comPack.sync)
    //	return -1;

    //PRINTF("Hash: %x", Murmur::mm_hash_32((uint8_t*)&comPack, 4));

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
    	modeTopic.publishConst(STAND_BY);
        break;
    case MODE_FLIGHT:
    	modeTopic.publishConst(FLIGHT);
        break;
    case MODE_SECURE:
    	modeTopic.publishConst(SECURE);
        break;
    case MODE_BD:
    	modeTopic.publishConst(BLOW_DOWN);
        break;
    case TEST_ACTIVATE:
    	testTopic.publishConst(true);
        break;
    case TEST_LEAVE:
    	testTopic.publishConst(false);
        break;
    default:
        return -1;
    }

    return 0;
}

void Telecommand::run()
{
    DpCommand comPack;
    while(1)
    {
        teleUART.suspendUntilDataReady();
        int c = teleUART.getcharNoWait();
        //PRINTF("Received: %x", c & 0xFF);
        if ((c & 0xFF) == SYNC_COMM)
        {
        	comPack.sync = c & 0xFF;

        	for (int i = 1; i < sizeof(comPack); i++)
        	{
        		teleUART.suspendUntilDataReady();
				c = teleUART.getcharNoWait();
				((uint8_t*)&comPack)[i] = c & 0xFF;
        	}

        	//PRINTF("I'm in if, yey :)");
            /*comPack.sync = c & 0xFF;
            teleUART.suspendUntilDataReady();
            c = teleUART.getcharNoWait();
            comPack.id = c & 0xFF;
            teleUART.suspendUntilDataReady();
			c = teleUART.getcharNoWait();
			comPack.counter = c & 0xFF;
            teleUART.suspendUntilDataReady();
            c = teleUART.getcharNoWait();
            comPack.counter &= (c << 8) & 0xFF00;
            teleUART.suspendUntilDataReady();
			c = teleUART.getcharNoWait();
			comPack.check = c & 0xFF;
			teleUART.suspendUntilDataReady();
			c = teleUART.getcharNoWait();
			comPack.check &= (c << 8) & 0x0000FF00;
            teleUART.suspendUntilDataReady();
			c = teleUART.getcharNoWait();
			comPack.check &= (c << 16) & 0x00FF0000;
			teleUART.suspendUntilDataReady();
			c = teleUART.getcharNoWait();
			comPack.check &= (c << 24) & 0xFF000000;*/

        	for (int i = 0; i < sizeof(comPack); i++)
			{
				PRINTF("%x", ((uint8_t*)&comPack)[i] & 0xFF);
			}

            decodeCommand(comPack);
        }
    }
}
