#include <iostream>

#include "smart_ptr.h"

int main() {
    int* p{new int{42}};
    smart_ptr<int> sp1{p};
    // Ref Count is 1
    std::cout << "Ref count is " << sp1.ref_count() << std::endl;
    {
        smart_ptr<int> sp2{sp1};
        // Ref Count is 2
        std::cout << "Ref count is " << sp1.ref_count() << std::endl;
        // Ref Count is 2
        std::cout << "Ref count is " << sp2.ref_count() << std::endl;
    }
    // Ref Count is 1
    std::cout << "Ref count is " << sp1.ref_count() << std::endl;
    smart_ptr<int> sp3;
    // Ref Count is 0
    std::cout << "Ref count is " << sp3.ref_count() << std::endl;
    sp3 = sp1;
    // Ref Count is 2
    std::cout << "Ref count is " << sp1.ref_count() << std::endl;
    // Ref Count is 2
    std::cout << "Ref count is " << sp3.ref_count() << std::endl;
    smart_ptr<int> sp4 = std::move(sp1);
    std::cout << *sp4 << " " << *sp3 << std::endl;  // prints 42 42
    std::cout << *sp1 << std::endl;                 // throws null_ptr_exception

    return 0;
}