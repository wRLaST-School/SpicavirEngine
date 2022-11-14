#pragma once
#include <Essentials.h>
#include <unordered_map>
#include <mutex>

template <class Key,
    class T,
    class Hash = std::hash<Key>,
    class Pred = std::equal_to<Key>,
    class Allocator = std::allocator<std::pair<const Key, T> > >
class exc_unordered_map
{
public:
    exc_unordered_map()
    {
        map.clear();
    };

    void Access(std::function<void(unordered_map<Key, T, Hash, Pred, Allocator>&)> implements)
    {
        std::lock_guard<std::mutex> lock(mtx);
        implements(map);
    };

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        map.clear();
    };

private:
    unordered_map<Key, T, Hash, Pred, Allocator> map;
    std::mutex mtx;
};