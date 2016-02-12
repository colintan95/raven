#ifndef QUEUE_H_
#define QUEUE_H_

//--------------------------------------------------
//
// Queue.h
//
// Defines the Queue container
//
//--------------------------------------------------

#include "base_include.h"

//
// Queue
//
// A FIFO container that supports queuing in one direction
//
template<typename T>
class Queue {

public:
	Queue(int capacity) {
		// Stores one more unused entry for head and tail indexing to work
		m_Data = MEM_NEW T[capacity + 1];

		m_Capacity = capacity;
		m_Head = 0;
		m_Tail = 0;
	}

	~Queue() {
		m_Tail = 0;
		m_Head = 0;
		m_Capacity = 0;
		MEM_DELETE_ARR(m_Data);
	}

	void PushBack(const T& data) {
		ASSERT(!IsFull());

		m_Data[m_Tail] = data;

		// Wraps around when m_Tail >= m_DataLength
		m_Tail = (m_Tail + 1) % (m_Capacity + 1); 
	}

	void PopFront() {
		ASSERT(!IsEmpty());

		// Wraps around when m_Head >= m_DataLength
		m_Head = (m_Head + 1) % (m_Capacity + 1); 
	}

	void Clear() {
		m_Head = 0;
		m_Tail = 0;
	}

	T& GetFront() { 
		ASSERT(!IsEmpty());

		return m_Data[m_Head]; 
	}

	const T& GetFront() {
		return const_cast<T&>(GetFront());
	}

	int GetLength() const { return m_Capacity; }

	bool IsFull() const { return (m_Head == (m_Tail + 1) % (m_Capacity + 1)); }
	bool IsEmpty() const { return (m_Head == m_Tail); }

private:
	T* m_Data;
	int m_Capacity; // Actual data size if m_Capacity + 1
	int m_Head; // Index of first element
	int m_Tail; // Index of last element + 1
};

#endif