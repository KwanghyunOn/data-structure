#include <string>

#include "deque.hpp"

template<typename Deque>
class Palindrome {
public:
    bool is_palindrome(const std::string&);
    void reset_deque();

private:
    Deque deque;
};

template<typename Deque>
bool Palindrome<Deque>::is_palindrome(const std::string& s1) {
	for(auto c : s1) deque.push_back(c);
	int idx = 0;
	while(!deque.empty())
		if(deque.remove_back().value() != s1[idx++]) return false;
	return true;
}

template<typename Deque>
void Palindrome<Deque>::reset_deque() {
    while (!deque.empty())
        deque.remove_front();
}
