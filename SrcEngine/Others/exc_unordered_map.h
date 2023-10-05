#pragma once
#include <Essentials.h>
#include <mutex>
#include <EASTL/unordered_map.h>

template <class Key,
    class T,
    class Hash = eastl::hash<Key>,
    class Pred = eastl::equal_to<Key>,
    class Allocator = EASTLAllocatorType >
class exc_unordered_map
{
public:
    exc_unordered_map()
    {
        map.clear();
    };

    void Access(std::function<void(eastl::unordered_map<Key, T, Hash, Pred, Allocator>&)> implements)
    {
        std::lock_guard<std::mutex> lock(mtx);
        implements(map);
    };

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        map.clear();
    };

private:
    eastl::unordered_map<Key, T, Hash, Pred, Allocator> map;
    std::mutex mtx;
};