#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include "rodos.h"
#include "comminterfaces.h"
#include "spihelper.h"
#include "fatfs/ff.h"

class StorageController : public Thread
{
	//SyncFifo<IMUdata, 100> imuFifo;
	//SyncFifo<HkData, 100> hkFifo;
	//SyncFifo<CmdData, 100> controlFifo;
public:
    StorageController(const char *name, const long priority)  : Thread(name, priority) {}

    void run();
};

extern StorageController storageController;

#endif // STORAGECONTROLLER_H
