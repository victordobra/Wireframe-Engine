#pragma once

#include "Hash.hpp"
#include "pair.hpp"
#include "IO.hpp"

namespace wfe {
    template<class Key, class Value>
    class unordered_map {
    public:
        struct node {
            Key key;
            Value value;
            node* next{nullptr};

            constexpr node() = default;
            constexpr node(const node&) = default;
            constexpr node(node&&) noexcept = default;
            constexpr node(Key k) : key(k) { 

            }
            constexpr node(Key k, Value v, node* next = nullptr) : key(k), value(v), next(next) { 

            }

            node& operator=(const node&) = default;
            node& operator=(node&&) noexcept = default;

            constexpr ~node() {
                delete next;
            }
        };

        constexpr unordered_map() : bucketCount(16), allocatedBucketCount(0), buckets(new node[bucketCount]) {

        }
        constexpr unordered_map(const unordered_map& other) : bucketCount(other.bucketCount), allocatedBucketCount(other.allocatedBuckets), buckets(new node[bucketCount]) {
            // Copy every bucket
            for(size_t i = 0; i < bucketCount; ++i) {
                // Copy the linked list at every bucket
                node* currentNode = buckets + i;
                const node* otherCurrentNode = buckets + i;

                while(otherCurrentNode->next) {
                    *currentNode = *otherCurrentNode;
                    currentNode->next = new node();

                    currentNode = currentNode->next;
                    otherCurrentNode = otherCurrentNode->next;
                }
            }
        }
        constexpr unordered_map(unordered_map&& other) noexcept : bucketCount(other.bucketCount), allocatedBucketCount(other.allocatedBucketCount), buckets(other.buckets) {
            // Unkink the buckets from the other map
            other.buckets = nullptr;
        }
        constexpr unordered_map(size_t bucketCount) : bucketCount(bucketCount), allocatedBucketCount(0), buckets(new node[bucketCount]) {

        }

        unordered_map& operator=(const unordered_map& other) {
            if(&other == this)
                return *this;
            
            // Delete the old list
            delete[] buckets;

            // Set the new values
            bucketCount = other.bucketCount;
            allocatedBucketCount = other.allocatedBucketCount;
            buckets = new node[bucketCount];

            // Copy every bucket
            for(size_t i = 0; i < bucketCount; ++i) {
                // Copy the linked list at every bucket
                node* currentNode = buckets + i;
                const node* otherCurrentNode = buckets + i;

                while(otherCurrentNode->next) {
                    *currentNode = *otherCurrentNode;
                    currentNode->next = new node();

                    currentNode = currentNode->next;
                    otherCurrentNode = otherCurrentNode->next;
                }
            }

            return *this;
        }
        unordered_map& operator=(unordered_map&& other) noexcept {
            // Delete the old list
            delete[] buckets;

            // Set the new values
            bucketCount = other.bucketCount;
            allocatedBucketCount = other.allocatedBucketCount;
            buckets = other.buckets;

            // Unkink the buckets from the other map
            other.buckets = nullptr;

            return *this;
        }

        Value& operator[](const Key& key) {
            // Find the appropriate index for the key
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            // Loop through the linked list and try to find a node with the specified key
            node* currentNode = buckets + index;
            while(currentNode->next) {
                if(currentNode->key == key)
                    return currentNode->value;
                currentNode = currentNode->next;
            }

            return *(insert(key, {}).val1);
        }

        pair<Value*, bool8_t> insert(const Key& key, const Value& value) {
            return insert({key, value});
        }
        pair<Value*, bool8_t> insert(const pair<Key, Value>& val) {
            // Find the appropriate index for the key
            size_t hash = GetHashCode(val.val1);
            size_t index = hash % bucketCount;

            // Loop through the linked list and try to find a node with the specified key
            node* currentNode = buckets + index;
            while(currentNode->next) {
                if(currentNode->key == val.val1)
                    return { &(currentNode->value), false };
                currentNode = currentNode->next;
            }

            if(currentNode == buckets + index) {
                ++allocatedBucketCount;

                if(allocatedBucketCount * 4 >= bucketCount * 3)
                    reserve(bucketCount * 2);
            }

            currentNode->key = val.val1;
            currentNode->value = val.val2;
            currentNode->next = new node();

            return { &(currentNode->value), true };
        }
        pair<Value*, bool8_t> insert_or_assign(const Key& key, Value&& val) {
            // Find the appropriate index for the key
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            // Loop through the linked list and try to find a node with the specified key
            node* currentNode = buckets + index;
            while(currentNode->next) {
                if(currentNode->key == key) {
                    currentNode->value = val;
                    return { &(currentNode->value), false };
                }
                currentNode = currentNode->next;
            }

            // Insert the value and return an iterator to it
            return insert({key, val});
        }
    
        void reserve(size_t newBucketCount) {
            // Save the previous variables
            size_t oldBucketCount = bucketCount;
            node* oldBuckets = buckets;

            // Set the new values
            bucketCount = newBucketCount;
            allocatedBucketCount = 0;
            buckets = new node[newBucketCount];

            // Loop through every old bucket
            for(size_t i = 0; i < oldBucketCount; ++i) {
                node* currentNode = oldBuckets + i;

                while(currentNode->next) {
                    insert({currentNode->key, currentNode->value});
                    currentNode = currentNode->next;
                }
            }
        }

        Value& at(const Key& key) {
            // Find the appropriate index for the key
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            // Loop through the linked list and try to find a node with the specified key
            node* currentNode = buckets + index;
            while(currentNode->next) {
                if(currentNode->key == key)
                    return currentNode->value;
                currentNode = currentNode->next;
            }

            console::OutFatalError("Failed to find an item with the specified key!", 1);
        }
        const Value& at(const Key& key) const{
            // Find the appropriate index for the key
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            // Loop through the linked list and try to find a node with the specified key
            node* currentNode = buckets + index;
            while(currentNode->next) {
                if(currentNode->key == key)
                    return currentNode->value;
                currentNode = currentNode->next;
            }

            console::OutFatalError("Failed to find an item with the specified key!", 1);
        }

        node* begin() { 
            return buckets; 
        }
        node* end()   { 
            return buckets + bucketCount;
        }
        const node* begin() const { 
            return buckets; 
        }
        const node* end()   const { 
            return buckets + bucketCount; 
        }
    
        bool8_t empty() const {
            // If at least one of the buckets has at least one element, the map isn't empty
            for(size_t i = 0; i < bucketCount; ++i) {
                if(buckets[i].next)
                    return false;
            }
            return true;
        }
        size_t size() const {
            size_t size = 0;

            // Loop through every bucket
            for(size_t i = 0; i < bucketCount; ++i) {
                const node* currentNode = buckets + i;

                // Increment the size for every element in the linked list
                while(currentNode->next) {
                    ++size;
                    currentNode = currentNode->next;
                }
            }

            return size;
        }
        size_t count(const Key& key) const {
            // Get the bucket of the specific key
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            size_t count = 0;
            const node* currentNode = buckets + index;
            
            // Increment the count for every node with the same key
            while(currentNode->next) {
                if(currentNode->key == key)
                    ++count;
                currentNode = currentNode->next;
            }

            return count;
        }
        size_t bucket_count() const { 
            return bucketCount; 
        }

        constexpr ~unordered_map() {
            delete[] buckets;
        }
    private:
        size_t bucketCount;
        size_t allocatedBucketCount;
        node* buckets;
    };
}