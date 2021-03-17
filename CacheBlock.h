#ifndef CACHE_BLOCK
#define CACHE_BLOCK

class CacheBlock {

    private:
        bool inCache;
        bool dirty;
        int counter;
    
    public:
        CacheBlock(bool initializeInCache);
        bool isInCache();
        bool isDirty();
        int getCounter();
        void loadIntoCache();
        void evict();
        void markAsDirty();
        void incrementCounter();

};

#endif