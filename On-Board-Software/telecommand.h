#ifndef TELECOMMAND_H
#define TELECOMMAND_H

#include "rodos.h"
#include "topics.h"
#include "structs.h"
#include "comminterfaces.h"
#include "murmur.h"

class Telecommand : public Thread
{
public:
    Telecommand();
    void run();
    int decodeCommand(dpCommand &comPack);
};

extern Telecommand telecommand;

#endif // TELECOMMAND_H
