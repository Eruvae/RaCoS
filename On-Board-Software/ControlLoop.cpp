#include "controlloop.h"
#define TRIGGER_THRESHOLD 10

ControlLoop controlLoop;
HAL_GPIO liftoffSignal(GPIO_017);


void ControlLoop::init()
{

}

void ControlLoop::run()
{
	test = true;
	mode = STAND_BY;
	modeTopic.publish(mode);
	testTopic.publish(test);
	liftoffSignal.init(false,1,0);

	setPeriodicBeat(0, 100*MILLISECONDS);

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
			cmd.valveState(0b10);
		}else if(mid > TRIGGER_THRESHOLD){
			actuatorHandler.setValve1(true);
			actuatorHandler.setValve2(false);
			cmd.valveState(0b1);
		}else{
			actuatorHandler.setValve1(true);
			actuatorHandler.setValve2(true);
			cmd.valveState(0b0);
		}
		cmd.vot1(30);
		cmd.vot2(30);
		controlTopic.publish(cmd,true);

		// Do Control stuff
		suspendUntilNextBeat();
	}
}
