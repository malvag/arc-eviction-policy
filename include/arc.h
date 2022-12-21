#ifndef ARC_CACHE_H
#define ARC_CACHE_H
#include <deque>
#include <stdint.h>
#include <string>
#include <unordered_map>

typedef std::string kv_cache_key_t;

typedef std::string kv_cache_value_t;

class arc_cache {
public:
  kv_cache_value_t lookup(kv_cache_key_t);
  void replace(uint64_t, kv_cache_key_t);
  arc_cache(uint64_t target_cache_mem_budget, uint64_t key_value_size);
  void put(kv_cache_key_t key, kv_cache_value_t value);

  std::unordered_map<kv_cache_key_t, kv_cache_value_t> hashed_mem;
  void get_stats();

private:
  void removeFromCache(kv_cache_key_t);
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