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
  
  cout << "initialized cache with:" << endl;
  cout << "\t" << numSets << " sets\n"
       << "\t" << numBlocksInSet << " blocks per set\n"
       << "\t" << numBytesInBlock << " bytes in block\n"
       << "\t" << (writeAllocate ? "write-allocate" : "no-write-allocate") << "\n"
       << "\t" << (writeThrough ? "write-through" : "write-back") << "\n"
       << "\t" << (lru == true ? "lru" : "fifo") << endl;
}

void Cache::performLoad(int address) {
  // cout << "performing load" << endl;
}

void Cache::performStore(int address) {
  // cout << "performing store" << endl;
}