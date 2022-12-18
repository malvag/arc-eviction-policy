
#include "arc.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "absl/log/initialize.h"
#include "absl/log/log.h"

using namespace absl;
int main() {
    // absl::InitializeLog();
    arc_cache cache(3);
    
    cache.lookup("Hello", "Hallo");
	cache.lookup("World", "Welt");
	std::cout << cache.lookup("World","(null)") << "\n";
	cache.lookup("Cache", "Cache");
	cache.lookup("Replace", "Ersetzen");
    std::cout << cache.hashed_mem.size() << "\n" ;
    for ( auto& a : cache.hashed_mem){
        std::cout << a.first << a.second << "\n" ;
    }


}

arc_cache::arc_cache(uint64_t cap){
    p = 0;
    capacity = cap;
}

/*
If (T1 is not empty) and ((T1 lenght exceeds the target p) or (x is in B2 and T1
lenght == p)) Delete the LRU page in T1 (also remove it from the cache), and
move it to MRU position in B1. else Delete the LRU page in T2 (also remove it
from the cache), and move it to MRU position in B2. endif
*/

bool contains(std::deque<kv_cache_key_t>& v, kv_cache_key_t key) {
    if (std::find(v.begin(), v.end(), key) != v.end()) return true;
    return false;
}

kv_cache_key_t removeLRUentryFrom(std::deque<kv_cache_key_t>& v) {
    if(v.size()){
        kv_cache_key_t key = v.back();
        v.pop_back();
        return key;
    }
    return "(null)";
}

void arc_cache::removeFromCache(kv_cache_key_t key) {
    auto it = hashed_mem.find(key);
    if(it != hashed_mem.end())
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

kv_cache_value_t arc_cache::lookup(kv_cache_key_t key, kv_cache_value_t value) {
    lookup_count++;
    // case 1
    if ((contains(T1, key) || contains(T2, key))) {
        T2.emplace_front(key);
        hit_count++;
    } else if (contains(B1, key)) {
        uint64_t y = std::max(B2.size() / B1.size(), (uint64_t)1);
        p = std::min(capacity, (p + y));
        replace(p, key);
        T2.emplace_front(key);
        hashed_mem.insert({key, value});
        miss_count++;
    } else if (contains(B2, key)) {
        uint64_t y = std::max(B1.size() / B2.size(), (uint64_t)1);
        p = std::max(capacity, (p - y));
        replace(p, key);
        T2.emplace_front(key);
        hashed_mem.insert({key, value});
        miss_count++;
    } else {
        // if ((key_in_t1 || key_in_t2 || key_in_b1 || key_in_b2)) {
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
                   ((B1.size() + T1.size() + B2.size() + T2.size()) >=
                    capacity)) {
            // case ii
            if (((B1.size() + T1.size()) < capacity) &&
                ((B1.size() + T1.size() + B2.size() + T2.size()) >= capacity) ==
                    2 * capacity) {
                removeLRUentryFrom(B2);
                replace(p, key);
            }
        }
        T1.emplace_front(key);
        hashed_mem.insert({key, value});
    }
    return hashed_mem[key];
}
