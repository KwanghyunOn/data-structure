#include "include/rbtree.hpp"
#include <algorithm>
#include <stdio.h>
#include <random>
#include <vector>

template <typename T>
bool is_red(const RBNode<T>* n) {
    return n && n->color == RED;
}

template<typename T>
bool test_left_lean(const RBTree<T>& rbtree) {
    auto res = true;

    if (!rbtree.root)
        return true;

    rbtree.root->traverse_inorder([&res](RBNode<int>* n) {
        if (is_red(n)) {
            if (RBNode<int>::is_red(n->right) ||
                RBNode<int>::is_red(n->left)) {
                /* No back-to-back red */
                res = false;
            }
        }

        if (RBNode<int>::is_red(n->right) &&
            !RBNode<int>::is_red(n->left)) {
            /* No right leaning node */
            res = false;
        }
    });

    return res;
}

template<typename T>
bool test_black_balance(const RBTree<T>& rbtree) {
    size_t num_black = 0;
    auto all_leaves = rbtree.collect_all_leaves();

    auto res = std::all_of(all_leaves.begin(), all_leaves.end(),
                           [&num_black](const auto& pn){
                               if (num_black == 0) {
                                   num_black = pn.first.num_black_;
                                   return true;
                               }

                               return num_black == pn.first.num_black_;
                           });

    return res;
}


void dfs(RBNode<int>* x) {
	printf("%d start\n", x->key);
	if(x->left) dfs(x->left.get());
	printf("INFO: %d ", x->key);
	if(x->color) printf("BLACK\n");
	else printf("RED\n");
	if(x->right) dfs(x->right.get());
	printf("%d end\n", x->key);
}


int main() {
    RBTree<int> rbtree;
    std::vector<int> xs;
    size_t n = 4;

    for (auto i = 0; i < n; i++)
        xs.emplace_back(i);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(xs.begin(), xs.end(), g);

	xs = {3,0,2};

	for(auto e : xs) printf("%d ", e);
	printf("\n");

    for (auto i : xs)
        rbtree.insert(i);
	
	dfs(rbtree.root.get());

	if(test_black_balance(rbtree)) printf("SUCCESS\n");
	else printf("FAIL\n");
}

