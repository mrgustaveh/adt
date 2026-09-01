#include "linked-list.h"
#include <iostream>

int main() {
    LinkedList<int> nums;
    nums.push_back(23);
    nums.push_front(30);
    nums.push_back(48);
    nums.push_front(200);
    nums.push_back(56);
    nums.push_front(89);
    nums.push_back(42);

    std::cout << "is empty: " << nums.empty_str() << "\n";
    std::cout << "size: " << nums.size() << "\n";

    std::cout << "pop-front" << "\n\n";
    nums.pop_front();

    std::cout << "is empty: " << nums.empty_str() << "\n";
    std::cout << "size: " << nums.size() << "\n";

    std::cout << "pop-back" << "\n\n";
    nums.pop_back();

    std::cout << "is empty: " << nums.empty_str() << "\n";
    std::cout << "size: " << nums.size() << "\n";

    return 0;
}
