#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include "rodos.h"
#include "fatfs/ff.h"
#include "sdcomm.h"

class StorageController : public Thread
{
public:
    StorageController();
    void run();
};

extern StorageController storageController;

#endif // STORAGECONTROLLER_H
