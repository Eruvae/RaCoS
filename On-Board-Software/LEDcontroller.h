/*
 * LEDcontroller.h
 *
 *  Created on: 17.11.2016
 *      Author: Raptoaaah
 */

#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_

#include "rodos.h"
#include "topics.h"
#include "structs.h"

class ledcontroller: public Thread{
private:
public:
	void run();
	void init();
};



#endif /* LEDCONTROLLER_H_ */
