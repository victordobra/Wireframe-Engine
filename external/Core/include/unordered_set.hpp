#pragma once

#include "vector.hpp"
#include "Hash.hpp"

namespace wfe {
    template<class Value>
    class unordered_set {
    public:
        class node {
        public:
            Value value{};
            node* next{nullptr};

            constexpr node() = default;
            constexpr node(const node&) = default;
            constexpr node(node&&) noexcept = default;
            constexpr node(Value v, node* next = nullptr) : value(v), next(next) { 

            }

            node& operator=(const node&) = default;
            node& operator=(node&&) noexcept = default;

            constexpr ~node() {
                delete next;
            }
        };

        constexpr unordered_set() : bucketCount(16), allocatedBuckets(0), buckets(new node*[bucketCount]) { 
            for(size_t i = 0; i < bucketCount; i++)
                buckets[i] = nullptr;
        }
        constexpr unordered_set(const unordered_set& other) : bucketCount(other.bucketCount), allocatedBuckets(other.allocatedBuckets), buckets(new node*[other.bucketCount]) {
            //Copy all of the buckets
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
        constexpr unordered_set(unordered_set&& other) noexcept : bucketCount(other.bucketCount), allocatedBuckets(other.allocatedBuckets), buckets(other.buckets) {
            //Unasign the other map's buckets
            other.buckets = nullptr;
            other.bucketCount = 0;
            other.allocatedBuckets = 0;
        }
        constexpr unordered_set(size_t bucketCount) : bucketCount(bucketCount), allocatedBuckets(0), buckets(new node*[bucketCount]) {
            for(size_t i = 0; i < bucketCount; i++)
                buckets[i] = nullptr;
        }

        unordered_set& operator=(const unordered_set& other) {
            //If the other bucket is the same as this bucket, exit the function
            if(&other == this)
                return *this;

            //Delete the old buckets
            for(size_t i = 0; i < bucketCount; i++)
                delete buckets[i];
            delete[] buckets;
            
            //Create the new buckets
            bucketCount = other.bucketCount;
            allocatedBuckets = other.allocatedBuckets;
            buckets = new node*[bucketCount];

            //Copy all of the buckets
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
        unordered_set& operator=(unordered_set&& other) noexcept {
            //Delete the old buckets
            for(size_t i = 0; i < bucketCount; i++)
                delete buckets[i];
            delete[] buckets;

            //Move the buckets over to this map
            bucketCount = other.bucketCount;
            allocatedBuckets = other.allocatedBuckets;
            buckets = other.buckets;

            //Unasign the other map's buckets
            other.buckets = nullptr;
            other.bucketCount = 0;
            other.allocatedBuckets = 0;

            return *this;
        }
    
        Value& insert(const Value& value) {
            //Find the correct bucket for the key
            size_t hash = GetHashCode(value);
            size_t index = hash % bucketCount;

            node** currentNode = buckets + index;

            //If the first node doesn't exist, increment the allocated buffer count and create it
            if(*currentNode == nullptr) {
                allocatedBuckets++;

                *currentNode = new node(value);

                return (*currentNode)->value;
            }

            //Loop through every node
            while(*currentNode != nullptr) {
                //If the node's key is equal to this key, return the value
                if(value == (*currentNode)->value) {
                    return (*currentNode)->value;
                }
                currentNode = &(*currentNode)->next;
            }

            //The element with the specified node doesn't exit, create it
            if(currentNode == buckets + index) {
                allocatedBuckets++;

                if(allocatedBuckets >= (bucketCount >> 2) * 3)
                    reserve(bucketCount * 2);
            }

            *currentNode = new node(value);

            return (*currentNode)->value;
        }
        unordered_set& remove(const Value& value) {
            size_t hash = GetHashCode(value);
            size_t index = hash % bucketCount;

            node** currentNode = buckets + index;
            
            while(*currentNode != nullptr) {
                if((*currentNode)->value == value) {
                    node* newNode = (*currentNode)->next;
                    delete *currentNode;
                    *currentNode = newNode;
                } else
                    currentNode = &((*currentNode)->next);
            }

            return *this;
        }
        unordered_set& reserve(size_t newBucketCount) {
            //Assert that the new bucket count must be higher than 0
            assert((newBucketCount) && "The new bucket count must be higher than 0!");

            //Create the new buckets
            node** newBuckets = new node*[newBucketCount];
            node*** bucketMap = new node**[newBucketCount];
            size_t newAllocatedBuckets = 0;

            for(size_t i = 0; i < newBucketCount; i++) {
                newBuckets[i] = nullptr;
                bucketMap[i] = newBuckets + i;
            }

            //Copy everything over from the old buckets
            node* oldNode;

            for(size_t i = 0; i < bucketCount; i++) {
                node* oldNode = buckets[i];

                while (oldNode != nullptr) {
                    node* newNode = new node(oldNode->value);
                    size_t newHash = GetHashCode(newNode->value);
                    size_t newIndex = newHash % newBucketCount;

                    if (newBuckets[newIndex] == nullptr)
                        newAllocatedBuckets++;
                    
                    *(bucketMap[i]) = newNode;
                    bucketMap[i] = &(newNode->next);
                }
            }

            //Delete the old buckets and replace them with the new onces
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
        size_t count(const Value& value) const {
            size_t hash = GetHashCode(value);
            size_t index = hash % bucketCount;

            size_t count = 0;
            node* currentNode = buckets[index];

            while(currentNode != nullptr) {
                count++;
                currentNode = currentNode->next;
            }

            return count;
        }
        size_t GetBucketCount() const { 
            return bucketCount; 
            }
        
        constexpr ~unordered_set() {
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