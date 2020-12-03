#include "include/fheap.hpp"
#include <assert.h>
#include <bits/stdc++.h>

template <typename T>
void traverse_root_list(FibonacciNode<T>* min_node) {
    if(!min_node) {
        return;
    }
    std::queue<T> root_key;
    root_key.push(min_node->key);
    FibonacciNode<T>* temp = min_node->right.get();
    while(temp != min_node) {
        root_key.push(temp->key);
        temp = temp->right.get();
    }
    while(!root_key.empty()) {
        T item = root_key.front();
        std::cout << item << " ";
        root_key.pop();
    }
    std::cout << std::endl;
}

template <typename T>
std::vector<FibonacciNode<T>*> get_root_list(FibonacciHeap<T>& h) {
    std::vector<FibonacciNode<T>*> root_list;

    if(h.is_empty())
        return root_list;

    FibonacciNode<T>* node = h.get_min_node();
    do {
        root_list.push_back(node);
        node = node->right.get();
    } while(node != h.get_min_node());

    return root_list;
}

template <typename T>
void check_min_heap_property(FibonacciNode<T>* n) {
    if(!n->child)
        return;

    std::vector<FibonacciNode<T>*> children_list;
    FibonacciNode<T>* node = n->child.get();
    do {
        children_list.push_back(node);
        node = node->right.get();
    } while(node != n->child.get());

    for(FibonacciNode<T>* child: children_list) {
        assert(n->key <= child->key);
        check_min_heap_property(child);
    }
}



int main() {
    FibonacciHeap<int> heap;
    std::vector<int> inserted;
    std::vector<int> extracted;
    size_t N = 10;
    // Prepare a random vector
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<size_t> dis_ins(200,500);
    std::uniform_int_distribution<size_t> dis_ext(100, 200);
    std::uniform_int_distribution<size_t> dis_key(0, 10000);
	
	N = 1;

    for(size_t i = 0 ; i < N; ++i) {
        extracted.clear();

        size_t num_ins = dis_ins(g);
        size_t num_ext = dis_ext(g);
		// num_ins = num_ext = 10;
		std::vector<int> t = {2, 5, 4, 1, 3};

        for(size_t j = 0 ; j < num_ins ; ++j) {
            int v = dis_key(g);
			//v = t[j];
            inserted.emplace_back(v);
            heap.insert(v);
        }

        for(size_t k = 0 ; k < num_ext ; ++k) {
			//printf("extract!\n");
			//auto min_node = heap.get_min_node();
			//auto z = min_node;
			//do {
				//printf("%d ", z->key);
				//z = z->right.get();
			//} while(z != min_node);
			//printf("\n");
            int ext = heap.extract_min().value();
            extracted.emplace_back(ext);
        }


		for(auto e : inserted) printf("%d ", e);
		printf("\n");
        std::sort(inserted.begin(), inserted.end(), std::less<int>());
		for(auto e : inserted) printf("%d ", e);
		printf("\n");
		for(auto e : extracted) printf("%d ", e);
		printf("\n");
        for(size_t l = 0 ; l < num_ext ; ++l) {
			if(inserted[l] != extracted[l])
				printf("%d: %d != %d\n", l, inserted[l], extracted[l]);
            assert(inserted[l] == extracted[l]);
        }
        inserted = std::vector<int>(inserted.begin() + num_ext, inserted.end());


        // Every root in the root list has a distinct degree value
        std::map<int, bool> degree_map;
        std::vector<FibonacciNode<int>*> root_list = get_root_list(heap);
        for(FibonacciNode<int>* root : root_list) {
            assert(degree_map[root->degree] == false);
            degree_map[root->degree] = true;
        }
    }

}
