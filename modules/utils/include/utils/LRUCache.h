#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <functional>
#include <future>
#include <memory>
#include <unordered_map>
#include <utils/Marcos.h>
#include <utils/MessageLoopImpl.h>
#include <utils/Time.h>
#include <optional>

namespace laya
{
template <typename K, typename V> class LRUCache
{
    DISALLOW_COPY_AND_ASSIGN(LRUCache)
  private:
    struct Node
    {
        K key;
        V value;
        Node *prev;
        Node *next;
        Node(const K &k, const V &v) : key(k), value(v), prev(nullptr), next(nullptr)
        {
        }
    };

    size_t capacity;
    std::unordered_map<K, Node *> cache;
    Node *head;
    Node *tail;

    void addNode(Node *node)
    {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(Node *node)
    {
        Node *prev = node->prev;
        Node *next = node->next;
        prev->next = next;
        next->prev = prev;
    }

    void moveToHead(Node *node)
    {
        removeNode(node);
        addNode(node);
    }

    Node *popTail()
    {
        Node *res = tail->prev;
        removeNode(res);
        return res;
    }

  public:
    explicit LRUCache(size_t capacity) : capacity(capacity)
    {
        // 初始化双向链表
        head = new Node(K(), V());
        tail = new Node(K(), V());
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache()
    {
        clear();
        delete head;
        delete tail;
    }

    std::optional<V> get(const K &key)
    {
        if (cache.find(key) == cache.end())
        {
            return std::nullopt;
        }
        Node *node = cache[key];
        moveToHead(node);
        return node->value;
    }

    void put(const K &key, const V &value, std::function<void(const K &key, const V &value)> onEvict = nullptr)
    {
        if (cache.find(key) != cache.end())
        {
            Node *node = cache[key];
            node->value = value;
            moveToHead(node);
        }
        else
        {
            Node *node = new Node(key, value);
            cache[key] = node;
            addNode(node);
            while (cache.size() > capacity)
            {
                Node *tail = popTail();
                if (onEvict)
                {
                    onEvict(tail->key, tail->value);
                }
                cache.erase(tail->key);
                delete tail;
            }
        }
    }

    void clear()
    {
        while (head->next != tail)
        {
            Node *temp = head->next;
            removeNode(temp);
            cache.erase(temp->key);
            delete temp;
        }
    }

    void iterate(std::function<void(const K &key, const V &value)> callback)
    {
        Node *current = head->next;
        while (current != tail)
        {
            callback(current->key, current->value);
            current = current->next;
        }
    }

    void clearAndSetCapacity(size_t newCapacity)
    {
        clear();
        capacity = newCapacity;
    }

    size_t getCapacity() const
    {
        return capacity;
    }

    size_t size() const
    {
        return cache.size();
    }
};
} // namespace laya
#endif