#pragma once
/*****************************************************************//**
 * @file   exc_unordered_map.h
 * @brief  unordered_mapの排他制御を行うヘッダー
 * 
 * @author Wrelf
 *********************************************************************/
#include <Essentials.h>
#include <mutex>

template <class Key,
    class T,
    class Hash = eastl::hash<Key>,
    class Pred = eastl::equal_to<Key>,
    class Allocator = EASTLAllocatorType >
/**
 * @brief unordered_mapの排他制御を行うラッパークラス
 */
class exc_unordered_map
{
public:
    exc_unordered_map()
    {
        map.clear();
    };

    /**
     * @brief ロックを取得してマップにアクセスする関数
     * 
     * @param implements mapを利用した処理の関数ポインタ
     */
    void Access(std::function<void(eastl::unordered_map<Key, T, Hash, Pred, Allocator>&)> implements)
    {
        std::lock_guard<std::mutex> lock(mtx);
        implements(map);
    };

    /**
     * @brief マップのclear関数を呼ぶ
     * 
     */
    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        map.clear();
    };

private:
    eastl::unordered_map<Key, T, Hash, Pred, Allocator> map;
    std::mutex mtx;
};