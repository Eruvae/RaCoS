#include "healthwatchdog.h"
#include <cstdlib>
#include <new>
#include "actuatorHandler.h"

HealthWatchdog healthWatchdog;

HealthWatchdog::HealthWatchdog() {
	noCriticalErrorOccurred = true;
	emergencyCutoff = false;
	sensorIMUStatus = NO_INIT;
	housekeepingStatus = NO_INIT;
	controlLoopStatus = NO_INIT;
	storageControllerStatus = NO_INIT;
	telemetryStatus = NO_INIT;
	criticalErrorActive = false;
	ignorePendingCritical = false;
}

void HealthWatchdog::run() {
	//modulStateTopic.publishConst(0);
	setPeriodicBeat(0, 500 * MILLISECONDS);
	while (1) {
		uint16_t status = encodeModulStates();
		modulStateTopic.publish(status);
		//PRINTF("Module state published\n");
        checkCriticalErrorPending();
        if(criticalErrorActive)sendCutoff(true);
		suspendUntilNextBeat();
	}
}

bool HealthWatchdog::emergencyCutoffCheck() {
	return emergencyCutoff;
}

void HealthWatchdog::sendCutoff(bool state) {
	CmdData out;
	if (!ignorePendingCritical)
		state = criticalErrorActive ? false : state;

	actuatorHandler.setCutoff(state);
	//out.valveState = state ? AH_EmergencyOpen : AH_EmergencyClose;
	//out.vot = 0;
	//controlTopic.publish(out);
	emergencyCutoff = state;
}
void HealthWatchdog::overrideCutoffProtection(bool override) {
	ignorePendingCritical = override;
}

uint16_t HealthWatchdog::encodeModulStates() {
	sensorIMUStatusBuffer.get(sensorIMUStatus);
	housekeepingStatusBuffer.get(housekeepingStatus);
	controlLoopStatusBuffer.get(controlLoopStatus);
	storageControllerStatusBuffer.get(storageControllerStatus);
	telemetryStatusBuffer.get(telemetryStatus);

	uint16_t states = (sensorIMUStatus & 0b111)
			| ((housekeepingStatus & 0b111) << 3)
			| ((controlLoopStatus & 0b111) << 6)
			| ((storageControllerStatus & 0b111) << 9)
			| ((telemetryStatus & 0b111) << 12);
	return states;
}
void HealthWatchdog::checkCriticalErrorPending() {
	if (sensorIMUStatus == CRITICAL_ERROR
			|| controlLoopStatus == CRITICAL_ERROR) {
		criticalErrorActive = true;
	} else
		criticalErrorActive = false;
}

/*
void HealthWatchdog::setIMUStatus(Status error) {
	sensorIMUStatus = error;
	if (error == CRITICAL_ERROR) {
		noCriticalErrorOccurred = false;
		sendCutoff(true);
	}
}

void HealthWatchdog::setHousekeepingStatus(Status error) {
	housekeepingStatus = error;
	if (error == CRITICAL_ERROR) {
		noCriticalErrorOccurred = false;
		sendCutoff(true);
	}
}

void HealthWatchdog::setControlLoopStatus(Status error) {
	controlLoopStatus = error;
	if (error == CRITICAL_ERROR) {
		noCriticalErrorOccurred = false;
		sendCutoff(true);
	}
}

void HealthWatchdog::setStorageControllerStatus(Status error) {
	storageControllerStatus = error;
	if (error == CRITICAL_ERROR) {
		noCriticalErrorOccurred = false;
		sendCutoff(true);
	}
}

void HealthWatchdog::setTelemetryStatus(Status error) {
	telemetryStatus = error;
	if (error == CRITICAL_ERROR) {
		noCriticalErrorOccurred = false;
		sendCutoff(true);
	}
}
*/
