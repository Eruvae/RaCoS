#include "healthwatchdog.h"

healthWatchdog::healthWatchdog()
{
}

void healthWatchdog::run()
{
    setPeriodicBeat(0, 500*MILLISECONDS);
    while(1){
        badCount += housekeepingPending? 1:0;
        badCount += sensorIMUpending? 1:0;
        badCount += actuatorHandlerpending? 1:0;
        badCount += controlLoopPending? 1:0;
        if(badCount>HWTD_ErrorThreshold){
            status stat;
            stat.errorcode = Timeout;
            stat.id = HWTD_ID;
            stat.systemBad = true;
            healthTopic.publish(stat);
        }
    }
}

bool healthWatchdog::selfcheck()
{
    bool t1 = housekeepingPending;
    bool t2 = sensorIMUpending;
    bool t3 = actuatorHandlerpending;
    bool t4 = controlLoopPending;
    bool success = false;

    housekeepingPending = true;
    sensorIMUpending = true;
    actuatorHandlerpending = true;
    controlLoopPending = true;

    if(housekeepingPending && sensorIMUpending && actuatorHandlerpending && controlLoopPending)success = true;

    housekeepingPending = false;
    sensorIMUpending = false;
    actuatorHandlerpending = false;
    controlLoopPending = false;

    if(!housekeepingPending && !sensorIMUpending && !actuatorHandlerpending && !controlLoopPending)success = true;
    else success = false;

    housekeepingPending = t1;
    sensorIMUpending = t2;
    actuatorHandlerpending = t3;
    controlLoopPending = t4;
    return success;
}

bool healthWatchdog::emergencyCutoffCheck()
{
    return emergencyCutoff;
}

void healthWatchdog::sendCutoff(bool state)
{
    cmdData out;
    out.valveState = state? AH_EmergencyOpen:AH_EmergencyClose;
    out.vot1 = 0;
    out.vot2 = 0;
    controlTopic.publish(out);
    emergencyCutoff = state;
}
