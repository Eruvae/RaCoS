#include "actuatorhandler.h"

HAL_GPIO valve1(GPIO_007);
HAL_GPIO valve2(GPIO_006);
HAL_GPIO cutoff(GPIO_037);

ActuatorHandler actuatorHandler;

ActuatorHandler::ActuatorHandler()
{
}

void ActuatorHandler::init()
{
    valve1.init(true, 1, 0);
    valve2.init(true, 1, 0);
    cutoff.init(true, 1, 0);
}

void ActuatorHandler::setValve1(bool state)
{
    valve1.setPins(state);
}

void ActuatorHandler::setValve2(bool state)
{
    valve2.setPins(state);
}

void ActuatorHandler::setCutoff(bool state)
{
    cutoff.setPins(state);
}
