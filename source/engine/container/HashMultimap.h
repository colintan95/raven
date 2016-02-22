#ifndef HASHMULTIMAP_H_
#define HASHMULTIMAP_H_

#include "base_include.h"

#include "allocator/PoolAllocator.h"

#include <cstring>

//--------------------------------------------------
//
// HashMultimap
//
// An associative array that allows one key to correspond with many values
// These values are stored in buckets indexed by the key
//
// Checks for duplicate values in the bucket before insertion
//
// Operations do NOT use dynamic allocation
// Handles key collision using chaining, where entries with the same index are
// linked together via a linked list
//
// Bucket is implemented as a linked list
//
//--------------------------------------------------
template<typename Key, typename Value>
class HashMultimap {

// Forware declarations
public:
	class LocalIterator;

	friend class LocalIterator;
    
private:
    struct Node;
    struct Entry;

public:
	HashMultimap(int capacity) : m_NodeAlloc(capacity), m_EntryAlloc(capacity) {
		m_Table = MEM_NEW Entry*[capacity];
		m_Capacity = capacity;
		m_NodeCount = 0;

		memset((void*)m_Table, 0, sizeof(Entry*) * capacity);
	}

	~HashMultimap() {
		MEM_DELETE_ARR(m_Table);
		m_EntryAlloc.Clear();
		m_NodeAlloc.Clear();
	}

	// Inserts the key and value into the map
	//
	// Fails if there is already a key or value in the bucket
	void Insert(const Key& key, const Value& value) {
		ASSERT(m_NodeCount < m_Capacity);

		uint64_t hash = CalcHashForKey(key);
		int index = CalcIndexForHash(hash);

		Entry** prevPtr = &m_Table[index];
		Entry* entry = m_Table[index];

		bool newEntry = false;

		// Traverses through the entry linked list to check if the entry 
		// already exists
		if (entry != nullptr) {
			while (entry != nullptr && entry->hash != hash) {
				prevPtr = &(entry->next);
				entry = entry->next;
			}
		}

		// Insertion fails if there is already a value in the bucket
		if (entry != nullptr && HasValueInEntry(entry, value)) {
			ASSERT(0);
			return;
		}

		// Creates an entry if it does not exist yet
		if (entry == nullptr) {
			entry = m_EntryAlloc.Alloc();

			entry->hash = hash;
			entry->next = nullptr;

			// Links the last entry in the list to the new entry
			*prevPtr = entry;

			newEntry = true;
		}
		
		// Create the new value
		Node* node = m_NodeAlloc.Alloc();
		++m_NodeCount;
		node->value = value;

		// Inserts the new value into the bucket
		
		if (!newEntry) {
			entry->tail->next = node;
			node->prev = entry->tail;
            node->next = nullptr;

			entry->tail = node;
		}
		else {
			node->next = nullptr;
			node->prev = nullptr;

			entry->head = node;
			entry->tail = node;
		}

		ASSERT(entry->head != nullptr);
		ASSERT(entry->tail != nullptr);
		ASSERT(entry->hash == hash);
	}

	// Remove node with the specified key and value
	void Remove(const Key& key, const Value& value) {

		uint64_t hash = CalcHashForKey(key);
		int index = CalcIndexForHash(hash);

		Entry** prevPtr = &m_Table[index];
		Entry* entry = m_Table[index];

		if (entry != nullptr) {
			while (entry != nullptr && entry->hash != hash) {
				prevPtr = &entry->next;
				entry = entry->next;
			}
		}

		if (entry == nullptr) {
			// Return since there is no such entry with the hash
			return;
		}

		Node* node = entry->head;

		while (node != nullptr && node->value != value) {
			node = node->next;
		}

		if (node == nullptr) {
			return;
		}

		if (entry->head == node) {
			entry->head = node->next;
		}
		if (entry->tail == node) {
			entry->tail = node->prev;
		}

		if (node->prev != nullptr) {
			node->prev->next = node->next;
		}
		if (node->next != nullptr) {
			node->next->prev = node->prev;
		}

		m_NodeAlloc.Dealloc(node);
		--m_NodeCount;

		// Removes the entry if there are no more values in the bucket
		if (entry->head == nullptr && entry->tail == nullptr) {
			*prevPtr = entry->next;

			m_EntryAlloc.Dealloc(entry);
		}
	}

	// Removes all values from the bucket with the corresponding key
	void RemoveKey(const Key& key) {
		uint64_t hash = CalcHashForKey(key);
		int index = CalcIndexForHash(hash);

		Entry** prevPtr = &m_Table[index];
		Entry* entry = m_Table[index];

		if (entry != nullptr) {
			while (entry != nullptr && entry->hash != hash) {
				prevPtr = &entry->next;
				entry = entry->next;
			}
		}

		if (entry == nullptr) {
			// Return since there is no such entry with the hash
			return;
		}

		Node* node = entry->head;

		// Removes all the value nodes in the bucket
		while (node != nullptr) {
			Node* nextNode = node->next;

			m_NodeAlloc.Dealloc(node);
			--m_NodeCount;

			node = nextNode;
		}

        entry->head = nullptr;
        entry->tail = nullptr;

		// Removes the entry since there are no more values in it
		*prevPtr = entry->next;

		m_EntryAlloc.Dealloc(entry);
	}

	void Clear() {
		m_NodeAlloc.Clear();
		m_EntryAlloc.Clear();

		m_NodeCount = 0;

		memset((void*)m_Table, 0, sizeof(Entry*) * m_Capacity);
	}

	// Returns an iterator to the beginning of the bucket corresponding with 
	// key
	LocalIterator Begin(const Key& key) {
		uint64_t hash = CalcHashForKey(key);
        int index = CalcIndexForHash(hash);
        Entry* entry = m_Table[index];
        
        while (entry != nullptr && entry->hash != hash) {
            entry = entry->next;
        }

		LocalIterator it;
		
		if (entry != nullptr) {
			it.node = entry->head;
		}
		else {
			it.node = nullptr;
		}

		return it;
	}

	// Returns an iterator to the end of the bucket corresponding with key
	LocalIterator End(const Key& key) {
		/*uint64_t hash = CalcHashForKey(key);
        int index = CalcIndexForHash(hash);
        Entry* entry = m_Table[index];*/

		LocalIterator it;

		/*if (entry != nullptr) {
			it.node = entry->tail;
		}
		else {
			it.node = nullptr;
		}*/

		it.node = nullptr;

		return it;
	}

public:
	int GetSize() const {
		return m_NodeCount;
	}

private:

	// Tests if the entry already contains the value
	bool HasValueInEntry(Entry* entry, const Value& value) {
		if (entry != nullptr) {
			Node* node = entry->head;

			while (node != nullptr && node->value != value) {
				node = node->next;
			}

			if (node != nullptr && node->value == value) {
				return true;
			}
		}

		return false;
	}

	int CalcIndexForHash(uint64_t hash) {
		return (int)(hash % (uint64_t)(m_Capacity));
	}

	uint64_t CalcHashForKey(const Key& key) {
		Hash<Key> hashClass;

		return hashClass.HashFunc(key);
	}

private:	
	PoolAllocator<Node> m_NodeAlloc; // Allocates all nodes
	PoolAllocator<Entry> m_EntryAlloc; // Allocates all table entries. Each entry corresponds to a bucket

	Entry** m_Table; // m_Table[key] points to the head of the entry linked list

	int m_NodeCount; // Number of nodes of the table being used

	int m_Capacity; // Max number of nodes that the table can use

private:
	// Stores the value
	struct Node {
		Node* prev;
		Node* next;
		Value value;
	};

	struct Entry {
		// Considered empty if head points to a nullptr
		Node* head;
		Node* tail;
		Entry* next;
		uint64_t hash; 
	};

public:
	class LocalIterator {
		friend class HashMultimap;

	public:
		LocalIterator() {
			node = nullptr;
		}

		Value& GetValue() {
			ASSERT(node != nullptr);
			return node->value;
		}

		LocalIterator& operator++() {
			if (node != nullptr) {
				node = node->next;
			}
            
            return *this;
		}

		LocalIterator operator++(int) {
			LocalIterator temp = *this;

			if (node != nullptr) {
				node = node->next;
			}

            return temp;
		}

		/*friend bool operator==(const LocalIterator& it1, const LocalIterator& it2);

		friend bool operator!=(const LocalIterator& it1, const LocalIterator& it2);*/

		bool operator==(const LocalIterator& it) {
			return node == it.node;
		}

		bool operator!=(const LocalIterator& it) {
			return node != it.node;
		}

	private:
		HashMultimap::Node* node;
	};

};

#endif