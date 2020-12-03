#ifndef __FHEAP_H_
#define __FHEAP_H_

#include <iostream>
#include <initializer_list>
#include <optional>
#include <vector>
#include <cmath>
#include <memory>
#include <queue>

template <typename T>
class PriorityQueue {
    public:
        virtual void insert(const T& item) = 0;
        virtual std::optional<T> extract_min() = 0;
        virtual bool is_empty() const = 0;
};

template <typename T>
class FibonacciNode {
    public:
        // constructors
        FibonacciNode();
        FibonacciNode(const T& item)
            :key(item), degree(0), child(nullptr), right(nullptr) {}

        // destructor
	// If you want to implement custom destructor, remove the default keyword
	// and implement your custom destructor.
        ~FibonacciNode() = default;

        T key;
        size_t degree;

        std::shared_ptr<FibonacciNode<T>> right;
        std::shared_ptr<FibonacciNode<T>> child;
        // NOTE: If you set left/parent pointer to shared_ptr, circular reference may cause!
        // So, left/parent pointer should be set to weak_ptr.
        std::weak_ptr<FibonacciNode<T>> left;
        std::weak_ptr<FibonacciNode<T>> parent;
};

template <typename T>
class FibonacciHeap : public PriorityQueue<T> {
    public:
        // Default Constructor
        FibonacciHeap()
            : min_node(nullptr), size_(0) {}

        // Constructor with Value
        FibonacciHeap(const T& item)
            : min_node(nullptr), size_(0) { insert(item); }

        // Disable copy constructor.
        FibonacciHeap(const FibonacciHeap<T> &);

        // Constructor with initializer_list
        // ex) FibonacciHeap<int> fheap = {1, 2, 3, 4};
        FibonacciHeap(std::initializer_list<T> list): min_node(nullptr), size_(0) {
            for(const T& item : list) {
                insert(item);
            }
        }

        // Destructor
        ~FibonacciHeap();

        // Insert item into the heap.
        void insert(const T& item) override;

        // Return raw pointer of the min node.
        // If the heap is empty, return nullptr.
        FibonacciNode<T>* get_min_node() { return min_node.get(); }

        // Return minimum value of the min node.
        // If the heap is empty, return std::nullopt.
        std::optional<T> get_min() const;

        // 1. Return minimum value of the min node
        // 2. Remove the node which contains minimum value from the heap.
        // If the heap is empty, return std::nullopt;
        std::optional<T> extract_min() override;

        // Return true if the heap is empty, false if not.
        bool is_empty() const override { return !size_; }

        // Return the number of nodes the heap contains.
        size_t size() const { return size_; }


    private:
        // Points to the node which has minimum value.
        std::shared_ptr<FibonacciNode<T>> min_node;

        // Value that represents how many nodes the heap contains.
        size_t size_;

        void insert(std::shared_ptr<FibonacciNode<T>>& node);

        // After extract, clean up the heap.
        void consolidate();

        // Combine two nodes.
        void merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y);
};


template <typename T>
void delete_node(std::shared_ptr<FibonacciNode<T>>& node) {
	if(!node) return;
	auto z = node;
	do {
		if(z->child) delete_node(z->child);
		z = z->right;
	} while(z && z != node);
	node->right = nullptr;
	node = nullptr;
}

template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
	delete_node(min_node);	
}

template <typename T>
std::optional<T> FibonacciHeap<T>::get_min() const {
	if(!min_node) 
		return std::nullopt;
	else
		return min_node->key;
}

template <typename T>
void FibonacciHeap<T>::insert(const T& item) {
	std::shared_ptr<FibonacciNode<T>> node = std::make_shared<FibonacciNode<T>>(item);
	insert(node);
}

template <typename T>
void FibonacciHeap<T>::insert(std::shared_ptr<FibonacciNode<T>>& node) {
	if(!min_node) {
		min_node = node;
		min_node->right = min_node;
		min_node->left = min_node;
	} else {
		std::shared_ptr<FibonacciNode<T>> l = min_node->left.lock();
		l->right = node;
		node->left = l;
		node->right = min_node;
		min_node->left = node;
		if(node->key < min_node->key)
			min_node = node;
	}
	size_++;
}

template <typename T>
std::optional<T> FibonacciHeap<T>::extract_min() {
	if(!min_node)
		return std::nullopt;
	if(min_node->child) {
		std::shared_ptr<FibonacciNode<T>> first_child = min_node->child;
		std::shared_ptr<FibonacciNode<T>> last_child = first_child->left.lock();
		std::shared_ptr<FibonacciNode<T>> l = min_node->left.lock();
		l->right = first_child;
		first_child->left = l;
		last_child->right = min_node;
		min_node->left = last_child;
	}

	T min_val = min_node->key;
	if(min_node->right == min_node) {
		min_node->right = nullptr;
		min_node = nullptr;
	} else {
		std::shared_ptr<FibonacciNode<T>> l = min_node->left.lock();
		std::shared_ptr<FibonacciNode<T>> r = min_node->right;
		l->right = r;
		r->left = l;
		min_node = r;
		consolidate();
	}
	size_--;
	return min_val;
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
	float phi = (1 + sqrt(5)) / 2.0;
	int len = int(log(size_) / log(phi)) + 10;
	std::vector<std::shared_ptr<FibonacciNode<T>>> A(len, nullptr);
	auto last_root = min_node->left.lock();
	for(auto z = min_node, next_root = z->right; ; z = next_root, next_root = z->right) {
		auto x = z;
		size_t d = x->degree;
		while(d < len && A[d]) {
			auto y = A[d];
			if(x->key > y->key) swap(x, y);
			merge(x, y);
			A[d] = nullptr;
			d++;
		}
		A[d] = x;
		if(z == last_root) break;
	}
	min_node = nullptr;
	for(auto &z : A) {
		if(!z) continue;
		if(!min_node || z->key < min_node->key)
			min_node = z;
	}
}

template <typename T>
void FibonacciHeap<T>::merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y) {
	std::shared_ptr<FibonacciNode<T>> l = y->left.lock();
	std::shared_ptr<FibonacciNode<T>> r = y->right;
	l->right = r;
	r->left = l;

	if(!x->child) {
		x->child = y;
		y->parent = x;
		y->right = y;
		y->left = y;
	} else {
		std::shared_ptr<FibonacciNode<T>> last_child = x->child->left.lock();
		y->parent = x;
		y->right = x->child;
		y->left = last_child;
		last_child->right = y;
		x->child->left = y;
	}
	x->degree++;
}

#endif // __FHEAP_H_
