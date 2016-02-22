#ifndef TREEMAP_H_
#define TREEMAP_H_

#include "base_include.h"

#include "allocator/PoolAllocator.h"

//--------------------------------------------------
//
// TreeMap
//
// An ordered associative array with one-to-one key and value correspondence
//
// Key MUST be a type that supports the comparison operator
//
// Each key must be unique - no duplicates allowed
// Ordering of keys is determined by applying the less than operator (<) on 
// the keys
//
// Implemented as a red black tree; implementation adapted from Introduction 
// to Algorithms, 3rd Edition by CLRS
//
//--------------------------------------------------
template<typename Key, typename Value>
class TreeMap {

// Forware declarations
public:
	class Iterator;
	friend class Iterator;

private:
	struct Node;

public:
	TreeMap(int capacity): m_NodeAlloc(capacity) {
		m_NodeAlloc.Clear();
		m_Capacity = capacity;

		m_NullNode.red = false;
		m_NullNode.parent = m_NullNode.left = m_NullNode.right = nullptr;

		m_Null = &m_NullNode;
		m_Root = m_Null;

		m_NodeCount = 0;
	}

	~TreeMap() {
		m_Root = nullptr;
		m_Null = nullptr;
		m_NodeAlloc.Clear();
	}

	// Inserts the value with the corresponding key
	//
	// The value is NOT inserted if the key already exists in the map
	void Insert(const Key& key, const Value& value) {
		ASSERT(m_NodeCount < m_Capacity);

		Node* node = m_NodeAlloc.Alloc();

		node->key = key;
		node->value = value;

		Node* prev = m_Null;
		Node* it = m_Root;

		while (it != m_Null) {
			prev = it;

			// If key already exists, method does NOT insert the new value
			if (key == it->key) {
				ASSERT(0);
				return;
			}
			if (node->key < it->key) {
				it = it->left;
			}
			else {
				it = it->right;
			}
		}

		node->parent = prev;

		if (prev == m_Null) {
			m_Root = node;
		}
		else if (node->key < prev->key) {
			prev->left = node;
		}
		else {
			prev->right = node;
		}

		node->left = m_Null;
		node->right = m_Null;
		node->red = true;

		InsertFixup(node);

		++m_NodeCount;
	}

	// Removes the node with the specified key
	//
	// Does nothing if there is no such node in the map
	void Remove(const Key& key) {
		Node* z = Find(key).node;

		// Returns if there is no node with the specified key
		if (z == nullptr || z == m_Null) {
			return;
		}

		Node* x = m_Null;
		Node* y = z;
		bool yOriginalRed = y->red;

		if (z->left == m_Null) {
			x = z->right;
			Transplant(z, z->right);
		}
		else if (z->right == m_Null) {
			x = z->left;
			Transplant(z, z->left);
		}
		else {
			y = FindMinimum(z->right);
			yOriginalRed = y->red;

			x = y->right;

			if (y->parent == z) {
				x->parent = y;
			}
			else {
				Transplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			Transplant(z, y);

			y->left = z->left;
			y->left->parent = y;
			y->red = z->red;
		}

		if (yOriginalRed == false) {
			RemoveFixup(x);
		}
        
        m_NodeAlloc.Dealloc(z);
        --m_NodeCount;
	}

	// Returns an iterator to the node that corresponds to the key
	//
	// If there is no such node, returns an iterator that equals to End()
	Iterator Find(const Key& key) {
		Node* prev = m_Null;
		Node* it = m_Root;

		while (it != m_Null) {
			prev = it;
			if (key == it->key) {
				break;
			}
			else if (key < it->key) {
				it = it->left;
			}
			else {
				it = it->right;
			}
		}

		if (it != m_Null) {
			Iterator res;

			res.owner = this;
            res.node = it;

			return res;
		}
		else {
			return End();
		}
	}

	void Clear() {
		m_NodeCount = 0;

		m_Root = m_Null;

		m_NodeAlloc.Clear();
	}

	Iterator Begin() {
		Iterator it;

		it.owner = this;
		it.node = FindMinimum(m_Root);

		return it;
	}

	Iterator End() {
		Iterator it;

		it.owner = this;
		it.node = m_Null;

		return it;
	}

private:
	// Red Black tree internal routines

	void LeftRotate(Node* node) {
		Node* x = node;
		Node* y = x->right;

		x->right = y->left;

		if (y->left != m_Null) {
			y->left->parent = x;
		}

		y->parent = x->parent;

		if (x->parent == m_Null) {
			m_Root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}

		y->left = x;
		x->parent = y;
	}

	void RightRotate(Node* node) {
		Node* x = node;
		Node* y = x->left;

		x->left = y->right;

		if (y->right != m_Null) {
			y->right->parent = x;
		}

		y->parent = x->parent;

		if (x->parent == m_Null) {
			m_Root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}

		y->right = x;
		x->parent = y;
	}

	void InsertFixup(Node* node) {
		Node* z = node;
		Node* y = m_Null;

		while (z->parent->red == true) {
			if (z->parent == z->parent->parent->left) {
				y = z->parent->parent->right;

				if (y->red == true) {
					z->parent->red = false;
					y->red = false;
					z->parent->parent->red = true;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->right) {
						z = z->parent;
						LeftRotate(z);
					}
					z->parent->red = false;
					z->parent->parent->red = true;
					RightRotate(z->parent->parent);
				}
			}
			else {
				y = z->parent->parent->left;

				if (y->red == true) {
					z->parent->red = false;
					y->red = false;
					z->parent->parent->red = true;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->left) {
						z = z->parent;
						RightRotate(z);
					}
					z->parent->red = false;
					z->parent->parent->red = true;
					LeftRotate(z->parent->parent);
				}
			}
		}

		m_Root->red = false;
	}

	void RemoveFixup(Node* x) {
		Node* w = m_Null;

		while (x != m_Root && x->red == false) {
			if (x == x->parent->left) {
				w = x->parent->right;

				if (w->red == true) {
					w->red = false;
					x->parent->red = true;

					LeftRotate(x->parent);

					w = x->parent->right;
				}

				if (w->left->red == false && w->right->red == false) {
					w->red = true;
					x = x->parent;
				}
				else {
					if (w->right->red == false) {
						w->left->red = false;
						w->red = true;

						RightRotate(w);

						w = x->parent->right;
					}

					w->red = x->parent->red;
					x->parent->red = false;
					w->right->red = false;

					LeftRotate(x->parent);

					x = m_Root;
				}
			}
			else {
				if (x == x->parent->right) {
					w = x->parent->left;

					if (w->red == true) {
						w->red = false;
						x->parent->red = true;

						RightRotate(x->parent);

						w = x->parent->left;
					}
					if (w->right->red == false && w->left->red == false) {
						w->red = true;
						x = x->parent;
					}
					else {
						if (w->left->red == false) {
							w->right->red = false;
							w->red = true;

							LeftRotate(w);

							w = x->parent->left;
						}

						w->red = x->parent->red;
						x->parent->red = false;
						w->left->red = false;

						RightRotate(x->parent);

						x = m_Root;
					}
				}
			}
		}

		x->red = false;
	}

	void Transplant(Node* u, Node* v) {
		if (u->parent == m_Null) {
			m_Root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	Node* FindMinimum(Node* node) {
		Node* x = node;

		while (x->left != m_Null) {
			x = x->left;
		}

		return x;
	}

	Node* FindMaximum(Node* node) {
		Node* x = node;

		while (x->right != m_Null) {
			x = x->right;
		}

		return x;
	}

	Node* FindSuccessor(Node* node) {
		Node* x = node;

		if (x->right != m_Null) {
			return FindMinimum(x->right);
		}	

		Node* y = x->parent;

		while (y != m_Null && x == y->right) {
			x = y;
			y = y->parent;
		}

		return y;
	}

private:
	PoolAllocator<Node> m_NodeAlloc;

	Node* m_Root;
	Node* m_Null; // Points to the null node

	int m_NodeCount;

	int m_Capacity;

	// Represents null; is colored black
	Node m_NullNode;

public:
	class Iterator {
		friend class TreeMap;

	public:
		Iterator() {
			node = nullptr;
		}

		Key GetKey() {
			if (owner != nullptr && node != owner->m_Null) {
				return node->key;
			}
		}

		Value& GetValue() {
            ASSERT(owner != nullptr && node != owner->m_Null);
            return node->value;
		}

		Iterator& operator++() {
			if (owner != nullptr && node != owner->m_Null) {
				node = owner->FindSuccessor(node);
			}
            
            return *this;
		}

		Iterator operator++(int) {
			Iterator temp = *this;

			if (owner != nullptr && node != owner->m_Null) {
				node = owner->FindSuccessor(node);
			}

            return temp;
		}

		bool operator==(const Iterator& it) {
			return node == it.node;
		}

		bool operator!=(const Iterator& it) {
			return node != it.node;
		}

	private:
		TreeMap* owner;
		Node* node;
	};

private:
	struct Node {
		Node* left;
		Node* right;
		Node* parent;
		bool red; // Node is red if true, and black if false
		Key key;
		Value value;
	};
};

#endif