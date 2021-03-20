#ifndef CACHE_BLOCK
#define CACHE_BLOCK

class CacheBlock {

    private:
        int tag;
        bool inCache;
        bool dirty;
        unsigned counter;
    
    public:
        CacheBlock(int addressTag);
        int getTag();
        bool isDirty();
        unsigned getCounter();
        void markAsDirty();
        void incrementCounter();
        void resetCounter();
        CacheBlock* getThis();

};

#endif