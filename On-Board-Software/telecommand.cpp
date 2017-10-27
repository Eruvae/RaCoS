#include "telecommand.h"
#include "storagecontroller.h"
#include "sensorIMU.h"

Telecommand telecommand;

Telecommand::Telecommand()
{
}

int Telecommand::decodeCommand(DpCommand &comPack)
{
    if (comPack.sync != SYNC_COMM)
    	return -1;

    if (Murmur::mm_hash_32((uint8_t*)&comPack, sizeof(comPack) - sizeof(comPack.check)) != comPack.check)
    	return -1;

    //PRINTF("Hash: %x", Murmur::mm_hash_32((uint8_t*)&comPack, 4));

    //TODO command counter

    switch(comPack.id)
    {
    case FORMAT_SD:
    	storageController.format();
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
    case DOWNLINK_SD:
    	storageController.downlinkData();
    	break;
    case REQUEST_PCK:
    	//uint32_t pckCnt;
    	//sdDLPackageBuffer.get(pckCnt);
    	//if (comPack.argument > pckCnt)
    	sdDLPackageTopic.publishConst(comPack.argument);
    	//PRINTF("Package %d requested\n", comPack.argument);
    	break;
    case RESET_SD:
    	storageController.reset();
    	break;
    case CALIBRATE_IMU:
    	sensorIMU.setIMUCalibration();
    	break;
    case MODE_SPINUP:
    	if (comPack.argument == 1)
    	{
    		spinupDirectionTopic.publishConst(true);
    		modeTopic.publishConst(TEST_SPINUP);
    	}
    	else if (comPack.argument == -1)
    	{
    		spinupDirectionTopic.publishConst(false);
    		modeTopic.publishConst(TEST_SPINUP);
    	}
    	break;
    case RESET_CONTROL:
    	resetControllerTopic.publishConst(true);
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

            decodeCommand(comPack);
        }
    }
}
