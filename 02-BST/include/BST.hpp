#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>


template <typename T>
class TreeNode
{
    public:
        T element;
        std::unique_ptr<TreeNode<T>> left;
        std::unique_ptr<TreeNode<T>> right;

        TreeNode<T>(const T& e)
            :element{e}, left{nullptr}, right{nullptr} {}

        ~TreeNode() {}

};


template <typename T>
struct BST
{
    public:
        std::unique_ptr<TreeNode<T>> root = nullptr;

        ~BST() {}

        bool insert(const T& key);
        bool search(const T& key);
        bool remove(const T& key);

    private:
        bool insert(std::unique_ptr<TreeNode<T>>& t, const T& key);
        bool search(std::unique_ptr<TreeNode<T>>& t, const T& key);
        bool remove(std::unique_ptr<TreeNode<T>>& t, const T& key);

};

template <typename T>
bool BST<T>::insert(const T& key) {
    return insert(root, key);
}

template <typename T>
bool BST<T>::search(const T& key) {
    return search(root, key);
}

template <typename T>
bool BST<T>::remove(const T& key) {
    return remove(root, key);
}

template <typename T>
bool BST<T>::insert(std::unique_ptr<TreeNode<T>>& t, const T& key) {
	if(!t) {
		t = std::make_unique<TreeNode<T>>(key);
		return true;
	}
	if(key < t->element) return insert(t->left, key);
	else if(key == t->element) return false;
	else return insert(t->right, key);
}

template <typename T>
bool BST<T>::search(std::unique_ptr<TreeNode<T>>& t, const T& key) {
	if(!t) return false;
	if(key < t->element) return search(t->left, key);
	else if(key == t->element) return true;
	else return search(t->right, key);
}

template <typename T>
bool BST<T>::remove(std::unique_ptr<TreeNode<T>>& t, const T& key) {
	if(!t) return false;

	if(key == t->element) {
		if(!(t->left) && !(t->right))
			t.reset();
		else if((t->left) && !(t->right))
			t = std::move(t->left);
		else if(!(t->left) && (t->right))
			t = std::move(t->right);
		else {
			TreeNode<T> *mx = t->left.get();
			while(mx->right) mx = mx->right.get();
			t->element = mx->element;
			remove(t->left, t->element);
		}
		return true;
	}
	else if(key < t->element)
		return remove(t->left, key);
	else
		return remove(t->right, key);
}
