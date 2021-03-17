#ifndef CACHE_BLOCK
#define CACHE_BLOCK

class CacheBlock {

    private:
        bool inCache;
        bool dirty;
        int counter;
    
    public:

        bool isInCache();
        bool isDirty();
        int getCounter();
        void loadIntoCache();
        void evict();
        void markAsDirty();
        void incrementCounter();

};

#endif