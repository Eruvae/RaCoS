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

#endif // STORAGECONTROLLER_H
