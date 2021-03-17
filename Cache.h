#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <map>
#include "CacheBlock.h"

class Cache {
    private:
        // map to simulate cache
        std::map<int, std::vector<CacheBlock>> *sets;
        // tracking variables for performance reporting
        int loads;
        int stores;
        int loadHits;
        int loadMisses;
        int storeHits;
        int storeMisses;
        int cycles;
        // variables for cache settings
        int numSets;
        int numBlocksInSet;
        int numBytesInBlock;
        bool writeAllocate;
        bool writeThrough;
        bool lru;
        // functions
        int getAddressIndex(int fullAddress);
        int getAddressTag(int fullAddress);
        void loadHit(std::vector<CacheBlock>, int counter);
        void loadMiss(std::vector<CacheBlock>, int tag);

    public:
        Cache(int numSets, int numBlocksInSet, int numBytesInBlock, bool writeAllocate, bool writeThrough, bool lru);
        void performLoad(int address);
        void performStore(int address);
        void printResults();
};

#endif