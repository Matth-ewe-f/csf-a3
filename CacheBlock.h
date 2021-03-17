#ifndef CACHE_BLOCK
#define CACHE_BLOCK

class CacheBlock {

    private:
        bool inCache;
        bool dirty;
        int counter;
    
    public:
        bool isInCache();
        void setInCache(bool val);
        bool isDirty();
        void setDirty(bool val);
        int getCounter();
        void incrementCounter();
        void resetCounter();

};

#endif