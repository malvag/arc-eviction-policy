#include "arc.h"
#include <absl/log/initialize.h>

int main() {
    arc_cache cache(5000);
    
    cache.lookup("Hello", "Hallo");
	cache.lookup("World", "Welt");
	cache.lookup("Cache", "Cache");
	cache.lookup("Replace", "Ersetzen");
    for(int i = 0 ; i < 100000; i++){
        int k = i % 1000;
        std::string key = "key" + std::to_string(k);
        std::string value = "value"+ std::to_string(k);
        cache.lookup(key, value);
    }
    // std::cout << cache.hashed_mem.size() << "\n" ;
    // for ( auto& a : cache.hashed_mem){
    //     std::cout << a.first << a.second << "\n" ;
    // }
    // cache.get_stats();

    return 0;

}