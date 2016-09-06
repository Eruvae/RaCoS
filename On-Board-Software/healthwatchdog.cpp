#include "healthwatchdog.h"
#include <cstdlib>
#include <new>

HealthWatchdog healthWatchdog;

HealthWatchdog::HealthWatchdog()
{
	pending = false;
	badCount = 0;
	noCriticalErrorOccurred = false;
	emergencyCutoff = false;
}

void HealthWatchdog::run()
{
	modulStateTopic.publishConst(0);
    setPeriodicBeat(0, 500*MILLISECONDS);
    while(1)
    {
        suspendUntilNextBeat();
    }
}

bool HealthWatchdog::emergencyCutoffCheck()
{
    return emergencyCutoff;
}

void HealthWatchdog::sendCutoff(bool state)
{
    CmdData out;
    out.valveState = state? AH_EmergencyOpen:AH_EmergencyClose;
    out.vot1 = 0;
    out.vot2 = 0;
    controlTopic.publish(out);
    emergencyCutoff = state;
}
