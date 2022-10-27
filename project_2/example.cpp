#include <iostream>
#include <vector>

int main() {
    std::vector<int> v(2);
    v.push_back(2);
    // v.push_back(2);
    std::cout << v.size();

    return 0;
}