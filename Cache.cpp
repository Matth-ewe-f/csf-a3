#include <iostream>
#include <map>
#include <vector>
#include "Cache.h"

using std::cout;
using std::endl;
using std::map;
using std::vector;

Cache::Cache (unsigned numSets, unsigned numBlocksInSet, unsigned numBytesInBlock, bool writeAllocate, bool writeThrough, bool lru) {
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
    sets = new map<int, vector<CacheBlock* >* >;

    // print initialization settings
    // printInitResults();
}

Cache::~Cache() {
    for (map<int, vector<CacheBlock* >* >::iterator it = sets->begin();
            it != sets->end();
            it++) {
        vector<CacheBlock *> * set = it->second;
        for (vector<CacheBlock*>::iterator jt = set->begin();
                jt != set->end();
                jt++) {
            delete *jt;
        }
        delete set;
    }
    delete sets;
}

int Cache::logBaseTwo(int input) {
    int index = 0;
    while (input > 1) {
        index++;
        input /= 2;
    }
    return index;
}

int Cache::getAddressIndex(int fullAddress) {
    // bit shifting discards offset, mod discards tag
    return (fullAddress >> logBaseTwo(numBytesInBlock)) % numSets;
}

int Cache::getAddressTag(int fullAddress) {
    // bit shifting discards offset, division discards index
    return (fullAddress >> logBaseTwo(numBytesInBlock)) / numSets;
}

void Cache::loadHit(vector<CacheBlock* > * set, unsigned counter) {
    // Assume LRU for MS2, so handle that case only
    for (vector<CacheBlock* >::iterator it = set->begin();
            it != set->end();
            it++) {
        if ((*it)->getCounter() < counter) {
            (*it)->incrementCounter();
        } else if ((*it)->getCounter() == counter) {
            (*it)->resetCounter();
        }
    }
}

void Cache::loadMissSetExists(vector<CacheBlock* > * set, int tag) {
    // increment all of the counters
    for (vector<CacheBlock* >::iterator it = set->begin();
            it != set->end();
            it++) {
        (*it)->incrementCounter();
    }
    // if the set is full, evict one block
    if (set->size() == numBlocksInSet - 1) {
        // find the block to evict
        for (vector<CacheBlock *>::iterator it = set->begin();
                it != set->end();
                it++) {
            if ((*it)->getCounter() == numBlocksInSet - 1) {
                set->erase(it);
                break;
            }
        }
    }
    // add the new block to the cache
    CacheBlock * newBlock = new CacheBlock(tag);
    set->push_back(newBlock);
}

void Cache::loadMissSetNotExists(int index, int tag) {
    // the set must be created
    vector<CacheBlock* > * set = new vector<CacheBlock *>;
    CacheBlock * block = new CacheBlock(tag);
    set->push_back(block);
    sets->insert({index, set});
}

void Cache::performLoad(int address) {
    loads++;
    // get relevant pieces of address
    int index = getAddressIndex(address);
    int tag = getAddressTag(address);

    vector<CacheBlock *> * set;
    // search the cache for the requested block
    if (sets->find(index) != sets->end()) {
        set = sets->at(index);
        for (vector<CacheBlock *>::iterator it = set->begin();
            it != set->end();
            it++) {
            if (tag == (*it)->getTag()) {
                // a hit has occurred
                loadHits++;
                loadHit(set, (*it)->getCounter());
                return;
            }
        }
        // if this point is reached, cache miss
        loadMisses++;
        loadMissSetExists(set, tag);
    } else {
        // a miss has occurred
        loadMisses++;
        loadMissSetNotExists(index, tag);
    }
}

void Cache::storeMissSetExists(vector<CacheBlock *> * set, int tag) {
    // increment all counters to make room for new block with counter = 0
    for (vector<CacheBlock *>::iterator it = set->begin();
            it != set->end();
            it++) {
        (*it)->incrementCounter();
    }
    // if an eviction must occur, perform it
    if (set->size() == numBlocksInSet - 1) {
        for (vector<CacheBlock *>::iterator it = set->begin();
                it != set->end();
                it++) {
            if ((*it)->getCounter() == numBlocksInSet - 1) {
                if (!writeThrough && (*it)->isDirty()) {
                    writeToMem();
                }
                it = set->erase(it);
                break;
            }
        }
    }
    // add the new block
    CacheBlock * block = new CacheBlock(tag);
    set->push_back(block);
    writeToCache();
    if (writeThrough) {
        writeToMem();
    } else {
        block->markAsDirty();
    }
}

void Cache::storeMissSetNotExists(int index, int tag) {
    vector<CacheBlock *> * set = new vector<CacheBlock *>;
    CacheBlock * block = new CacheBlock(tag);
    writeToCache();
    if (writeThrough) {
        writeToMem();
    } else {
        block->markAsDirty();
    }
    set->push_back(block);
    sets->insert({index, set});
}

void Cache::storeHit(vector<CacheBlock *> * set, unsigned counter) {
    // only consider LRU case for MS2, so update counters for LRU case
    for (vector<CacheBlock *>::iterator it = set->begin();
            it != set->end();
            it++) {
        if ((*it)->getCounter() < counter) {
            (*it)->incrementCounter();
        } else if ((*it)->getCounter() == counter) {
            (*it)->resetCounter();
            if (!writeThrough) {
                (*it)->markAsDirty();
            }
        }
    }
    writeToCache();
    if (writeThrough) {
        writeToMem();
    }
}

void Cache::performStore(int address) {
    stores++;
    // get relevant pieces of address
    int index = getAddressIndex(address);
    int tag = getAddressTag(address);

    vector<CacheBlock *> * set;
    // search the cache for the requested block
    if (sets->find(index) != sets->end()) {
        set = sets->at(index);
        for (vector<CacheBlock *>::iterator it = set->begin();
            it != set->end();
            it++) {
            if (tag == (*it)->getTag()) {
                // a hit has occurred
                storeHits++;
                storeHit(set, (*it)->getCounter());
                return;
            }
        }
        // if this point is reached, cache miss
        storeMisses++;
        storeMissSetExists(set, tag);
    } else {
        storeMisses++;
        storeMissSetNotExists(index, tag);
    }
}

void Cache::writeToCache() {
    cycles += 1;
}

void Cache::writeToMem() {
    cycles += 100 * (numBytesInBlock / 4);
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

void Cache::printInitResults() {
    cout << "initialized cache with:" << endl;
    cout << "\t" << numSets << " sets\n"
             << "\t" << numBlocksInSet << " blocks per set\n"
             << "\t" << numBytesInBlock << " bytes in block\n"
             << "\t" << (writeAllocate ? "write-allocate" : "no-write-allocate") << "\n"
             << "\t" << (writeThrough ? "write-through" : "write-back") << "\n"
             << "\t" << (lru == true ? "lru" : "fifo") << endl;
}