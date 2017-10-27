#include "actuatorhandler.h"

#define BIT_V1 0b1
#define BIT_V2 0b10
#define BIT_CO 0b100

#ifdef invert
HAL_GPIO valve1(GPIO_006);
HAL_GPIO valve2(GPIO_007);
#else
HAL_GPIO valve1(GPIO_007);
HAL_GPIO valve2(GPIO_006);
#endif
HAL_GPIO cutoff(GPIO_037);

ActuatorHandler actuatorHandler;

ActuatorHandler::ActuatorHandler() {
	valveState = 0;
}

void ActuatorHandler::init() {
	valve1.init(true, 1, 0);
	valve2.init(true, 1, 0);
	cutoff.init(true, 1, 0);
	valveState = 0;
}

void ActuatorHandler::setValve1(bool state) {

	valve1.setPins(state);
	valveState = state ? (valveState | BIT_V1) : (valveState & ~BIT_V1);
	/*if (testTopic.getNext())
		valve1.setPins(true);
	else
		valve1.setPins(state);*/
	//valveState = !state ? (valveState | BIT_V1) : (valveState & ~BIT_V1);
	 //cmd.valveState = valveState;
	 //controlTopic.publish(cmd);*/
}

void ActuatorHandler::setValve2(bool state) {
	valve2.setPins(state);
	valveState = state ? (valveState | BIT_V2) : (valveState & ~BIT_V2);
	/*if (testTopic.getNext())
		valve2.setPins(true);
	else
		valve2.setPins(state);*/
	//valveState = !state ? (valveState | BIT_V2) : (valveState & ~BIT_V2);
	 //cmd.valveState = valveState;
	 //controlTopic.publish(cmd);
}

void ActuatorHandler::setCutoff(bool state) {
	cutoff.setPins(state);
	valveState = state ? (valveState | BIT_CO) : (valveState & ~BIT_CO);
	/*if (testTopic.getNext())
		cutoff.setPins(true);
	else
		cutoff.setPins(false);*/
	//cutoff.setPins(state);
	//valveState = !state ? (valveState | BIT_CO) : (valveState & ~BIT_CO);
	 //cmd.valveState = valveState;
	 //controlTopic.publish(cmd);
}
