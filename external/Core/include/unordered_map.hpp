#pragma once

#include "vector.hpp"
#include "Hash.hpp"

namespace mge {
    template<class Key, class Value>
    class unordered_map {
    public:
        class node {
        public:
            Key key;
            Value value;
            node* next{nullptr};

            node() = default;
            node(const node&) = default;
            node(node&&) noexcept = default;
            node(Key k) : key(k) { }
            node(Key k, Value v, node* next = nullptr) : key(k), value(v), next(next) { }

            node& operator=(const node&) = default;
            node& operator=(node&&) noexcept = default;

            ~node() {
                delete next;
            }
        };

        unordered_map() : bucketCount(16), allocatedBuckets(0), buckets(new node*[bucketCount]) { 
            for(size_t i = 0; i < bucketCount; i++)
                buckets[i] = nullptr;
        }
        unordered_map(const unordered_map& other) : bucketCount(other.bucketCount), allocatedBuckets(other.allocatedBuckets), buckets(new node*[bucketCount]) {
            // Copy all of the buckets
            for(size_t i = 0; i < bucketCount; i++) {
                buckets[i] = nullptr;
                node** thisNode = buckets + i;
                node* otherNode = other.buckets[i];

                while(otherNode != nullptr) {
                    *thisNode = new node();
                    *thisNode->key = otherNode->key;
                    *thisNode->value = otherNode->value;

                    thisNode = &(*thisNode)->next;
                    otherNode = otherNode->next;
                }
            }
        }
        unordered_map(unordered_map&& other) noexcept : bucketCount(other.bucketCount), allocatedBuckets(other.allocatedBuckets), buckets(other.buckets) {
            // Unasign the other map's buckets
            other.buckets = nullptr;
            other.bucketCount = 0;
            other.allocatedBuckets = 0;
        }
        unordered_map(size_t bucketCount) : bucketCount(bucketCount), allocatedBuckets(0), buckets(new node*[bucketCount]) {
            for(size_t i = 0; i < bucketCount; i++)
                buckets[i] = nullptr;
        }

        unordered_map& operator=(const unordered_map& other) {
            // If the other bucket is the same as this bucket, exit the function
            if(&other == this)
                return *this;

            // Delete the old buckets
            for(size_t i = 0; i < bucketCount; i++)
                delete buckets[i];
            delete[] buckets;
            
            //Create the new buckets
            bucketCount = other.bucketCount;
            allocatedBuckets = other.allocatedBuckets;
            buckets = new node*[bucketCount];

            // Copy all of the buckets
            for(size_t i = 0; i < bucketCount; i++) {
                buckets[i] = nullptr;
                node** thisNode = buckets + i;
                node* otherNode = other.buckets[i];

                while(otherNode != nullptr) {
                    *thisNode = new node();
                    *thisNode->key = otherNode->key;
                    *thisNode->value = otherNode->value;

                    thisNode = &(*thisNode)->next;
                    otherNode = otherNode->next;
                }
            }

            return *this;
        }
        unordered_map& operator=(unordered_map&& other) noexcept {
            // Delete the old buckets
            for(size_t i = 0; i < bucketCount; i++)
                delete buckets[i];
            delete[] buckets;

            // Move the buckets over to this map
            bucketCount = other.bucketCount;
            allocatedBuckets = other.allocatedBuckets;
            buckets = other.buckets;

            // Unasign the other map's buckets
            other.buckets = nullptr;
            other.bucketCount = 0;
            other.allocatedBuckets = 0;

            return *this;
        }
    
        Value& operator[](const Key& key) {
            // Find the correct bucket for the key
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            node** currentNode = buckets + index;

            // If the first node doesn't exist, increment the allocated buffer count and create it
            if(*currentNode == nullptr) {
                allocatedBuckets++;

                *currentNode = new node(key);

                return (*currentNode)->value;
            }

            // Loop through every node
            while(*currentNode != nullptr) {
                //If the node's key is equal to this key, return the value
                if(key == (*currentNode)->key) {
                    return (*currentNode)->value;
                }
                currentNode = &(*currentNode)->next;
            }

            // The element with the specified node doesn't exit, create it
            if(currentNode == buckets + index) {
                allocatedBuckets++;

                if(allocatedBuckets >= (bucketCount >> 2) * 3)
                    reserve(bucketCount * 2);
            }

            *currentNode = new node(key);

            return (*currentNode)->value;
        } 
        Value& operator[](Key&& key) {
            // Find the correct bucket for the key
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            node** currentNode = buckets + index;

            // If the first node doesn't exist, increment the allocated buffer count and create it
            if(*currentNode == nullptr) {
                allocatedBuckets++;

                *currentNode = new node(key);

                return (*currentNode)->value;
            }

            //Loop through every node
            while(*currentNode != nullptr) {
                //If the node's key is equal to this key, return the value
                if(key == (*currentNode)->key) {
                    return (*currentNode)->value;
                }
                currentNode = &(*currentNode)->next;
            }

            // The element with the specified node doesn't exit, create it
            if(currentNode == buckets + index) {
                allocatedBuckets++;

                if(allocatedBuckets >= (bucketCount >> 2) * 3)
                    reserve(bucketCount * 2);
            }

            *currentNode = new node(key);

            return (*currentNode)->value;
        }
        Value& insert(const Key& key, const Value& value) {
            // Find the correct bucket for the key
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            node** currentNode = buckets + index;

            // If the first node doesn't exist, increment the allocated buffer count and create it
            if(*currentNode == nullptr) {
                allocatedBuckets++;

                *currentNode = new node(key);
                (*currentNode)->value = value;

                return (*currentNode)->value;
            }

            // Loop through every node
            while(*currentNode != nullptr) {
                //If the node's key is equal to this key, return the value
                if(key == (*currentNode)->key) {
                    return (*currentNode)->value;
                }
                currentNode = &(*currentNode)->next;
            }

            // The element with the specified node doesn't exit, create it
            if(currentNode == buckets + index) {
                allocatedBuckets++;

                if(allocatedBuckets >= (bucketCount >> 2) * 3)
                    reserve(bucketCount * 2);
            }

            *currentNode = new node(key);
            (*currentNode)->value = value;

            return (*currentNode)->value;
        }
        Value& insert_or_assign(const Key& key, const Value& value) {
            return insert(key, value);
        }
        unordered_map& remove(const Key& key) {
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            node** currentNode = buckets + index;
            
            while(*currentNode != nullptr) {
                if((*currentNode)->key == key) {
                    node* newNode = (*currentNode)->next;
                    delete *currentNode;
                    *currentNode = newNode;
                } else
                    currentNode = &((*currentNode)->next);
            }
        }
        unordered_map& reserve(size_t newBucketCount) {
            // Assert that the new bucket count must be higher than 0
            assert((newBucketCount) && "The new bucket count must be higher than 0!");

            //Create the new buckets
            node** newBuckets = new node*[newBucketCount];
            node*** bucketMap = new node**[newBucketCount];
            size_t newAllocatedBuckets = 0;

            for(size_t i = 0; i < newBucketCount; i++) {
                newBuckets[i] = nullptr;
                bucketMap[i] = newBuckets + i;
            }

            // Copy everything over from the old buckets
            node* oldNode;

            for(size_t i = 0; i < bucketCount; i++) {
                node* oldNode = buckets[i];

                while (oldNode != nullptr) {
                    node* newNode = new node(oldNode->key, oldNode->value);
                    size_t newHash = GetHashCode(newNode->key);
                    size_t newIndex = newHash % newBucketCount;

                    if (newBuckets[newIndex] == nullptr)
                        newAllocatedBuckets++;
                    
                    *(bucketMap[i]) = newNode;
                    bucketMap[i] = &(newNode->next);
                }
            }

            // Delete the old buckets and replace them with the new onces
            for(size_t i = 0; i < bucketCount; i++)
                delete buckets[i];
            delete[] buckets;

            buckets = newBuckets;
            bucketCount = newBucketCount;
            allocatedBuckets = newAllocatedBuckets;

            delete[] bucketMap;

            return *this;
        }

        size_t count() const {
            size_t count = 0;
            for(size_t i = 0; i < bucketCount; i++) {
                node* currentNode = buckets[i];

                while(currentNode != nullptr) {
                    count++;
                    currentNode = currentNode->next;
                }
            }

            return count;
        }
        size_t count(const Key& key) const {
            size_t hash = GetHashCode(key);
            size_t index = hash % bucketCount;

            size_t count = 0;
            node* currentNode = buckets[index];

            while(currentNode != nullptr) {
                if(key == currentNode->key)
                    ++count;
                currentNode = currentNode->next;
            }

            return count;
        }
        node** data() {
            return buckets;
        }
        const node* data() const {
            return buckets;
        }
        size_t bucket_count() const { 
            return bucketCount; 
        }
        
        ~unordered_map() {
            for(size_t i = 0; i < bucketCount; i++)
                delete buckets[i];
            delete[] buckets;
        }
    private:
        size_t bucketCount;
        size_t allocatedBuckets;
        node** buckets;
    };
}