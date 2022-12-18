#ifndef ARC_CACHE_H
#define ARC_CACHE_H
#include <stdint.h>

#include "cache.h"

class arc_cache : public usr_level_cache {
   public:
    kv_cache_value_t lookup(kv_cache_key_t, kv_cache_value_t);
    void replace(uint64_t, kv_cache_key_t);
    void removeFromCache(kv_cache_key_t);
    arc_cache(uint64_t);
   private:
    uint64_t hit_count;
    uint64_t miss_count;
    uint64_t lookup_count;
    uint64_t capacity;
    uint64_t p;
    std::deque<std::string> T1;
    std::deque<std::string> B1;
    std::deque<std::string> T2;
    std::deque<std::string> B2;
};

#endif