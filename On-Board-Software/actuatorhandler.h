#ifndef ACTUATORHANDLER_H
#define ACTUATORHANDLER_H

#include "rodos.h"
#include "topics.h"

class ActuatorHandler : public Initiator
{

	//CmdData cmd;
public:
	volatile uint8_t valveState;
    ActuatorHandler();
    void init();
    void setValve1(bool state);
    void setValve2(bool state);
    void setCutoff(bool state);
};

extern ActuatorHandler actuatorHandler;

#endif // ACTUATORHANDLER_H
