#ifndef ACTUATORHANDLER_H
#define ACTUATORHANDLER_H

#include "rodos.h"

class ActuatorHandler : public Thread
{
public:
    ActuatorHandler();
    void init();
    void setValve1(bool state);
    void setValve2(bool state);
    void setCutoff(bool state);
};

extern ActuatorHandler actuatorHandler;

#endif // ACTUATORHANDLER_H
