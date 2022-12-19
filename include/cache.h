#ifndef USR_LEVEL_CACHE_H
#define USR_LEVEL_CACHE_H
#include <stdint.h>

#include "absl/container/node_hash_map.h"

// struct kv_pair{

// }

typedef std::string kv_cache_key_t;
typedef std::string kv_cache_value_t;

class usr_level_cache {
public:

  virtual kv_cache_value_t lookup(kv_cache_key_t, kv_cache_value_t) = 0;
  virtual void replace(uint64_t, kv_cache_key_t) = 0;
};

#endif