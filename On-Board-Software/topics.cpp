#include "topics.h"

Topic<IMUdata> IMUTopic(-1, "IMU Data");
CommBuffer<IMUdata> IMUBuffer;
Subscriber IMUSubscriber(IMUTopic, IMUBuffer);

Topic<hkData> hkTopic(-1, "Housekeeping Data");
CommBuffer<hkData> hkBuffer;
Subscriber hkSubscriber(hkTopic, hkBuffer);

Topic<cmdData> controlTopic(-1, "Control Data");
CommBuffer<cmdData> controlBuffer;
Subscriber controlSubscriber(controlTopic, controlBuffer);

Topic<status> healthTopic(-1, "Health Data");
CommBuffer<status> healthBuffer;
Subscriber healthSubscriber(healthTopic, healthBuffer);

Topic<tcData> tcTopic(-1, "Telecommand Data");
CommBuffer<tcData> tcBuffer;
Subscriber tcSubscriber(tcTopic, tcBuffer);
