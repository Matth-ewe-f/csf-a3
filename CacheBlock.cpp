#include "CacheBlock.h"

/*
 * Getters for fields
 */
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