#ifndef CACHE_BLOCK
#define CACHE_BLOCK

class CacheBlock {

    private:
        int tag;
        bool inCache;
        bool dirty;
        int counter;
    
    public:
        CacheBlock(int addressTag, bool initializeInCache);
        int getTag();
        bool isInCache();
        bool isDirty();
        int getCounter();
        void loadIntoCache();
        void evict();
        void markAsDirty();
        void incrementCounter();
        void resetCounter();

};

#endif