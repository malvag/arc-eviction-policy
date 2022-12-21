
#include "arc.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

arc_cache::arc_cache(uint64_t target_cache_mem_budget,
                     uint64_t key_value_size) {

  cacheLock.lock();
  p = 0;
  capacity = target_cache_mem_budget / key_value_size;
  lookup_count = 0;
  hit_count = 0;
  miss_count = 0;
  cacheLock.unlock();
  // absl::ParseCommandLine();
  // absl::InitializeLog();
}

static inline double round_to(double value, double precision = 1.0) {
  return std::round(value / precision) * precision;
}

void arc_cache::get_stats() {
  cacheLock.lock();
  std::cout << "----\n";
  std::cout << "ARC replacement policy:\n";
  std::cout << "----\n";
  // std::cout << "hit ratio:" <<
  // round_to((double)hit_count/(double)lookup_count,2);
  std::cout << "hit ratio:" << (double)hit_count / (double)lookup_count << "\n";
  std::cout << "lookups:" << lookup_count << "\n";
  std::cout << "hits:" << hit_count << "\n";
  std::cout << "misses:" << miss_count << "\n";
  std::cout << "----\n";
  cacheLock.unlock();
}

/*
If (T1 is not empty) and ((T1 lenght exceeds the target p) or (x is in B2 and T1
lenght == p)) Delete the LRU page in T1 (also remove it from the cache), and
move it to MRU position in B1. else Delete the LRU page in T2 (also remove it
from the cache), and move it to MRU position in B2. endif
*/

bool contains(std::deque<kv_cache_key_t> &v, kv_cache_key_t key) {
  if (std::find(v.begin(), v.end(), key) != v.end())
    return true;
  return false;
}

kv_cache_key_t removeLRUentryFrom(std::deque<kv_cache_key_t> &v) {
  if (v.size()) {
    kv_cache_key_t key = v.back();
    v.pop_back();
    return key;
  }
  return "(null)";
}

void arc_cache::removeFromCache(kv_cache_key_t key) {
  auto it = hashed_mem.find(key);
  if (it != hashed_mem.end())
    hashed_mem.erase(it);
}

void arc_cache::replace(uint64_t p, kv_cache_key_t key) {
  if ((T1.size() >= 1) &&
      ((contains(B2, key) && (T1.size() == p)) || (T1.size() > p))) {
    // take LRU entry from T1 and
    kv_cache_key_t lru_entry_key = removeLRUentryFrom(T1);
    // add it on top of B1
    B1.emplace_front(lru_entry_key);
    // then remove it from cache
    removeFromCache(lru_entry_key);
  } else {
    // take LRU entry from T2 and
    kv_cache_key_t lru_entry_key = removeLRUentryFrom(T2);
    // add it on top of B2
    B2.emplace_front(lru_entry_key);
    // then remove it from cache
    removeFromCache(lru_entry_key);
  }
}

void arc_cache::put(kv_cache_key_t key, kv_cache_value_t value) {
  cacheLock.lock();

  hashed_mem.emplace(key, value);
  cacheLock.unlock();

}

kv_cache_value_t arc_cache::lookup(kv_cache_key_t key) {
  cacheLock.lock();

  lookup_count++;
  // case 1
  if ((contains(T1, key) || contains(T2, key))) {
    hit_count++;

    T2.emplace_front(key);
  } else if (contains(B1, key)) {
    miss_count++;

    uint64_t y = std::max(B2.size() / B1.size(), (uint64_t)1);
    p = std::min(capacity, (p + y));
    replace(p, key);
    T2.emplace_front(key);
    hashed_mem.emplace(key, "");
  } else if (contains(B2, key)) {
    miss_count++;

    uint64_t y = std::max(B1.size() / B2.size(), (uint64_t)1);
    p = std::max(capacity, (p - y));
    replace(p, key);
    T2.emplace_front(key);
    hashed_mem.emplace(key, "");
  } else {
    miss_count++;

    if ((B1.size() + T1.size()) == capacity) {
      // case i
      if (T1.size() < capacity) {
        removeLRUentryFrom(B1);
        replace(p, key);
      } else {
        kv_cache_key_t lru_entry_key = removeLRUentryFrom(T1);
        removeFromCache(lru_entry_key);
      }
    } else if (((B1.size() + T1.size()) < capacity) &&
               ((B1.size() + T1.size() + B2.size() + T2.size()) >= capacity)) {
      // case ii
      if (((B1.size() + T1.size()) < capacity) &&
          ((B1.size() + T1.size() + B2.size() + T2.size()) >= capacity) ==
              2 * capacity) {
        removeLRUentryFrom(B2);
        replace(p, key);
      }
    }
    T1.emplace_front(key);
    hashed_mem.emplace(key, "");
  }
  std::string value = hashed_mem[key];
  cacheLock.unlock();
  return value;
}
