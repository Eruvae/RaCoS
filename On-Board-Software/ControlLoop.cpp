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
		double mid = imu.gyroFiltered[2];
		if(mid < -TRIGGER_THRESHOLD){
			actuatorHandler.setValve1(false);
			actuatorHandler.setValve2(true);
		}else if(mid > TRIGGER_THRESHOLD){
			actuatorHandler.setValve1(true);
			actuatorHandler.setValve2(false);
		}else{
			actuatorHandler.setValve1(true);
			actuatorHandler.setValve2(true);
		}

		// Do Control stuff
		suspendUntilNextBeat();
	}
}
