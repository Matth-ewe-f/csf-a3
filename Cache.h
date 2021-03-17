#ifndef CACHE_H
#define CACHE_H

#include <map>
#include "CacheBlock.h"

class Cache {
  // map to simulate cache
    std::map<int, CacheBlock*> *blocks;
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

  public:
    Cache(int numSets, int numBlocksInSet, int numBytesInBlock, bool writeAllocate, bool writeThrough, bool lru);
    void performLoad(int address);
    void performStore(int address);
    void printResults();
};

#endif