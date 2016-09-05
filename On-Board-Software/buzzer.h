/*
 * buzzer.h
 *
 *  Created on: 03.09.2016
 *      Author: Raptoaaah
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "rodos.h"
class Buzzer : public Thread{
public:
	void init();
	void run();
	void playTone(int tone, int duration);
};
#endif /* BUZZER_H_ */
