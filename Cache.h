#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <map>
#include "CacheBlock.h"

class Cache {
    private:
        // map to simulate cache
        std::map<int, std::vector<CacheBlock* >* > *sets;
        // tracking variables for performance reporting
        int loads;
        int stores;
        int loadHits;
        int loadMisses;
        int storeHits;
        int storeMisses;
        int cycles;
        // variables for cache settings
        unsigned numSets;
        unsigned numBlocksInSet;
        unsigned numBytesInBlock;
        bool writeAllocate;
        bool writeThrough;
        bool lru;
        // functions
        int logBaseTwo(int input);
        int getAddressIndex(int fullAddress);
        int getAddressTag(int fullAddress);
        void loadHit(std::vector<CacheBlock *>* set, unsigned counter);
        void loadMissSetExists(std::vector<CacheBlock* >* set, int tag);
        void loadMissSetNotExists(int index, int tag);
        void storeHit(std::vector<CacheBlock *>* set, unsigned counter);
        void storeMissSetExists(std::vector<CacheBlock* >* set, int tag);
        void storeMissSetNotExists(int index, int tag);
        void readFromCache();
        void readFromMem();
        void writeToCache();
        void writeToMem();

    public:
        Cache(unsigned numSets, unsigned numBlocksInSet, unsigned numBytesInBlock, bool writeAllocate, bool writeThrough, bool lru);
        ~Cache();
        void performLoad(int address);
        void performStore(int address);
        void printResults();
        void printInitResults();
};

#endif