#include "controlloop.h"
#define TRIGGER_THRESHOLD 30

ControlLoop controlLoop;


void ControlLoop::init()
{

}

void ControlLoop::run()
{
	test = true;
	mode = STAND_BY;
	modeTopic.publish(mode);
	testTopic.publish(test);

	setPeriodicBeat(0, 100*MILLISECONDS);

	while(1)
	{
		modeBuffer.get(mode);
		IMUdata imu;
		IMUBuffer.get(imu);
		double mid = (imu.gyroData1[2]+imu.gyroData2[2])*0.00762939453125/2;
		if(mid < -TRIGGER_THRESHOLD){
			actuatorHandler.setValve1(true);
			actuatorHandler.setValve2(false);
		}else if(mid > TRIGGER_THRESHOLD){
			actuatorHandler.setValve1(false);
			actuatorHandler.setValve2(true);
		}else{
			actuatorHandler.setValve1(false);
			actuatorHandler.setValve2(false);
		}

		// Do Control stuff
		suspendUntilNextBeat();
	}
}
