#include <iostream>
#include <map>
#include <vector>
#include "Cache.h"
#include "CacheBlock.h"

using std::cout;
using std::endl;
using std::map;
using std::vector;

Cache::Cache (int numSets, int numBlocksInSet, int numBytesInBlock, bool writeAllocate, bool writeThrough, bool lru) {
  // initialize counters
  this->loads = 0;
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
  
  // initialize map used to keep track of cache blocks
  blocks = new map<int, vector<CacheBlock>>;

  // print initialization settings
  cout << "initialized cache with:" << endl;
  cout << "\t" << numSets << " sets\n"
       << "\t" << numBlocksInSet << " blocks per set\n"
       << "\t" << numBytesInBlock << " bytes in block\n"
       << "\t" << (writeAllocate ? "write-allocate" : "no-write-allocate") << "\n"
       << "\t" << (writeThrough ? "write-through" : "write-back") << "\n"
       << "\t" << (lru == true ? "lru" : "fifo") << endl;
}

Cache::~Cache() {
  delete blocks;
}

void Cache::performLoad(int address) {
  // TODO: handle load opeartions with the pre-determined cache settings
}

void Cache::performStore(int address) {
  // TODO: handle store opeartions with the pre-determined cache settings
}

void Cache::printResults() {
  cout << "Total loads: " << loads
       << "\nTotal stores: " << stores
       << "\nLoad hits: " << loadHits
       << "\nLoad misses: " << loadMisses
       << "\nStore hits: " << storeHits
       << "\nStore misses: " << storeMisses
       << "\nTotal cycles: " << cycles << endl;
}