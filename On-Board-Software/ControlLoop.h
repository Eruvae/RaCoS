#ifndef CONTROLLOOP_H
#define CONTROLLOOP_H

#include "rodos.h"
#include "topics.h"

class ControlLoop: public Thread {
	Mode mode;
	bool test;
public:
	void init();
	void run();
};

#endif /* CONTROLLOOP_H */
