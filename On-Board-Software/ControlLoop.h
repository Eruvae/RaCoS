#ifndef CONTROLLOOP_H
#define CONTROLLOOP_H

#include "rodos.h"
#include "topics.h"
#include "actuatorhandler.h"

class ControlLoop: public Thread {
	Mode mode;
	bool test;
	bool lo_last;
	bool soe_last;
	bool sods_last;
public:
	void init();
	void run();
};

#endif /* CONTROLLOOP_H */
