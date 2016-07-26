#ifndef TOPICS_H
#define TOPICS_H

#include "rodos.h"
#include "structs.h"

extern Topic<IMUdata> IMUTopic;
extern CommBuffer<IMUdata> IMUBuffer;

extern Topic<hkData> hkTopic;
extern CommBuffer<hkData> hkBuffer;

extern Topic<cmdData> controlTopic;
extern CommBuffer<cmdData> controlBuffer;

extern Topic<status> healthTopic;
extern CommBuffer<status> healthBuffer;

extern Topic<tcData> tcTopic;
extern CommBuffer<tcData> tcBuffer;

#endif
