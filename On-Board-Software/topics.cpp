#include "topics.h"

Topic<IMUdata> IMUTopic(-1, "IMU Data");
CommBuffer<IMUdata> IMUBuffer;
Subscriber IMUSubscriber(IMUTopic, IMUBuffer);

Topic<HkData> hkTopic(-1, "Housekeeping Data");
CommBuffer<HkData> hkBuffer;
Subscriber hkSubscriber(hkTopic, hkBuffer);

Topic<CmdData> controlTopic(-1, "Control Data");
CommBuffer<CmdData> controlBuffer;
Subscriber controlSubscriber(controlTopic, controlBuffer);

Topic<Status> sensorIMUStatusTopic(-1, "IMU Status");
CommBuffer<Status> sensorIMUStatusBuffer;
Subscriber sensorIMUStatusSubscriber(sensorIMUStatusTopic, sensorIMUStatusBuffer);

Topic<Status> housekeepingStatusTopic(-1, "Houskeeping Status");
CommBuffer<Status> housekeepingStatusBuffer;
Subscriber housekeepingStatusSubscriber(housekeepingStatusTopic, housekeepingStatusBuffer);

Topic<Status> controlLoopStatusTopic(-1, "Control Loop Status");
CommBuffer<Status> controlLoopStatusBuffer;
Subscriber controlLoopStatusSubscriber(controlLoopStatusTopic, controlLoopStatusBuffer);

Topic<Status> storageControllerStatusTopic(-1, "Storage Controller Status");
CommBuffer<Status> storageControllerStatusBuffer;
Subscriber storageControllerStatusSubscriber(storageControllerStatusTopic, storageControllerStatusBuffer);

Topic<Status> telemetryStatusTopic(-1, "Telemetry Status");
CommBuffer<Status> telemetryStatusBuffer;
Subscriber telemetryStatusSubscriber(telemetryStatusTopic, telemetryStatusBuffer);

Topic<TcData> tcTopic(-1, "Telecommand Data");
CommBuffer<TcData> tcBuffer;
Subscriber tcSubscriber(tcTopic, tcBuffer);

Topic<Mode> modeTopic(-1, "Software Mode");
CommBuffer<Mode> modeBuffer;
Subscriber modeSubscriber(modeTopic, modeBuffer);

Topic<bool> testTopic(-1, "Test Mode");
CommBuffer<bool> testBuffer;
Subscriber testSubscriber(testTopic, testBuffer);

Topic<uint16_t> modulStateTopic(-1, "Modul States");
CommBuffer<uint16_t> modulStateBuffer;
Subscriber modulStateSubscriber(modulStateTopic, modulStateBuffer);

Topic<uint32_t> sdDLPackageTopic(-1, "SD Downlink Package");
CommBuffer<uint32_t> sdDLPackageBuffer;
Subscriber sdDLPackageSubscriber(sdDLPackageTopic, sdDLPackageBuffer);

Topic<uint8_t> signalTopic(-1, "Signal States");
CommBuffer<uint8_t> signalTopicBuffer;
Subscriber signalTopicSubscriber(signalTopic, signalTopicBuffer);

Topic<bool> resetControllerTopic(-1, "Reset Controller");
CommBuffer<bool> resetControllerBuffer;
Subscriber resetControllerSubscriber(resetControllerTopic, resetControllerBuffer);

Topic<bool> spinupDirectionTopic(-1, "Spinup Direction");
CommBuffer<bool> spinupDirectionBuffer;
Subscriber spinupDirectionSubscriber(spinupDirectionTopic, spinupDirectionBuffer);

Topic<DpDebug> dlDebugTopic(-1, "Downlink Debug");
Fifo<DpDebug, 10> dlDebugFifo;
Subscriber dlDebugSubscriber(dlDebugTopic, dlDebugFifo);

Topic<DownlinkIMU> dlIMUTopic(-1, "Downlink IMU");
Fifo<DownlinkIMU, 10> dlIMUFifo;
Subscriber dlIMUSubscriber(dlIMUTopic, dlIMUFifo);

Topic<DownlinkPresTemp> dlPTTopic(-1, "Downlink PT");
Fifo<DownlinkPresTemp, 10> dlPTFifo;
Subscriber dlPTSubscriber(dlPTTopic, dlPTFifo);

Topic<DownlinkCalc> dlCalcTopic(-1, "Downlink Calc");
Fifo<DownlinkCalc, 10> dlCalcFifo;
Subscriber dlCalcSubscriber(dlCalcTopic, dlCalcFifo);

