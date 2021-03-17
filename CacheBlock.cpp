#include "CacheBlock.h"

/*
 * CacheBlock Constructor
 */
CacheBlock::CacheBlock(int addressTag, bool initializeInCache) {
    tag = addressTag;
    inCache = initializeInCache;
    dirty = false;
    counter = 0;
}

/*
 * Getters for fields
 */
int CacheBlock::getTag() {
    return tag;
}

bool CacheBlock::isInCache() {
    return inCache;
}

bool CacheBlock::isDirty() {
    return dirty;
}

int CacheBlock::getCounter() {
    return counter;
}

/*
 * Cache block management operations
 */
void CacheBlock::loadIntoCache() {
    inCache = true;
    dirty = false;
    counter = 0;
}

void CacheBlock::evict() {
    inCache = false;
    dirty = false;
}

void CacheBlock::markAsDirty() {
    dirty = true;
}

void CacheBlock::incrementCounter() {
    counter++;
}