#ifndef CONTROLLOOP_H
#define CONTROLLOOP_H

#define operationalTime 230 //in seconds

#include "rodos.h"
#include "topics.h"
#include "actuatorhandler.h"
#include "buzzer.h"

class ControlLoop: public Thread {
	Mode mode;
	bool test;
	uint64_t lo_last;
	uint64_t soe_last;
	uint64_t sods_last;
	uint64_t startTime;
	uint64_t shutdownTime;
	bool debouncedLO;
	bool debouncedSOE;
	bool debouncedSODS;
	bool dcdcSignal;
	double deltaOmegaStartup[2]; //0: Rate in degree/sec   1: delta t
	double deltaOmegaOperation[2];
	double deltaOmegaShutdown[2];
	short controlState;
	short timelineState;
	long long rescheduleTime;
	bool rescheduleLoop;
	bool calibrated;
	double dynamicThreshold;
	CmdData cmd;
	Status CLhealth;
    bool invertValves;
    double integratorHelper;
public:
	void init();
	void run();
	void calibratePrediction();
	void flightPhaseControl();
};

#endif /* CONTROLLOOP_H */
