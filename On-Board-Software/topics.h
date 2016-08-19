#ifndef TOPICS_H
#define TOPICS_H

#include "rodos.h"
#include "structs.h"

extern Topic<IMUdata> IMUTopic;
extern CommBuffer<IMUdata> IMUBuffer;

extern Topic<HkData> hkTopic;
extern CommBuffer<HkData> hkBuffer;

extern Topic<CmdData> controlTopic;
extern CommBuffer<CmdData> controlBuffer;

extern Topic<Status> healthTopic;
extern CommBuffer<Status> healthBuffer;

extern Topic<TcData> tcTopic;
extern CommBuffer<TcData> tcBuffer;

extern Topic<Mode> modeTopic;
extern CommBuffer<Mode> modeBuffer;

extern Topic<bool> testTopic;
extern CommBuffer<bool> testBuffer;

extern Topic<uint16_t> modulStateTopic;
extern CommBuffer<uint16_t> modulStateBuffer;

#endif
