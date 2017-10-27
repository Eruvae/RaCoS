#include "controlloop.h"
#include "healthWatchdog.h"
#include "math.h"
#include <stdio.h>
#include "telemetry.h"
#define TRIGGER_THRESHOLD 5
#define CALIBRATE_PREDICT_THRESHOLD 0.05 //Threshold for the Derivative of the angular rate to detect module borders
#define SUBSAMPLING_PREDICT_CALIBRATE 15 //in Milliseconds
#define CONTROLLOOPTIME 100 //Milliseconds
#define SPINUPRATE 30.0 //Degree per Second
#define ADDITIONAL_MARGIN 30.0
#define SPINUP_MARGIN 30.0

#define DELTA_OMEGA_DIVIDOR			8000.0

ControlLoop controlLoop;
HAL_GPIO loSignal(GPIO_017);
HAL_GPIO soeSignal(GPIO_016);
HAL_GPIO sodsSignal(GPIO_002);
HAL_GPIO dcdcConverter(GPIO_021);

void ControlLoop::init() {
	loSignal.init(false, 1, 0);
	soeSignal.init(false, 1, 0);
	sodsSignal.init(false, 1, 0);
	sodsSignal.config(GPIO_CFG_PULLDOWN_ENABLE, true);
	dcdcConverter.init(true, 1, 0);
}

void ControlLoop::calibratePrediction() {
	//Startup-Phase
	double deltaOmegaOld = 0;
	double deltaOmega = 0;
	double deltaDeltaOmega = 0;
	double deltaDeltaOmegaOld = 0;
	uint64_t period = NOW();
	IMUdata imu;
	IMUBuffer.get(imu);
	bool spinRight = imu.gyroFiltered[2] < 0 ? true : false;
	if (spinRight) {
		actuatorHandler.setValve1(true);
		actuatorHandler.setValve2(false);
		healthWatchdog.sendCutoff(true);
	} else {
		actuatorHandler.setValve1(false);
		actuatorHandler.setValve2(true);
		healthWatchdog.sendCutoff(true);
	}
	cmd.valveState = actuatorHandler.valveState;
	controlTopic.publish(cmd);
	IMUBuffer.get(imu);
    deltaOmega = imu.gyroFiltered[2];
    suspendCallerUntil(NOW()+100*MILLISECONDS);
    IMUBuffer.get(imu);
	deltaOmegaStartup[1] = (NOW()- period)/MILLISECONDS;
	deltaOmegaStartup[0] = fabs(imu.gyroFiltered[2] - deltaOmega);

	//Operational Phase
	period = NOW();
	IMUBuffer.get(imu);
	deltaOmega = imu.gyroFiltered[2];
	suspendCallerUntil(NOW()+1*SECONDS);
	IMUBuffer.get(imu);

	PRINTF("OP1: %f\n, OP2: %f\n", deltaOmega, imu.gyroFiltered[2]);

	deltaOmegaOperation[0] = fabs(imu.gyroFiltered[2] - deltaOmega);
	deltaOmegaOperation[1] = (NOW()- period)/MILLISECONDS;
	deltaOmegaOperation[0] /= 1000;
	deltaOmegaOperation[1] /= 1000;

	//Shutdown-Phase
	period = NOW();
	actuatorHandler.setValve1(false);
	actuatorHandler.setValve2(false);
	healthWatchdog.sendCutoff(false);
	cmd.valveState = actuatorHandler.valveState;
	controlTopic.publish(cmd);

	IMUBuffer.get(imu);
    deltaOmega = imu.gyroFiltered[2];
    suspendCallerUntil(NOW()+100*MILLISECONDS);
    IMUBuffer.get(imu);
	deltaOmegaShutdown[1] = (NOW()- period)/MILLISECONDS;
	deltaOmegaShutdown[0] = fabs(imu.gyroFiltered[2] - deltaOmega);
	calibrated = true;

	deltaOmegaStartup[0] = deltaOmegaStartup[0] < 0.01 ? 0.01 : deltaOmegaStartup[0] > 8.0 ? 8.0 : deltaOmegaStartup[0];
	deltaOmegaOperation[0] = deltaOmegaOperation[0] < 0.0001 ? 0.0001 : deltaOmegaOperation[0] > 0.08 ? 0.08 : deltaOmegaOperation[0];
	deltaOmegaShutdown[0] = deltaOmegaShutdown[0] < 0.01 ? 0.01 : deltaOmegaShutdown[0] > 8.0 ? 8.0 : deltaOmegaShutdown[0];

	char debugStr[50];
	sprintf(debugStr, "deltaOmegaStartup: %f, %f\n", deltaOmegaStartup[0],
			deltaOmegaStartup[1]);
	telemetry.dl_debug(debugStr);
	sprintf(debugStr, "deltaOmegaOperation: %f, %f\n", deltaOmegaOperation[0],
			deltaOmegaOperation[1]);
	telemetry.dl_debug(debugStr);
	sprintf(debugStr, "deltaOmegaShutdown: %f, %f\n", deltaOmegaShutdown[0],
			deltaOmegaShutdown[1]);
	telemetry.dl_debug(debugStr);
}

void ControlLoop::flightPhaseControl() {
	switch (timelineState) {
	case 0: //On the Launchpad, no Launch yet
		if (debouncedLO) {
			startTime = NOW();
			shutdownTime = NOW() + operationalTime * SECONDS;
			timelineState++;
			mode = STAND_BY;
		}
		break;
	case 1: //Rocket ascending
		if(debouncedSOE) {
			timelineState++;
			mode = CLhealth == CRITICAL_ERROR? SECURE : FLIGHT;
		}
		if(NOW()>shutdownTime) {
			timelineState = 3;
			mode = BLOW_DOWN;
		}
		break;
	case 2: //Start of operational Phase
		if (CLhealth == CRITICAL_ERROR) {
			mode = SECURE;
		}
		if(NOW()>shutdownTime) {
			timelineState++;
			mode = BLOW_DOWN;
		}
		break;
	case 3: //Timeout for Operation; Enter Blow-Down-Mode
		break;
	}
	modeTopic.publish(mode);
}

void ControlLoop::run() {
    invertValves = false;
	calibrated = false;
    test = false;
    debouncedLO = false;
    debouncedSOE = false;
    debouncedSODS = false;
    dcdcSignal = false;
	controlState = 1;
	timelineState = 0;
	mode = STAND_BY;
	modeTopic.publish(mode);
	testTopic.publish(test);
    bool resetPending = false;
    resetControllerTopic.publish(resetPending);

	setPeriodicBeat(0, CONTROLLOOPTIME * MILLISECONDS);

	controlLoopStatusTopic.publishConst(OK);
	CLhealth = OK;

	//First States
	lo_last = loSignal.readPins() ? NOW():0;
	soe_last = soeSignal.readPins() ? NOW():0;
	sods_last = sodsSignal.readPins() ? NOW():0;
	startTime = 0;
	dynamicThreshold = 0;
	integratorHelper = 0.0;


	buzzer.playTone(3000,500);

	while (1) {

		modeBuffer.get(mode);
		IMUdata imu;
        IMUBuffer.get(imu);
        HkData hkdata;
        hkBuffer.get(hkdata);
        resetControllerBuffer.get(resetPending);

		testBuffer.get(test);

		//Timeline here
		//if (!test)
		flightPhaseControl();

		double mid = imu.gyroFiltered[2];

		//Debouncing for Input-Signals
		if (debouncedLO) {
			lo_last = (loSignal.readPins()) ? lo_last > 0 ? lo_last : NOW(): 0;
			if (lo_last > 0 && loSignal.readPins()) {
				if (NOW() - lo_last > 1 * SECONDS)
				{
					debouncedLO = false;
					lo_last = 0;
				}
			} else {
				lo_last = 0;
			}
		} else {
			lo_last = (!loSignal.readPins()) ? lo_last > 0 ? lo_last : NOW(): 0;
			if (lo_last > 0 && !loSignal.readPins()) {
				if (NOW() - lo_last > 1 * SECONDS)
				{
					debouncedLO = true;
					lo_last = 0;
				}
			} else {
				lo_last=0;
			}
		}
		if (debouncedSOE) {
			soe_last = (soeSignal.readPins()) ? soe_last > 0 ? soe_last : NOW(): 0;
			if (soe_last > 0 && soeSignal.readPins()) {
				if (NOW() - soe_last > 1 * SECONDS)
				{
					debouncedSOE = false;
					soe_last = 0;
				}
			} else {
				soe_last=0;
			}
		} else {
			soe_last = (!soeSignal.readPins()) ? soe_last > 0 ? soe_last : NOW(): 0;
			if (soe_last > 0 && !soeSignal.readPins()) {
				if (NOW() - soe_last > 1 * SECONDS)
				{
					debouncedSOE = true;
					soe_last = 0;
				}
			} else {
				soe_last=0;
			}
		}
		if (debouncedSODS) {
			sods_last =
					(!sodsSignal.readPins()) ? sods_last > 0 ? sods_last : NOW(): 0;
			if (sods_last > 0 && !sodsSignal.readPins()) {
				if (NOW() - sods_last > 1 * SECONDS)
				{
					debouncedSODS = false;
					sods_last = 0;
				}
			} else {
				sods_last=0;
			}
		} else {
			sods_last = sodsSignal.readPins() ? sods_last > 0 ? sods_last : NOW(): 0;
			if (sods_last > 0 && sodsSignal.readPins()) {
				if (NOW() - sods_last > 1 * SECONDS)
				{
					debouncedSODS = true;
					sods_last = 0;
				}
			} else {
				sods_last=0;
			}
		}
        
        //Reset Controller
        if(resetPending){
            dynamicThreshold = 0;
            integratorHelper = 0.0;
            CLhealth = OK;
            controlLoopStatusTopic.publishConst(OK);
            resetControllerTopic.publishConst(false);
        }


        double tankpress = hkdata.presTank * PRES_HIGH_FACTOR - PRES_HIGH_OFFSET;
        //char dbgStr[50];
        //sprintf(dbgStr, "PresTank: %f\n", tankpress);
        //telemetry.dl_debug(dbgStr);
        if(test/* || mode == FLIGHT*/){
            if(hkdata.presTank != 0 && tankpress<22.0){
                controlLoopStatusTopic.publishConst(CRITICAL_ERROR);
                CLhealth = CRITICAL_ERROR;
                mode = SECURE;
                modeTopic.publish(mode);
            }
        }
        if(mode == BLOW_DOWN) healthWatchdog.overrideCutoffProtection(true);
        else healthWatchdog.overrideCutoffProtection(false);
        
		//Cases of Controller
		switch (mode) {
		case STAND_BY:
			actuatorHandler.setValve1(false);
			actuatorHandler.setValve2(false);
			healthWatchdog.sendCutoff(false);
			dcdcSignal = false;
			dcdcConverter.setPins(dcdcSignal);
			cmd.valveState = 0;
			break;
		case FLIGHT:
            //Error detection
			dynamicThreshold =
					dynamicThreshold == 0 ?
							fabs(imu.gyroFiltered[2]) + SPINUP_MARGIN : dynamicThreshold;
			if (fabs(imu.gyroFiltered[2]) > dynamicThreshold) {
                if (CLhealth == MINOR_ERROR) {
                    controlLoopStatusTopic.publishConst(CRITICAL_ERROR);
                    CLhealth = CRITICAL_ERROR;
                    mode = SECURE;
                    modeTopic.publish(mode);
                } else {
                    invertValves = true;
                    controlLoopStatusTopic.publishConst(MINOR_ERROR);
                    CLhealth = MINOR_ERROR;
                    dynamicThreshold += ADDITIONAL_MARGIN;
                }
				
			} else if (fabs(imu.gyroFiltered[2]) < dynamicThreshold - SPINUP_MARGIN)
				dynamicThreshold = fabs(imu.gyroFiltered[2]) + SPINUP_MARGIN;
                
			//dcdcConverter.setPins(test ? false : true);
			dcdcSignal = true;
			dcdcConverter.setPins(dcdcSignal);
			if (!calibrated)
				calibratePrediction();
			long calcTimes;
			long calcTime;
			switch (controlState) {
			case 0:
                    if (!invertValves) {
                        actuatorHandler.setValve1(true);
                        actuatorHandler.setValve2(false);
                    } else {
                        actuatorHandler.setValve1(false);
                        actuatorHandler.setValve2(true);
                    }
				
				healthWatchdog.sendCutoff(true);
				//Calculation of remaining spindown time
				calcTimes = -mid - deltaOmegaStartup[0] - deltaOmegaShutdown[0];
				calcTimes /= deltaOmegaOperation[0];
				calcTime = deltaOmegaShutdown[1]
						+ (calcTimes * deltaOmegaOperation[1]);
				cmd.vot = calcTime;
				//Prediction of shutdown
				if (calcTime < CONTROLLOOPTIME + deltaOmegaShutdown[1]) {
					rescheduleLoop = true;
					rescheduleTime = NOW()+ calcTime - (deltaOmegaShutdown[1]*MILLISECONDS);
					controlState = 1;
				}
				break;
			case 1:
				if (mid
						< -(deltaOmegaStartup[0] + deltaOmegaShutdown[0] + 1)) {
					controlState = 0;
					healthWatchdog.sendCutoff(true);
					if (!invertValves) {
						actuatorHandler.setValve1(true);
						actuatorHandler.setValve2(false);
					} else {
						actuatorHandler.setValve1(false);
						actuatorHandler.setValve2(true);
					}
					//Calculation of remaining spindown time
					calcTimes = -mid - deltaOmegaStartup[0]
							- deltaOmegaShutdown[0];
					calcTimes /= deltaOmegaOperation[0];
					calcTime = deltaOmegaStartup[1] + deltaOmegaShutdown[1]
							+ (calcTimes * deltaOmegaOperation[1]);
					cmd.vot = calcTime;
					//Extraordinary shutdown prediction in case of spindown time < controlloop-time + time for shutdown
					if (calcTime < CONTROLLOOPTIME + deltaOmegaShutdown[1]) {

						//healthWatchdog.sendCutoff(true);
						rescheduleLoop = true;
						rescheduleTime = NOW()+ calcTime - (deltaOmegaShutdown[1]*MILLISECONDS);
						controlState = 1;
					}
				} else if (mid
						> (deltaOmegaStartup[0] + deltaOmegaShutdown[0] + 1)) {
					controlState = 2;
					healthWatchdog.sendCutoff(true);
					if (!invertValves) {
						actuatorHandler.setValve1(false);
						actuatorHandler.setValve2(true);
					} else {
						actuatorHandler.setValve1(true);
						actuatorHandler.setValve2(false);
					}
					//Calculation of remaining spindown time
					calcTimes = mid - deltaOmegaStartup[0]
							- deltaOmegaShutdown[0];
					calcTimes /= deltaOmegaOperation[0];
					int calcTime = deltaOmegaStartup[1] + deltaOmegaShutdown[1]
							+ (calcTimes * deltaOmegaOperation[1]);
					cmd.vot = calcTime;
					//Extraordinary shutdown prediction in case of spindown time < controlloop-time + time for shutdown
					if (calcTime < CONTROLLOOPTIME + deltaOmegaShutdown[1]) {

						//healthWatchdog.sendCutoff(true);
						rescheduleLoop = true;
						rescheduleTime = NOW()+ calcTime - (deltaOmegaShutdown[1]*MILLISECONDS);
						controlState = 1;
					}
				} else {
					actuatorHandler.setValve1(false);
					actuatorHandler.setValve2(false);
					healthWatchdog.sendCutoff(false);
					cmd.vot = 0;
				}
				break;
			case 2:
                    if (!invertValves) {
                        actuatorHandler.setValve1(false);
                        actuatorHandler.setValve2(true);
                    } else {
                        actuatorHandler.setValve1(true);
                        actuatorHandler.setValve2(false);
                    }
				healthWatchdog.sendCutoff(true);
				//Calculation of remaining spindown time
				calcTimes = mid - deltaOmegaStartup[0] - deltaOmegaShutdown[0];
				calcTimes /= deltaOmegaOperation[0];
				calcTime = deltaOmegaShutdown[1]
						+ (calcTimes * deltaOmegaOperation[1]);
				cmd.vot = calcTime;
				//Prediction of shutdown
				if (calcTime < CONTROLLOOPTIME + deltaOmegaShutdown[1]) {
					rescheduleLoop = true;
					rescheduleTime = NOW()+ calcTime - (deltaOmegaShutdown[1]*MILLISECONDS);
					controlState = 1;
				}
				break;
			}
			break;
		case SECURE:
			actuatorHandler.setValve1(false);
			actuatorHandler.setValve2(false);
			healthWatchdog.sendCutoff(false);
			dcdcSignal = false;
			dcdcConverter.setPins(dcdcSignal);
			cmd.valveState = 0b000;
			break;
		case BLOW_DOWN:
			if (tankpress < 2.0)
			{
				mode = SECURE;
				modeTopic.publish(mode);
			}
			healthWatchdog.sendCutoff(true);
			actuatorHandler.setValve1(true);
			actuatorHandler.setValve2(true);
			//dcdcConverter.setPins(test ? false : true);
			dcdcSignal = true;
			dcdcConverter.setPins(dcdcSignal);
			cmd.valveState = 0b111;
			break;
        case TEST_SPINUP:
        	//integratorHelper += fabs(mid)/(1000.0/CONTROLLOOPTIME);
        	bool dir;
        	spinupDirectionBuffer.get(dir);
            if (fabs(mid) < 30.0) {
            	healthWatchdog.sendCutoff(true);
            	if (dir)
            	{
					actuatorHandler.setValve1(true);
					actuatorHandler.setValve2(false);
            	}
            	else
            	{
            		actuatorHandler.setValve1(false);
					actuatorHandler.setValve2(true);
            	}
            	dcdcSignal = true;
                dcdcConverter.setPins(dcdcSignal);
            } else {
                mode = FLIGHT;
                modeTopic.publish(mode);
            }
		}

		cmd.signals = debouncedLO ? 0b1 : 0;
		cmd.signals |= debouncedSOE ? 0b10 : 0;
		cmd.signals |= debouncedSODS ? 0b100 : 0;
		cmd.signals |= dcdcSignal ? 0b1000 : 0;
		cmd.valveState = actuatorHandler.valveState;
		cmd.deltaOmegaStart = (int16_t)(deltaOmegaStartup[0] * DELTA_OMEGA_DIVIDOR);
		cmd.deltaOmegaOperation = (int16_t)(deltaOmegaOperation[0] * DELTA_OMEGA_DIVIDOR * 100.0);
		cmd.deltaOmegaShutdown = (int16_t)(deltaOmegaShutdown[0] * DELTA_OMEGA_DIVIDOR);

		//PRINTF("SODS: %d\n", debouncedSODS);

		controlTopic.publish(cmd);
		//PRINTF("Control Topic published\n");

		if (rescheduleLoop) {
			rescheduleLoop = false;
			suspendCallerUntil(rescheduleTime);
		} else
			suspendUntilNextBeat();
	}
}
