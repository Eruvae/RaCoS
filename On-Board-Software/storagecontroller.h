#ifndef STORAGECONTROLLER_H
#define STORAGECONTROLLER_H

#include "rodos.h"
#include "comminterfaces.h"
#include "spihelper.h"
#include "topics.h"
#include "structs.h"
#include "fatfs/ff.h"

#define PACKBUF_SIZE 50
#define PACKSEND_DELAY 35

class StorageController : public Thread
{
	uint8_t writeBuffer[256];
	volatile int writeLen;
	const TCHAR *writeFname;
	BYTE writeMode;

	DpGeneric bufferedPacks[PACKBUF_SIZE];
	int readPos;
	int len;
	volatile bool formatSD;
	volatile bool resetSD;
	volatile bool downlinkSD;
	uint32_t currentPackage;
	volatile bool ready;
	volatile bool initInProgress;

	int getPckLen(DpGeneric *pack);
	int getPckCounter(DpGeneric *pack);
	//bool downlinkSDPck(DpGeneric *pack);
	void sendSDdata();
	void readFiles();
	void errorState();
public:
    StorageController(const char *name, const long priority) : Thread(name, priority, 10000),
		formatSD(false), downlinkSD(false), currentPackage(0), ready(false), readPos(0), len(0), resetSD(false), initInProgress(true),
		writeLen(0), writeFname(0), writeMode(0){}

    bool isReady();
    bool isInitInProgress();
    bool readFromFile(const TCHAR *fname, void *buffer, UINT len, BYTE mode);
    bool writeToFile(const TCHAR *fname, void *buffer, UINT len, BYTE mode);

    void downlinkData();
    //void sendNextPackage(int pckcnt);
    void format();
    void reset();
    void run();
};

static inline uint32_t generateChecksum(const uint8_t *data, int len)
{
	/*uint32_t hash = 0;
	for (int i = 0; i < len; i++)
		hash += data[i];

	return hash;*/
	return Murmur::mm_hash_32(data, len);
}

extern StorageController storageController;

#endif // STORAGECONTROLLER_H
