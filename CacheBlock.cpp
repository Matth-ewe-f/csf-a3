// Matthew Flynn (mflynn24) and Andrew Suh (wsuh1)
// CacheBlock Object, simply tracks info about each cache block in a set
// CSF Assignment #3

#include "CacheBlock.h"

/*
 * Constructs a new CacheBlock object
 * 
 * Parameters:
 *   addressTag - the tag of the block being stored in this object
 * 
 * Returns:
 *   The newly created object
 */
CacheBlock::CacheBlock(int addressTag) {
    tag = addressTag;
    dirty = false;
    counter = 0;
}

/*
 * Returns the tag of the block being stored in this object
 */
int CacheBlock::getTag() {
    return tag;
}

/*
 * Returns whether or not this block has been written to since it was loaded
 */
bool CacheBlock::isDirty() {
    return dirty;
}

/*
 * Returns the fifo/lru counter for this block
 */
unsigned CacheBlock::getCounter() {
    return counter;
}

/*
 * Marks this block as dirty: it's been written to since it was loaded
 */
void CacheBlock::markAsDirty() {
    dirty = true;
}

/*
 * Increments this blocks fifo/lru counter
 */
void CacheBlock::incrementCounter() {
    counter++;
}

/*
 * Sets this block's fifo/lru counter to zero
 */
void CacheBlock::resetCounter() {
    counter = 0;
}