#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "base_include.h"

#include "allocator/PoolAllocator.h"

//--------------------------------------------------
//
// HashMap
//
// An associative container that stores a key and value pair. In this map, each
// key can only correspond to ONE value. Use a HashMultimap if one key needs 
// to contain multiple values
//
// An entry into the map contains of 1) Key and 2) Value. The key can be 
//
// There are two linked list that are internally maintained: 1) Map List and 2)
// Collision List
//
// Map List: All entries (key and value pair) are added to the this list
// can be traversed with an iterator
//
// Entries with the same index are placed in the repeated index list
//
//--------------------------------------------------
template<typename Key, typename Value>
class HashMap {

// Forward declarations
public:
	class Iterator;
	friend class Iterator;

private:
	struct Entry;

public:
    HashMap(int capacity): m_EntryAlloc(capacity) {
		m_Table = MEM_NEW Entry*[capacity];
		m_Capacity = capacity;
		m_EntryCount = 0;

		m_ListHead = nullptr;
		m_ListTail = nullptr;

		memset((void*)m_Table, 0, sizeof(Entry*) * capacity);
	}

	~HashMap() {
		MEM_DELETE_ARR(m_Table);
		m_EntryAlloc.Clear();
	}

	// Inserts the key and value into the map
	//
	// Fails if the key is already inside the map
	void Insert(const Key& key, const Value& value) {
		ASSERT(m_EntryCount < m_Capacity);

		uint64_t hash = CalcHashForKey(key);
		int index = CalcIndexForHash(hash);

		// Refers to the previous pointer so that it can modified during
		// insertion or removal in a singly-linked list
		Entry** prevPtr = &m_Table[index];

		Entry* entry = m_Table[index];

		// Traverses through the entry linked list to check if the entry 
		// already exists
		if (entry != nullptr) {
			while (entry != nullptr && entry->hash != hash) {
				prevPtr = &(entry->next);
				entry = entry->next;
			}
		}

		// Insertion fails if the key is already in the map
		if (entry != nullptr && entry->hash == hash) {
			ASSERT(0);
			return;
		}

		// Creates an entry
		entry = m_EntryAlloc.Alloc();
		++m_EntryCount;

		entry->value = value;
		entry->hash = hash;
		entry->next = nullptr;

		// Links the last entry in the repeated index list or the table pointer to the new entry
		*prevPtr = entry;

		// Adds the new entry into the map linked list
		if (m_ListHead == nullptr) {
			m_ListHead = entry;
			m_ListTail = entry;
			entry->nextValue = nullptr;
			entry->prevValue = nullptr;
		}
		else {
			entry->prevValue = m_ListTail;
			entry->nextValue = nullptr;

			m_ListTail->nextValue = entry;
			m_ListTail = entry;
		}

		ASSERT(entry->hash == hash);
	}

	void Remove(const Key& key) {
		uint64_t hash = CalcHashForKey(key);
		int index = CalcIndexForHash(hash);

		// Refers to the previous pointer so that it can modified during
		// insertion or removal in a singly-linked list
		Entry** prevPtr = &m_Table[index];

		Entry* entry = m_Table[index];
		
		// Traverses through the entry linked list to find the entry with the
		// specified key
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

		// Removes the entry from the repeated index list
		*prevPtr = entry->next;

		// Removes the entry from the map linked list
		if (m_ListHead == entry) {
			m_ListHead = entry->nextValue;
		}
		if (m_ListTail == entry) {
			m_ListTail = entry->nextValue;
		}
		if (entry->prevValue != nullptr) {
			entry->prevValue->nextValue = entry->nextValue;
		}
		if (entry->nextValue != nullptr) {
			entry->nextValue->prevValue = entry->prevValue;
		}

		m_EntryAlloc.Dealloc(entry);
		--m_EntryCount;
	}

	// Returns an iterator to the entry with the specified key
	//
	// If there is not such key, returns an iterator to End()
	Iterator Find(const Key& key) {
		uint64_t hash = CalcHashForKey(key);
		int index = CalcIndexForHash(hash);

		Entry* entry = m_Table[index];

		if (entry != nullptr) {
			while (entry != nullptr && entry->hash != hash) {
				entry = entry->next;
			}
		}

		if (entry != nullptr) {
			Iterator it;

			it.entry = entry;

			return it;
		}
		else {
			return End();
		}
	}

	void Clear() {
		m_EntryAlloc.Clear();

		m_EntryCount = 0;

		m_ListHead = nullptr;
		m_ListTail = nullptr;

		memset((void*)m_Table, 0, sizeof(Entry*) * m_Capacity);
	}

	Iterator Begin() {
		Iterator it;
		
		it.entry = m_ListHead;

		return it;
	}

	Iterator End() {
		Iterator it;

		it.entry = nullptr;

		return it;
	}

	bool IsFull() const {
		return m_EntryCount == m_Capacity;
	}

	bool IsEmpty() const {
		return m_EntryCount < m_Capacity;
	}

public:
	int GetSize() const {
		return m_EntryCount;
	}

private:
	int CalcIndexForHash(uint64_t hash) {
		return (int)(hash % (uint64_t)(m_Capacity));
	}

	uint64_t CalcHashForKey(const Key& key) {
		Hash<Key> hashClass;

		return hashClass.HashFunc(key);
	}


private:	
	PoolAllocator<Entry> m_EntryAlloc; // Allocates all table entries. Each entry corresponds to a bucket

	Entry** m_Table; // m_Table[key] points to the head of the entry linked list

	// Head and tail of the map linked list; used for traversal
	Entry* m_ListHead;
	Entry* m_ListTail; 

	int m_EntryCount; // Number of entries in the table

	int m_Capacity; // Max number of nodes that the table can use

private:
	struct Entry {
		Entry* prevValue; // Prev entry in the map linked list
		Entry* nextValue; // Next entry in the map linked list

		Entry* next; // Next entry that has the same index
		Value value;
		uint64_t hash;
    };

public:
	class Iterator {
		friend class HashMap;

	public:
		Iterator() {
			entry = nullptr;
		}

		Value& GetValue() {
			ASSERT(entry != nullptr);
			return entry->value;
		}

		Iterator& operator++() {
			if (entry != nullptr) {
				entry = entry->nextValue;
			}
            
            return *this;
		}

		Iterator operator++(int) {
			Iterator temp = *this;

			if (entry != nullptr) {
				entry = entry->nextValue;
			}

			return temp;
		}

		bool operator==(const Iterator& it) {
			return entry == it.entry;
		}

		bool operator!=(const Iterator& it) {
			return entry != it.entry;
		}

	private:
		HashMap::Entry* entry;
    };
};

#endif