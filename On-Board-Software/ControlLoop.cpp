#include "ControlLoop.h"

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

	while(1)
	{
		modeBuffer.get(mode);

		// Do Control stuff
	}
}
