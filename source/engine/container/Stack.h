#ifndef STACK_H_
#define STACK_H_

#include "base_include.h"

//--------------------------------------------------
//
// Stack
//
// A LIFO container
//
//--------------------------------------------------
template<typename T>
class Stack {

public:
    explicit Stack(int capacity) {
        ASSERT(capacity > 0);

        m_Capacity = capacity;
        m_Data = MEM_NEW T[capacity];
        m_Top = 0;
    }

    ~Stack() {
        m_Top = 0;
        m_Capacity = 0;
        MEM_DELETE_ARR(m_Data);
    }

    void Push(const T& data) {
        ASSERT(!IsFull());

        m_Data[m_Top] = data;
        ++m_Top;
    }

    void Pop() {
        ASSERT(!IsEmpty());

        --m_Top;
    }

    void Clear() {
        m_Top = 0;
    }

    // Returns a reference
    T& GetFront() { 
        ASSERT(m_Top > 0 && m_Top <= m_Capacity);

        return m_Data[m_Top - 1]; 
    }

    // Returns a const reference
    const T& GetFront() const {
        return const_cast<T&>(GetFront());
    }

    int GetCapacity() const { return m_Capacity; }

    bool IsFull() const { return (m_Top == m_Capacity); }
    bool IsEmpty() const { return (m_Top == 0); }

private:
    T* m_Data;
    int m_Capacity;

    // Index above the top element
    int m_Top;
};


#endif