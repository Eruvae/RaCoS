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

Topic<Status> healthTopic(-1, "Health Data");
CommBuffer<Status> healthBuffer;
Subscriber healthSubscriber(healthTopic, healthBuffer);

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

