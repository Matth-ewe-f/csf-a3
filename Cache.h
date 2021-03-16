#ifndef CACHE_H
#define CACHE_H

#include <map>

class Cache {
  // map to simulate cache
    std::map<int, int> data;
  // tracking variables for performance reporting
    int loans;
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
    void performLoad();
    void performStore();
};

#endif