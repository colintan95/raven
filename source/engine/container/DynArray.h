#ifndef DYNARRAY_H_
#define DYNARRAY_H_

#include "base_include.h"

#include <cstring>

//--------------------------------------------------
//
// DynArray
//
// Resizable, dynamic array
//
//--------------------------------------------------
template<typename T>
class DynArray {

public:
	DynArray(size_t capacity) {
		m_Data = nullptr;

		Resize(capacity);

		m_Tail = 0;
	}

	~DynArray() {
		m_Tail = 0;
		m_Capacity = 0;

		if (m_Data != nullptr) {
			delete[] m_Data;
		}

	}

	T& operator[](int index) {
		return m_Data[index];
	}

	void PushBack(const T& element) {
		ASSERT(!IsFull());

		m_Data[m_Tail] = element;
		++m_Tail;
	}

	void PopBack() {
		ASSERT(!IsEmpty());

		--m_Tail;
	}

	void Insert(const T& element, int index) {
		ASSERT(!IsFull());
		ASSERT(index <= m_Tail);

		for (int i = m_Tail; i > index; --i) {
			m_Data[i] = m_Data[i-1];
		}
		m_Data[index] = element;

		++m_Tail;
	}

	void Remove(int index) {
		ASSERT(!IsEmpty());
		ASSERT(index < m_Tail);

		for (int i = index; i < (m_Tail - 1); ++i) {
			m_Data[i] = m_Data[i+1];
		}

		--m_Tail;
	}

	void Resize(size_t capacity) {
		ASSERT(capacity > 0);

		T* prevMem = m_Data;

		m_Data = new T[capacity];

		// Copies and deletes the old memory if memory was allocated before
		if (prevMem != nullptr) {
			size_t overlap = capacity > m_Capacity ? m_Capacity : capacity;
			
			memcpy((void*)m_Data, (void*)prevMem, overlap * sizeof(T));
			delete[] prevMem;

			if (m_Tail > overlap) {
				m_Tail = overlap;
			}
		}

		m_Capacity = capacity;
	}

	T& GetBack() {
		ASSERT(!IsEmpty());

		return m_Data[m_Tail];
	}

	T& GetFront() {
		ASSERT(!IsEmpty());

		return m_Data[0];
	}

	size_t GetCapacity() const {
		return m_Capacity;
	}

	size_t GetSize() const {
		return m_Tail;
	}

	bool IsEmpty() const {
		return m_Tail == 0;
	}

	bool IsFull() const {
		return m_Tail == m_Capacity;
	}

private:
	T* m_Data;
	size_t m_Capacity;

	// Index of (last element in the array + 1)
	//
	// Indicates the size of the array
	size_t m_Tail;
};

#endif