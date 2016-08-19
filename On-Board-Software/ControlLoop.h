#ifndef CONTROLLOOP_H_
#define CONTROLLOOP_H_

#include "rodos.h"
#include "topics.h"

class ControlLoop: public Thread {
	Mode mode;
	bool test;
public:
	void init();
	void run();
};

#endif /* CONTROLLOOP_H_ */
