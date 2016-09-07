#include "controlloop.h"
#include "healthWatchdog.h"
#define TRIGGER_THRESHOLD 10

ControlLoop controlLoop;
HAL_GPIO loSignal(GPIO_017);
HAL_GPIO soeSignal(GPIO_016);
HAL_GPIO sodsSignal(GPIO_002);
HAL_GPIO statusLED(GPIO_036);


void ControlLoop::init()
{
	loSignal.init(false, 1, 0);
	soeSignal.init(false, 1, 0);
	sodsSignal.init(false, 1, 0);
	statusLED.init(true, 1, 0);
}

void ControlLoop::run()
{
	test = true;
	mode = STAND_BY;
	modeTopic.publish(mode);
	testTopic.publish(test);

	setPeriodicBeat(0, 100*MILLISECONDS);

	healthWatchdog.setControlLoopStatus(OK);

	lo_last = loSignal.readPins();
	soe_last = soeSignal.readPins();
	sods_last = sodsSignal.readPins();

	PRINTF("LO: %d, SOE: %d, SODS: %d\n", lo_last, soe_last, sods_last);

	statusLED.setPins(lo_last);

	while(1)
	{


		modeBuffer.get(mode);
		IMUdata imu;
		IMUBuffer.get(imu);
		CmdData cmd;
		double mid = imu.gyroFiltered[2];
		if(mid < -TRIGGER_THRESHOLD){
			actuatorHandler.setValve1(false);
			actuatorHandler.setValve2(true);
			cmd.valveState = 0b10;
		}else if(mid > TRIGGER_THRESHOLD){
			actuatorHandler.setValve1(true);
			actuatorHandler.setValve2(false);
			cmd.valveState = 0b1;
		}else{
			actuatorHandler.setValve1(true);
			actuatorHandler.setValve2(true);
			cmd.valveState = 0b0;
		}
		cmd.vot1 = 30;
		cmd.vot2 = 30;
		controlTopic.publish(cmd);

		bool lo = loSignal.readPins();
		bool soe = soeSignal.readPins();
		bool sods = sodsSignal.readPins();

		if (lo != lo_last)
		{
			PRINTF("LO: %d\n", lo);
			lo_last = lo;
		}
		if (soe != soe_last)
		{
			PRINTF("SOE: %d\n", soe);
			soe_last = soe;
		}
		if (sods != sods_last)
		{
			PRINTF("SODS: %d\n", sods);
			sods_last = sods;
		}

		statusLED.setPins(lo_last);

		suspendUntilNextBeat();
	}
}
