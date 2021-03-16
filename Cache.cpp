#include "Cache.h"
#include <iostream>
using namespace std;

Cache::Cache (int numSets, int numBlocksInSet, int numBytesInBlock, bool writeAllocate, bool writeThrough, bool lru) {
  // initialize counters
  this->loans = 0;
  this->stores = 0;
  this->loadHits = 0;
  this->loadMisses = 0;
  this->storeHits = 0;
  this->storeMisses = 0;
  this->cycles = 0;
  // initialize Cache configurations
  this->numSets = numSets;
  this->numBlocksInSet = numBlocksInSet;
  this->numBytesInBlock = numBytesInBlock;
  this->writeAllocate = writeAllocate;
  this->writeThrough = writeThrough;
  this->lru = lru;

  // TODO: initialize the map data structure for cache
  
  cout << "initialized cache" << endl;
}

void Cache::performLoad() {
  
}

void Cache::performStore() {
  
}