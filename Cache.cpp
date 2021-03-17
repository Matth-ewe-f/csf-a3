#include <iostream>
#include <map>
#include <vector>
#include "Cache.h"

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
    sets = new map<int, vector<CacheBlock>>;

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
    delete sets;
}

int Cache::getAddressIndex(int fullAddress) {
    // bit shifting discards offset, mod discards tag
    return (fullAddress >> (numBytesInBlock * 8)) % numSets;
}

int Cache::getAddressTag(int fullAddress) {
    // bit shifting discards offset, division discards index
    return (fullAddress >> (numBytesInBlock * 8)) / numSets;
}

void Cache::loadHit(vector<CacheBlock> set, int counter) {
    // Assume LRU for MS2, so handle that case only
    for (vector<CacheBlock>::iterator it = set.begin();
            it != set.end();
            it++) {
        if (it->getCounter() < counter) {
            it->incrementCounter();
        } else if (it->getCounter() == counter) {
            it->resetCounter();
        }
    }
}

void Cache::loadMiss(vector<CacheBlock> set, int tag) {
    // increment all of the counters
    for (vector<CacheBlock>::iterator it = set.begin();
            it != set.end();
            it++) {
        it->incrementCounter();
    }
    // if the set is full, evict one block
    if (set.size() >= numBlocksInSet) {
        // find the block to evict
        for (vector<CacheBlock>::iterator it = set.begin();
                it != set.end();
                it++) {
            if (it->getCounter() == numBlocksInSet) {
                set.erase(it);
                break;
            }
        }
    }
    // add the new block to the cache
    CacheBlock newBlock = CacheBlock(tag);
    set.push_back(newBlock);
}

void Cache::performLoad(int address) {
    loads++;
    // get relevant pieces of address
    int index = getAddressIndex(address);
    int tag = getAddressTag(address);

    // search the cache for the requested block
    vector<CacheBlock> set = sets->at(index);
    for (vector<CacheBlock>::iterator it = set.begin();
            it != set.end();
            it++) {
        if (tag == it->getTag()) {
            // a hit has occurred
            loadHits++;
            loadHit(set, it->getCounter());
            return;
        }
    }
    // a miss has occurred
    loadMisses++;
    loadMiss(set, tag);
}

void Cache::performStore(int address) {
    stores++;
    // get relevant pieces of address
    int index = getAddressIndex(address);
    int tag = getAddressTag(address);

    // search the cache for the requested block
    vector<CacheBlock> set = sets->at(index);
    for (vector<CacheBlock>::iterator it = set.begin();
            it != set.end();
            it++) {
        if (tag == it->getTag()) {
            storeHits++;
            // TODO cache hit has occured
        }
    }
    storeMisses++;
    // TODO cache miss has occured
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