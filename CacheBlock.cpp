#include "CacheBlock.h"

/*
 * Simple getters and setters for 'inCache' and 'dirty' fields
 */
bool CacheBlock::isInCache() {
    return isInCache;
}

void CacheBlock::setInCache(bool val) {
    inCache = val;
}

bool CacheBlock::isDirty() {
    return dirty;
}

void CacheBlock::setDirty(bool val) {
    dirty = val;
}

/*
 * Getter and setters for 'counter' (used in determining which block to
 * evict). Setters are limited to the type of operations that are meant to
 * be performed on the field
 */
int CacheBlock::getCounter() {
    return counter;
}

void CacheBlock::incrementCounter() {
    counter++;
}

void CacheBlock::resetCounter() {
    counter = 0;
}

