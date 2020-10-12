#ifndef _DEQUE_H
#define _DEQUE_H

#include <string>
#include <iostream>
#include <type_traits>
#include <optional>
#include <iostream>
#include <memory>
#include <cassert>

/* NOTE: Deque, ArrayDeque, ListDeque Declaration modification is not allowed.
 * Fill in the TODO sections in the following code. */
template <typename T>
class Deque {
public:
    virtual ~Deque() = default;

    /* NOTE: We won't implement push functions that take rvalue references. */
    virtual void push_front(const T&) = 0;
    virtual void push_back(const T&) = 0;

    /* NOTE: Unlike STL implementations which have separate `front` and
       pop_front` functions, we have one unified method for removing an elem. */
    virtual std::optional<T> remove_front() = 0;
    virtual std::optional<T> remove_back() = 0;

    virtual bool empty() = 0;
    virtual size_t size() = 0;

    virtual T& operator[](size_t) = 0;
};

template <typename T>
class ArrayDeque : public Deque<T> {
public:
    ArrayDeque();
    ~ArrayDeque() = default;

    void push_front(const T&) override;
    void push_back(const T&) override;

    std::optional<T> remove_front() override;
    std::optional<T> remove_back() override;

    bool empty() override;
    size_t size() override;
    size_t capacity();

    T& operator[](size_t) override;

private:
    std::unique_ptr<T[]> arr;
    size_t front;
    size_t back;
    size_t size_;
    size_t capacity_;

    void resize();
};

template <typename T>
ArrayDeque<T>::ArrayDeque() :
    front{63 /* You can change this */},
    back{0 /* You can change this */},
    size_{0}, capacity_{64} {
    arr = std::make_unique<T[]>(capacity_);
}

template <typename T>
void ArrayDeque<T>::push_front(const T& item) {
	if(front == back) resize();
	arr[front] = item;
	front = (front > 0) ? front-1 : capacity_-1;
	size_++;
}

template <typename T>
void ArrayDeque<T>::push_back(const T& item) {
	if(front == back) resize();
	arr[back] = item;
	back = (back < capacity_-1) ? back+1 : 0;
	size_++;
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_front() {
	if(size_ > 0) {
		front = (front < capacity_-1) ? front+1 : 0;
		size_--;
		return arr[front];
	}
    return std::nullopt;
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_back() {
	if(size_ > 0) {
		back = (back > 0) ? back-1 : capacity_-1;
		size_--;
		return arr[back];
	}
    return std::nullopt;
}

template <typename T>
void ArrayDeque<T>::resize() {
	size_t new_capacity_ = 2 * capacity_;
	std::unique_ptr<T[]> new_arr = std::make_unique<T[]>(new_capacity_);
	if(front < back) {
		for(int i = front; i < back; i++)
			new_arr[i] = arr[i];
	} else {
		for(int i = 0; i < back; i++)
			new_arr[i] = arr[i];
		for(int i = front; i < capacity_; i++)
			new_arr[i + capacity_] = arr[i];
		front += capacity_;
	}
	arr = std::move(new_arr);
	capacity_ = new_capacity_;
}

template <typename T>
bool ArrayDeque<T>::empty() {
	return size_ == 0;
}

template <typename T>
size_t ArrayDeque<T>::size() {
	return size_;
}

template <typename T>
size_t ArrayDeque<T>::capacity() {
	return capacity_;
}

template <typename T>
T& ArrayDeque<T>::operator[](size_t idx) {
	return arr[(front + idx + 1) % capacity_];
}

template<typename T>
struct ListNode {
    std::optional<T> value;
    ListNode* prev;
    ListNode* next;

    ListNode() : value(std::nullopt), prev(this), next(this) {}
    ListNode(const T& t) : value(t), prev(this), next(this) {}

    ListNode(const ListNode&) = delete;
};

template<typename T>
class ListDeque : public Deque<T> {
public:
    ListDeque();
    ~ListDeque();

    void push_front(const T&) override;
    void push_back(const T&) override;

    std::optional<T> remove_front() override;
    std::optional<T> remove_back() override;

    bool empty() override;
    size_t size() override;

    T& operator[](size_t) override;

    size_t size_ = 0;
    ListNode<T>* sentinel = nullptr;
};

template<typename T>
ListDeque<T>::ListDeque() : sentinel(new ListNode<T>{}), size_(0) {}

template<typename T>
void ListDeque<T>::push_front(const T& t) {
	ListNode<T>* node = new ListNode<T>(t);
	node->next = sentinel->next;
	node->prev = sentinel;
	node->next->prev = node;
	sentinel->next = node;
	size_++;
}

template<typename T>
void ListDeque<T>::push_back(const T& t) {
	ListNode<T>* node = new ListNode<T>(t);
	node->next = sentinel;
	node->prev = sentinel->prev;
	node->prev->next = node;
	sentinel->prev = node;
	size_++;
}

template<typename T>
std::optional<T> ListDeque<T>::remove_front() {
	if(size_ > 0) {
		T front_val = sentinel->next->value.value();
		ListNode<T>* new_front = sentinel->next->next;
		delete sentinel->next;
		new_front->prev = sentinel;
		sentinel->next = new_front;
		size_--;
		return front_val;
	}
    return std::nullopt;
}

template<typename T>
std::optional<T> ListDeque<T>::remove_back() {
	if(size_ > 0) {
		T back_val = sentinel->prev->value.value();
		ListNode<T>* new_back = sentinel->prev->prev;
		delete sentinel->prev;
		new_back->next = sentinel;
		sentinel->prev = new_back;
		size_--;
		return back_val;
	}
    return std::nullopt;
}

template<typename T>
bool ListDeque<T>::empty() {
	return size_ == 0;
}

template<typename T>
size_t ListDeque<T>::size() {
	return size_;
}

template<typename T>
T& ListDeque<T>::operator[](size_t idx) {
	ListNode<T>* cur = sentinel->next;
	for(int i = 0; i < idx; i++)
		cur = cur->next;
	return cur->value.value();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListNode<T>& n) {
    if (n.value)
        os << n.value.value();

    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListDeque<T>& l) {
    auto np = l.sentinel->next;
    while (np != l.sentinel) {
        os << *np << ' ';
        np = np->next;
    }

    return os;
}

template<typename T>
ListDeque<T>::~ListDeque() {
	ListNode<T>* cur = sentinel->next;
	while(cur != sentinel) {
		ListNode<T>* nxt = cur->next;
		delete cur;
		cur = nxt;
	}
	delete cur;
}

#endif // _DEQUE_H
