/*
 * LEDcontroller.cpp
 *
 *  Created on: 17.11.2016
 *      Author: Raptoaaah
 */
#include "LEDcontroller.h"

HAL_GPIO statusLED(GPIO_036);

ledcontroller ledcontroller;

void ledcontroller::init() {
	statusLED.init(true, 1, 0);
}

void ledcontroller::run() {
	setPeriodicBeat(0, 125 * MILLISECONDS);
	short state = 0;
	while (true) {
		Mode mode;
		bool test;
		modeBuffer.get(mode);
		testBuffer.get(test);

		if (test) {
			switch (state) {
			case 0:
				if (mode >= STAND_BY) {
					statusLED.setPins(1);
				}
				break;
			case 1:
				statusLED.setPins(0);
				break;
			case 2:
				if (mode >= FLIGHT) {
					statusLED.setPins(1);
				}
				break;
			case 3:
				statusLED.setPins(0);
				break;
			case 4:
				if (mode >= SECURE) {
					statusLED.setPins(1);
				}
				break;
			case 5:
				statusLED.setPins(0);
				break;
			case 6:
				if (mode >= BLOW_DOWN) {
					statusLED.setPins(1);
				}
				break;
			case 7:
				statusLED.setPins(0);
				break;
			default:
				break;
			}
			state = ++state > 7 ? 0 : state;
		} else {
            switch (state) {
                case 0:
                    if (mode >= STAND_BY) {
                        statusLED.setPins(0);
                    }
                    break;
                case 1:
                    statusLED.setPins(1);
                    break;
                case 2:
                    if (mode >= FLIGHT) {
                        statusLED.setPins(0);
                    }
                    break;
                case 3:
                    statusLED.setPins(1);
                    break;
                case 4:
                    if (mode >= SECURE) {
                        statusLED.setPins(0);
                    }
                    break;
                case 5:
                    statusLED.setPins(1);
                    break;
                case 6:
                    if (mode >= BLOW_DOWN) {
                        statusLED.setPins(0);
                    }
                    break;
                case 7:
                    statusLED.setPins(1);
                    break;
                default:
                    break;
            }
            state = ++state > 7 ? 0 : state;
        }
        suspendUntilNextBeat();
    }
}
