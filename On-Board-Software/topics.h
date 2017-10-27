#ifndef TOPICS_H
#define TOPICS_H

#include "rodos.h"
#include "structs.h"
#include "murmur.h"

extern Topic<IMUdata> IMUTopic;
extern CommBuffer<IMUdata> IMUBuffer;

extern Topic<HkData> hkTopic;
extern CommBuffer<HkData> hkBuffer;

extern Topic<CmdData> controlTopic;
extern CommBuffer<CmdData> controlBuffer;

extern Topic<Status> sensorIMUStatusTopic;
extern CommBuffer<Status> sensorIMUStatusBuffer;

extern Topic<Status> housekeepingStatusTopic;
extern CommBuffer<Status> housekeepingStatusBuffer;

extern Topic<Status> controlLoopStatusTopic;
extern CommBuffer<Status> controlLoopStatusBuffer;

extern Topic<Status> storageControllerStatusTopic;
extern CommBuffer<Status> storageControllerStatusBuffer;

extern Topic<Status> telemetryStatusTopic;
extern CommBuffer<Status> telemetryStatusBuffer;

extern Topic<TcData> tcTopic;
extern CommBuffer<TcData> tcBuffer;

extern Topic<Mode> modeTopic;
extern CommBuffer<Mode> modeBuffer;

extern Topic<bool> testTopic;
extern CommBuffer<bool> testBuffer;

extern Topic<uint16_t> modulStateTopic;
extern CommBuffer<uint16_t> modulStateBuffer;

extern Topic<uint32_t> sdDLPackageTopic;
extern CommBuffer<uint32_t> sdDLPackageBuffer;

extern Topic<uint8_t> signalTopic;
extern CommBuffer<uint8_t> signalTopicBuffer;

extern Topic<bool> resetControllerTopic;
extern CommBuffer<bool> resetControllerBuffer;

extern Topic<bool> spinupDirectionTopic;
extern CommBuffer<bool> spinupDirectionBuffer;

extern Topic<DpDebug> dlDebugTopic;
extern Fifo<DpDebug, 10> dlDebugFifo;

extern Topic<DownlinkIMU> dlIMUTopic;
extern Fifo<DownlinkIMU, 10> dlIMUFifo;

extern Topic<DownlinkPresTemp> dlPTTopic;
extern Fifo<DownlinkPresTemp, 10> dlPTFifo;

extern Topic<DownlinkCalc> dlCalcTopic;
extern Fifo<DownlinkCalc, 10> dlCalcFifo;

#endif
