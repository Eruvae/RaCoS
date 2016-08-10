#ifndef HEALTHWATCHDOG_H
#define HEALTHWATCHDOG_H
#define HWTD_ErrorThreshold 5
#define Timeout 25
#define HWTD_ID 5
#define AH_EmergencyOpen 1234
#define AH_EmergencyClose 4567


#include "rodos.h"
#include "topics.h"
#include "structs.h"


class HealthWatchdog : public Thread
{
private:
    bool housekeepingPending;
    bool sensorIMUpending;
    bool actuatorHandlerpending;
    bool controlLoopPending;
    bool pending;
    uint32_t badCount;
    bool noCriticalErrorOccurred;
    bool emergencyCutoff;
public:
    HealthWatchdog();
    void run();
    bool selfcheck();
    bool emergencyCutoffCheck();
    void sendCutoff(bool state);
};

// extern HealthWatchdog healthWatchdog;

#endif // HEALTHWATCHDOG_H
