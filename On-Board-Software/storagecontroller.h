#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include "rodos.h"
#include "comminterfaces.h"
#include "spihelper.h"
#include "topics.h"
#include "structs.h"
#include "fatfs/ff.h"



class StorageController : public Thread
{
public:
    StorageController(const char *name, const long priority)  : Thread(name, priority, 10000) {}

    void run();
};

extern StorageController storageController;

#endif // STORAGECONTROLLER_H
