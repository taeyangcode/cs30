#include <cassert>
#include <iostream>
#include <new>
#include <set>
#include <stdexcept>

#include "smart_ptr.h"
using namespace std;
struct OurType {
    explicit OurType(int v = 0) : m_value(v) { m_count++; }
    OurType(const OurType& other) : m_value(other.m_value) {
        m_count++;
        m_asstcopycount++;
    }
    ~OurType() { m_count--; }
    OurType& operator=(const OurType& rhs) {
        m_value = rhs.m_value;
        m_asstcopycount++;
        return *this;
    }
    int m_value;
    static int m_count;
    static int m_asstcopycount;
};
inline bool operator==(const OurType& lhs, const OurType& rhs) { return lhs.m_value == rhs.m_value; }
inline bool operator<(const OurType& lhs, const OurType& rhs) { return lhs.m_value < rhs.m_value; }
inline bool operator!=(const OurType& lhs, const OurType& rhs) { return !(lhs == rhs); }
inline bool operator<=(const OurType& lhs, const OurType& rhs) { return !(rhs < lhs); }
inline bool operator>=(const OurType& lhs, const OurType& rhs) { return !(lhs < rhs); }
inline bool operator>(const OurType& lhs, const OurType& rhs) { return rhs < lhs; }
inline bool operator==(const OurType& lhs, int rhs) { return lhs.m_value == rhs; }
inline bool operator!=(const OurType& lhs, int rhs) { return !(lhs == rhs); }
#include <iostream>
inline std::ostream& operator<<(std::ostream& lhs, const OurType& rhs) { return lhs << rhs.m_value; }
int OurType::m_count = 0;
int OurType::m_asstcopycount = 0;
inline int itemcount() {
    return OurType::m_count;
}
inline int nasstcopys() {
    return OurType::m_asstcopycount;
}
std::set<void*> addrs;
bool recordaddrs = false;
int throwBadAlloc = 0;
// value of 0 means allocations don't throw
// value of 1 means 1st allocation throws
// value of 2 means 2nd allocation throws
// value of n means nth allocation throws
void* operator new(size_t n) {
    if (recordaddrs) {
        if (throwBadAlloc == 1)
            throw std::bad_alloc();
        else if (throwBadAlloc > 1)
            throwBadAlloc--;
    }
    void* p = malloc(n);
    if (recordaddrs) {
        recordaddrs = false;
        addrs.insert(p);
        recordaddrs = true;
    }
    return p;
}
void operator delete(void* p) noexcept {
    if (recordaddrs) {
        recordaddrs = false;
        std::set<void*>::iterator it = addrs.find(p);
        if (it != addrs.end())
            addrs.erase(it);
        recordaddrs = true;
    }
    free(p);
}
void testone(int n) {
    smart_ptr<double> dsp0;
    smart_ptr<double> dsp1{new double{3.14}};
    smart_ptr<double> dsp2, dsp3;
    switch (n) {
        default: {
            assert(false);
        } break;
        case 1: {
            assert(dsp0.ref_count() == 0);
        } break;
        case 2: {
            assert(dsp1.ref_count() == 1);
        } break;
        case 3: {
            dsp0 = dsp1;
            assert(dsp0.ref_count() == 2);
        } break;
        case 4: {
            dsp3 = dsp2 = dsp1;
            assert(dsp3.ref_count() == dsp2.ref_count() &&
                   dsp1.ref_count() == dsp2.ref_count() &&
                   dsp1.ref_count() == 3);
        } break;
        case 5: {
            dsp1 = dsp0;
            assert(dsp1.ref_count() == 0 && dsp0.ref_count() == 0);
        } break;
        case 6: {
            assert(std::is_nothrow_constructible<smart_ptr<int>>::value);
            assert(std::is_nothrow_copy_constructible<smart_ptr<int>>::value);
            // possible XCode compiler bug for following two asserts
            // assert(std::is_nothrow_constructible<smart_ptr<int>, int*>::value);
            // assert(std::is_nothrow_assignable<A, A>::value);
            assert(std::is_nothrow_move_assignable<smart_ptr<int>>::value);
            assert(std::is_nothrow_move_constructible<smart_ptr<int>>::value);
            assert(!noexcept(std::declval<smart_ptr<int>>().clone()));
            assert(!noexcept(std::declval<smart_ptr<int>>().operator*()));
            assert(!noexcept(std::declval<smart_ptr<int>>().operator->()));
            assert(noexcept(std::declval<smart_ptr<int>>().ref_count()));
        } break;
        case 7: {
            {
                // testing constructor
                smart_ptr<OurType> osp0{new OurType{0}};
                assert(itemcount() == 1);
            }
            assert(itemcount() == 0);
        } break;
        case 8: {
            {
                // testing assignment operator
                smart_ptr<OurType> osp0{new OurType{0}};
                assert(itemcount() == 1);
                smart_ptr<OurType> osp1;
                osp1 = osp0;
                assert(itemcount() == 1);
            }
            assert(itemcount() == 0);
        } break;
        case 9: {
            {
                // testing copy constructor
                smart_ptr<OurType> osp0{new OurType{0}};
                assert(itemcount() == 1);
                smart_ptr<OurType> osp1{osp0};
                assert(osp0.ref_count() == 2);
                assert(osp1.ref_count() == 2);
                assert(itemcount() == 1);
            }
            assert(itemcount() == 0);
        } break;
        case 10: {
            {
                // testing move constructor
                smart_ptr<OurType> osp0{new OurType{0}};
                assert(itemcount() == 1);
                smart_ptr<OurType> osp1{std::move(osp0)};
                assert(itemcount() == 1);
            }
            assert(itemcount() == 0);
        } break;
        case 11: {
            {
                // testing move assignment
                smart_ptr<OurType> osp0{new OurType{0}};
                assert(osp0.ref_count() == 1);
                assert(itemcount() == 1);
                smart_ptr<OurType> osp1;
                osp1 = std::move(osp0);
                assert(osp1.ref_count() == 1);
                assert(osp0.ref_count() == 0);
                assert(itemcount() == 1);
            }
            assert(itemcount() == 0);
        } break;
        case 12: {
            {
                // testing move constructor
                smart_ptr<OurType> osp0{new OurType{0}};
                assert(osp0.ref_count() == 1);
                assert(itemcount() == 1);
                smart_ptr<OurType> osp1{std::move(osp0)};
                assert(osp1.ref_count() == 1);
                assert(osp0.ref_count() == 0);
                assert(itemcount() == 1);
            }
            assert(itemcount() == 0);
        } break;
        case 13: {
            {
                // testing move assignment
                smart_ptr<OurType> osp0{new OurType{0}};
                assert(osp0.ref_count() == 1);
                assert(itemcount() == 1);
                smart_ptr<OurType> osp1;
                osp1 = std::move(osp0);
                assert(osp1.ref_count() == 1);
                assert(osp0.ref_count() == 0);
                assert(itemcount() == 1);
            }
            assert(itemcount() == 0);
        } break;
        case 14: {
            {
                // testing assignment operator
                smart_ptr<OurType> osp0;
                assert(itemcount() == 0);
                smart_ptr<OurType> osp1;
                osp1 = osp0;
                assert(itemcount() == 0);
            }
            assert(itemcount() == 0);
        } break;
        case 15: {
            try {
                *dsp0;
                assert(0);
            } catch (...) {
            }
        } break;
        case 16: {
            smart_ptr<OurType> osp{new OurType{42}};
            try {
                assert((*osp).m_value == 42);
            } catch (...) {
                assert(0);
            }
        } break;
        case 17: {
            smart_ptr<OurType> osp;
            try {
                (*osp).m_value;  // should throw
                assert(0);
            } catch (...) {
            }
        } break;
        case 18: {
            smart_ptr<OurType> osp{new OurType{42}};
            try {
                assert(osp->m_value == 42);
            } catch (...) {
                assert(0);
            }
        } break;
        case 19: {
            smart_ptr<OurType> osp;
            try {
                osp->m_value;  // should throw
                assert(0);
            } catch (...) {
            }
        } break;
        case 20: {
            assert(!dsp0.clone());
            assert(!dsp1.clone());
        } break;
        case 21: {
            dsp3 = dsp2 = dsp1;
            assert(dsp1.clone());
            assert(dsp1.ref_count() == 1 && dsp2.ref_count() == 2 && dsp3.ref_count() == 2);
            assert(*dsp1 == 3.14 && *dsp2 == 3.14 && *dsp3 == 3.14);
        } break;
        case 22: {
            dsp3 = dsp2 = dsp1;
            recordaddrs = true;
            throwBadAlloc = 1;  // first allocation throws exception
            size_t oldSize = addrs.size();
            try {
                dsp1.clone();
                assert(0);
            } catch (std::bad_alloc) {
            }
            // test for strong guarantee, no change to dsp1
            assert(*dsp1 == *dsp2 && dsp1.ref_count() == dsp2.ref_count());
            assert(addrs.size() == oldSize);
            recordaddrs = false;
        } break;
        case 23: {
            dsp3 = dsp2 = dsp1;
            recordaddrs = true;
            throwBadAlloc = 2;  // second allocation throws exception
            size_t oldSize = addrs.size();
            try {
                dsp1.clone();
                assert(0);
            } catch (std::bad_alloc) {
            }
            // test for strong guarantee, no change to dsp1
            assert(*dsp1 == *dsp2 && dsp1.ref_count() == dsp2.ref_count());
            assert(addrs.size() == oldSize);
            recordaddrs = false;
        } break;
        case 24: {
            dsp3 = dsp2 = dsp1;
            assert(*dsp1 == *dsp2 && *dsp2 == *dsp3 && *dsp1 == 3.14);
            assert(dsp1.ref_count() == dsp2.ref_count() &&
                   dsp2.ref_count() == dsp3.ref_count() &&
                   dsp1.ref_count() == 3);
        } break;
        case 25: {
            dsp3 = dsp2 = dsp1;
            dsp3 = dsp0;
            assert(*dsp1 == *dsp2 && *dsp1 == 3.14);
            assert(dsp1.ref_count() == dsp2.ref_count() &&
                   dsp1.ref_count() == 2 &&
                   dsp3.ref_count() == 0);
        }
    }
}
int main() {
    // for (unsigned int i = 1; i < 26; ++i) {
    testone(23);
    std::cout << "Passed: 23"
              << "\n";
    // }
}