// Matthew Flynn (mflynn24) and Andrew Suh (wsuh1)s
// Cache object, used to simulate loading/storing of cache data
// CSF Assignment #3

#include <iostream>
#include <map>
#include <vector>
#include "Cache.h"

using std::cout;
using std::endl;
using std::map;
using std::vector;

/*
 * Creates a new Cache Object
 * 
 * Parameters:
 *   numSets - the number of sets in the cache
 *   numBlocksInSet - the number of blocks in each set
 *   numBytesInBlock - the number of bytes in (size of) each block
 *   writeAllocate - whether or not the cache is write allocate
 *   writeThrough - whether or not the cache is write through
 *   lru - whether or not the cache used the LRU strategy for evictions
 * 
 * Returns:
 *   A new Cache object, with the specified configuration
 * 
 */ 
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

/*
 * Computes the log base two of a number, rounding down to the nearest integer
 * 
 * Parameters:
 *   input - the number of which to take the log
 * 
 * Returns:
 *   the log base two of the given integer, rounded down to the nearest integer
 */ 
int Cache::logBaseTwo(int input) {
    int index = 0;
    while (input > 1) {
        index++;
        input /= 2;
    }
    return index;
}

/*
 * Computes the index of a memory address
 * 
 * Parameters:
 *   fullAddress - the memory address
 * 
 * Returns:
 *   the index of the given memory address, based on the configuration settings
 *   of this cache object
 * 
 */ 
int Cache::getAddressIndex(int fullAddress) {
    // bit shifting discards offset, mod discards tag
    return (fullAddress >> logBaseTwo(numBytesInBlock)) % numSets;
}

/*
 * Computes the tag of a memory address
 * 
 * Parameters:
 *   fullAddress - the memory address
 * 
 * Returns:
 *   the tag of the given memory address, based on the configuration settings
 *   of this cache object
 * 
 */ 
int Cache::getAddressTag(int fullAddress) {
    // bit shifting discards offset, division discards index
    return (fullAddress >> logBaseTwo(numBytesInBlock)) / numSets;
}

/*
 * Handles what should happen on a load hit. increases cycle count and updates
 * lru counters
 * 
 * Parameters:
 *   set - pointer to the vector of cache blocks (i.e. the cache set) in which
 *   the hit occurred
 *   counter - the value of the lru counter of the block that was hit before
 *   the hit occurred
 * 
 */ 
void Cache::loadHit(vector<CacheBlock* > * set, unsigned counter) {
    if (lru) {
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
    
    readFromCache();
}

/*
 * Handles what should happen on a load miss, when there is already at least
 * one block in the cache set that the new block needs to be put into. 
 * increases cycle count, updates lru/fifo counters, and performs evictions if
 * necessary
 * 
 * Parameters:
 *   set - pointer to the vector of cache blocks (i.e. the cache set) in which
 *   the new block should be loaded
 *   tag - the tag of the memory address that was requested
 * 
 */ 
void Cache::loadMissSetExists(vector<CacheBlock* > * set, int tag) {
    // increment all of the counters
    for (vector<CacheBlock* >::iterator it = set->begin();
            it != set->end();
            it++) {
        (*it)->incrementCounter();
    }
    // if the set is full, evict one block
    if (set->size() == numBlocksInSet) {
        // find the block to evict
        for (vector<CacheBlock *>::iterator it = set->begin();
                it != set->end();
                it++) {
            if ((*it)->getCounter() == numBlocksInSet) {
                if (!writeThrough && (*it)->isDirty()) {
                    writeBlockToMem();
                }
                delete *it;
                set->erase(it);
                break;
            }
        }
        writeToCache();
    }
    // add the new block to the cache
    CacheBlock * newBlock = new CacheBlock(tag);
    readFromMem();
    readFromCache();
    set->push_back(newBlock);
}

/*
 * Handles what should happen on a load miss, when there are no blocks of the
 * same index loaded into the cache already. Creates the set and increases the
 * cycle count
 * 
 * Parameters:
 *   index - the index of the requested block of memory
 *   tag - the tag of the requested block of memory
 * 
 */ 
void Cache::loadMissSetNotExists(int index, int tag) {
    // the set must be created
    vector<CacheBlock* > * set = new vector<CacheBlock *>;
    CacheBlock * block = new CacheBlock(tag);
    readFromMem();
    readFromCache();
    set->push_back(block);
    sets->insert({index, set});
}

/*
 * Determines if a load request resolves in a hit or miss, and then handles
 * the request appropriately
 * 
 * Parameters:
 *   address - the memory address that is being loaded
 * 
 */ 
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

/*
 * Handles what should happen on a store miss, when there is already at least
 * one block in the cache set that has the same index as the block being
 * reqested. increases cycle count, and if necessitated by the cache 
 * configuration, loads the block into the cache, updates fifo/lru counters,
 * and performs evictions if necessary
 * 
 * Parameters:
 *   set - pointer to the vector of cache blocks (i.e. the cache set) for
 *   blocks of the index of the requested address
 *   tag - the tag of the memory address that was requested
 * 
 */ 
void Cache::storeMissSetExists(vector<CacheBlock *> * set, int tag) {
    // if no-write-allocate, just write to mem and that's it
    if (!writeAllocate) {
        writeToMemFourBytes();
        return;
    }
    // increment all counters to make room for new block with counter = 0
    for (vector<CacheBlock *>::iterator it = set->begin();
            it != set->end();
            it++) {
        (*it)->incrementCounter();
    }
    // if an eviction must occur, perform it
    if (set->size() == numBlocksInSet) {
        for (vector<CacheBlock *>::iterator it = set->begin();
                it != set->end();
                it++) {
            if ((*it)->getCounter() == numBlocksInSet) {
                if (!writeThrough && (*it)->isDirty()) {
                    writeBlockToMem();
                }
                delete *it;
                it = set->erase(it);
                break;
            }
        }
    }
    // add the new block
    CacheBlock * block = new CacheBlock(tag);
    set->push_back(block);
    readFromMem();
    if (writeThrough) {
        writeToMemFourBytes();
    } else {
        writeToCache();
        block->markAsDirty();
    }
}

/*
 * Handles what should happen on a store miss, when there are no blocks of the
 * same index loaded into the cache already. Increases the cycle count, and if
 * necessitated by the cache settings, creates the set and loads the block into
 * the cache.
 * 
 * Parameters:
 *   index - the index of the requested block of memory
 *   tag - the tag of the requested block of memory
 * 
 */ 
void Cache::storeMissSetNotExists(int index, int tag) {
    // if no-write-allocate, simply write to mem and that's it
    if (!writeAllocate) {
        writeToMemFourBytes();
        return;
    }
    vector<CacheBlock *> * set = new vector<CacheBlock *>;
    CacheBlock * block = new CacheBlock(tag);
    readFromMem();
    if (writeThrough) {
        writeToMemFourBytes();
    } else {
        writeToCache();
        block->markAsDirty();
    }
    set->push_back(block);
    sets->insert({index, set});
}

/*
 * Handles what should happen on a store hit. increases cycle count and updates
 * lru counters
 * 
 * Parameters:
 *   set - pointer to the vector of cache blocks (i.e. the cache set) in which
 *   the hit occurred
 *   counter - the value of the lru counter of the block that was hit before
 *   the hit occurred
 * 
 */ 
void Cache::storeHit(vector<CacheBlock *> * set, unsigned counter) {
    if (lru) {
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
    }
    writeToCache();
    if (writeThrough) {
        writeToMemFourBytes();
    }
}

/*
 * Determines if a store request resolves in a hit or miss, and then handles
 * the request appropriately
 * 
 * Parameters:
 *   address - the memory address that is being written to
 * 
 */ 
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

/*
 * Updates cycle counter when a value is read from the cache.
 */ 
void Cache::readFromCache() {
    cycles += 1;
}

/*
 * Updates cycle counter when a value is read from memory.
 */ 
void Cache::readFromMem() {
    cycles += 100 * (numBytesInBlock / 4);
}

/*
 * Updates cycle counter when a value is written to the cache.
 */ 
void Cache::writeToCache() {
    cycles += 1;
}

/*
 * Updates cycle counter when a block is written to memory.
 */ 
void Cache::writeBlockToMem() {
    cycles += 100 * (numBytesInBlock / 4);
}

/*
 * Updates cycle counter when four bytes (smallest unit) are written to memory
 */ 
void Cache::writeToMemFourBytes() {
    cycles += 100;
}

/*
 * Prints the results of the cache simulation in the format required by the
 * autograder.
 */ 
void Cache::printResults() {
    cout << "Total loads: " << loads
             << "\nTotal stores: " << stores
             << "\nLoad hits: " << loadHits
             << "\nLoad misses: " << loadMisses
             << "\nStore hits: " << storeHits
             << "\nStore misses: " << storeMisses
             << "\nTotal cycles: " << cycles << endl;
}

/*
 * Prints the results of the cache simulation are bare numbers without labels
 */ 
void Cache::printResultsNoText() {
    cout << loads 
        << "\n" << stores 
        << "\n" << loadHits 
        << "\n" << loadMisses 
        << "\n"  << storeHits 
        << "\n"  << storeMisses 
        << "\n"  << cycles << endl;
}

/*
 * Prints the configuration settings of the cache
 */ 
void Cache::printInitResults() {
    cout << "initialized cache with:" << endl;
    cout << "\t" << numSets << " sets\n"
             << "\t" << numBlocksInSet << " blocks per set\n"
             << "\t" << numBytesInBlock << " bytes in block\n"
             << "\t" << (writeAllocate ? "write-allocate" : "no-write-allocate") << "\n"
             << "\t" << (writeThrough ? "write-through" : "write-back") << "\n"
             << "\t" << (lru == true ? "lru" : "fifo") << endl;
}