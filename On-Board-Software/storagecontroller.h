#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include "rodos.h"

class StorageController : public Thread
{
public:
    StorageController();
    void run();
};

#endif // STORAGECONTROLLER_H
