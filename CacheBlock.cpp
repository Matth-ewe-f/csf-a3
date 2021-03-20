#include "CacheBlock.h"

/*
 * CacheBlock Constructor
 */
CacheBlock::CacheBlock(int addressTag) {
    tag = addressTag;
    dirty = false;
    counter = 0;
}

/*
 * Getters for fields
 */
int CacheBlock::getTag() {
    return tag;
}

bool CacheBlock::isDirty() {
    return dirty;
}

unsigned CacheBlock::getCounter() {
    return counter;
}

CacheBlock* CacheBlock::getThis() {
    return this;
}

/*
 * Cache block management operations
 */
void CacheBlock::markAsDirty() {
    dirty = true;
}

void CacheBlock::incrementCounter() {
    counter++;
}

void CacheBlock::resetCounter() {
    counter = 0;
}