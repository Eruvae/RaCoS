#include "controlloop.h"
#include "actuatorhandler.h"

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

	setPeriodicBeat(0, 5000*MILLISECONDS);
	bool toggle = false;
	while(1)
	{
		modeBuffer.get(mode);

		actuatorHandler.setValve1(toggle);
		actuatorHandler.setCutoff(toggle);
		actuatorHandler.setValve2(toggle);

		toggle = !toggle;

		// Do Control stuff
		suspendUntilNextBeat();
	}
}
