#include "telecommand.h"

Telecommand::Telecommand()
{
}

int Telecommand::decodeCommand()
{
    //int c = teleUART.getcharNoWait();
    // TODO decodation

    return 0;
}

void Telecommand::run()
{
    while(1)
    {
        teleUART.suspendUntilDataReady();
        decodeCommand();
    }
}
