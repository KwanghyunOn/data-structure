#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>
#include <cstring>

#define INITIAL_TABLE_SIZE 64

#include "hash_slot.hpp"

/* Fill in the TODO sections in the following code. */

template <typename K, typename V, typename F>
class HashTable {
public:
    HashTable();
    ~HashTable();
    int get(const K &key, V &value);
    int put(const K &key, const V &value);
    int remove(const K &key);
    size_t get_table_size();
    size_t get_size();
    double get_load_factor();

protected:
    size_t table_size;
    
private:
    HashTable(const HashTable & other);
    const HashTable & operator=(const HashTable & other);
    F hash_func;
    size_t size;
    HashSlot<K, V> *table;

    // Should be overriden by the derived class
    virtual unsigned long get_next_pos(unsigned long pos,
                                       unsigned long step) = 0;
    unsigned long get_pos(const K key);
    void enlarge_table();
};

template <typename K, typename V, typename F>
class LinearProbeHashTable: public HashTable<K, V, F> {
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
		return (pos + step) % HashTable<K, V, F>::get_table_size();
    }
};

template <typename K, typename V, typename F>
class QuadProbeHashTable: public HashTable<K, V, F> {
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
		return (pos + step*(step+1)/2) % HashTable<K, V, F>::get_table_size();
    }
};

template <typename K, typename V, typename F>
HashTable<K, V, F>::HashTable(): table(), hash_func(),
                                 size(0), table_size(INITIAL_TABLE_SIZE) {
    table = new HashSlot<K, V>[table_size];
}

template <typename K, typename V, typename F>
HashTable<K, V, F>::~HashTable() {
	delete[] table;
}

template <typename K, typename V, typename F>
void HashTable<K, V, F>::enlarge_table() {
	table_size <<= 1;
	size = 0;
	HashSlot<K, V> *old_table = table;
	table = new HashSlot<K, V>[table_size];
	for(size_t i = 0; i < (table_size>>1); i++) {
		if(old_table[i].is_removed() || old_table[i].is_empty()) continue;
		put(old_table[i].get_key(), old_table[i].get_value());
	}
	delete[] old_table;
}

template <typename K, typename V, typename F>
unsigned long HashTable<K, V, F>::get_pos(const K key) {
	return hash_func(key) % table_size;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::get(const K &key, V &value) {
	for(unsigned long i = 0; i < table_size; i++) {
		unsigned long pos = get_next_pos(get_pos(key), i);
		//printf("[get %d] Step %lu: pos %lu\n", key, i, pos);
		if(table[pos].is_removed()) continue;
		if(table[pos].is_empty()) return -1;
		if(table[pos].get_key() == key) {
			value = table[pos].get_value();
			return (int)i;
		}
	}
	return -1;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::put(const K &key, const V &value) {
	for(unsigned long i = 0; i < table_size; i++) {
		unsigned long pos = get_next_pos(get_pos(key), i);
		//printf("[put %d] Step %lu: pos %lu\n", key, i, pos);
		if(table[pos].is_removed()) continue;
		if(table[pos].is_empty()) {
			table[pos].set_key_value(key, value);
			//printf("[put %d] table[%lu] = %d\n", key, pos, value);
			size++;
			if(get_load_factor() >= 0.5)
				enlarge_table();
			return (int)i;
		} else if(table[pos].get_key() == key) {
			return -1;
		}
	}
	return -1;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::remove(const K &key) {
	for(unsigned long i = 0; i < table_size; i++) {
		unsigned long pos = get_next_pos(get_pos(key), i);
		//printf("[remove %d] Step %lu: pos %lu\n", key, i, pos);
		if(table[pos].is_removed()) continue;
		else if(table[pos].is_empty()) return -1;
		else if(table[pos].get_key() == key) {
			table[pos].set_removed();
			size--;
			return (int)i;
		}
	}
	return -1;
}

template <typename K, typename V, typename F>
size_t HashTable<K, V, F>::get_table_size() {
    return table_size;
}

template <typename K, typename V, typename F>
size_t HashTable<K, V, F>::get_size() {
    return size;
}

template <typename K, typename V, typename F>
double HashTable<K, V, F>::get_load_factor() {
    return (double)size/table_size;
}
