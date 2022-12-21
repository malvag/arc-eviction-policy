#include "arc.h"

int main() {
    arc_cache cache(5000);
    
    if(cache.lookup("Hello") == "")
        cache.put("Hello","World");
	if(cache.lookup("World") == "")
        cache.put("World","Welt");
	if(cache.lookup("Cache") == "")
        cache.put("Cache","Cache");
	if(cache.lookup("Replace") == "")
        cache.put("Replace","Antikastasi");
    for(int i = 0 ; i < 100000; i++){
        int k = i % 1000;
        std::string key = "key" + std::to_string(k);
        std::string value = "value"+ std::to_string(k);
        if(cache.lookup(key) == "")
            cache.put(key,value);
    }
    // std::cout << cache.hashed_mem.size() << "\n" ;
    // for ( auto& a : cache.hashed_mem){
    //     std::cout << a.first << a.second << "\n" ;
    // }
    cache.get_stats();

    return 0;

}