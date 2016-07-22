#include "topics.h"

Topic<IMUdata> IMUTopic;
CommBuffer<IMUdata> IMUBuffer;
Subscriber IMUSubscriber(IMUTopic, IMUBuffer);

Topic<hkData> hkTopic;
CommBuffer<hkData> hkBuffer;
Subscriber hkSubscriber(hkTopic, hkBuffer);

Topic<cmdData> controlTopic;
CommBuffer<cmdData> controlBuffer;
Subscriber controlSubscriber(controlTopic, controlBuffer);

Topic<status> healthTopic;
CommBuffer<status> healthBuffer;
Subscriber healthSubscriber(healthTopic, healthBuffer);

Topic<tcData> tcTopic;
CommBuffer<tcData> tcBuffer;
Subscriber tcSubscriber(tcTopic, tcSubscriber);