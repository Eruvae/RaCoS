#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include "rodos.h"
#include "comminterfaces.h"
#include "spihelper.h"
#include "fatfs/ff.h"

class StorageController : public Thread
{
	bool is_initialized;
public:
    StorageController(const char *name, const long priority);
    int sendCommand(uint8_t command, uint32_t argument, uint8_t crc = 0xFF, uint32_t *trailingData = NULL);
	int initSD();
	int read_sector_segment(uint32_t sector, uint8_t *buf);
	int read_multiple_sector_segments(uint32_t sectorStart, uint32_t sectorCount, uint8_t *buf);
	int write_sector_segment(uint32_t sector, const uint8_t *buf);
	int get_status();
    void run();
};

extern StorageController storageController;

#endif // STORAGECONTROLLER_H
