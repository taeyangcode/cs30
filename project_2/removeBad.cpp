#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

void removeOdds(std::list<int>& l) {
    for (auto index = l.begin();
         index != l.end();
         index = (*index & 1) ? l.erase(index) : ++index) {
    }
}

void removeOdds(std::vector<int>& v) {
    for (auto index = v.begin();
         index != v.end();
         index = (*index & 1) ? v.erase(index) : ++index) {
    }
}

std::vector<int> destroyedOnes;

class Movie {
   public:
    Movie(int r) : m_rating(r) {
    }

    ~Movie() {
        destroyedOnes.push_back(this->m_rating);
    }

    int rating() const {
        return this->m_rating;
    }

   private:
    int m_rating;
};

void removeBad(std::list<Movie*>& l) {
    for (auto index = l.begin(); index != l.end();) {
        if ((*index)->rating() < 50) {
            delete *index;
            index = l.erase(index);
        } else {
            ++index;
        }
    }
}

void removeBad(std::vector<Movie*>& l) {
    for (auto index = l.begin(); index != l.end();) {
        if ((*index)->rating() < 50) {
            delete *index;
            index = l.erase(index);
        } else {
            ++index;
        }
    }
}

void testone(int n) {
    switch (n) {
        default: {
            assert(false);
        } break;
        case 1: {  // removeOdds(<list>&)
            int a[8] = {2, 8, 5, 6, 7, 3, 4, 1};
            list<int> x(a, a + 8);  // construct x from the array
            assert(x.size() == 8 && x.front() == 2 && x.back() == 1);
            removeOdds(x);
            assert(x.size() == 4);
            vector<int> v(x.begin(), x.end());  // construct v from x
            sort(v.begin(), v.end());
            int expect[4] = {2, 4, 6, 8};
            for (int k = 0; k < 4; k++)
                assert(v[k] == expect[k]);
        } break;
        case 2: {  // removeOdds(<vector>&)
            int a[8] = {2, 8, 5, 6, 7, 3, 4, 1};
            vector<int> x(a, a + 8);  // construct x from the array
            assert(x.size() == 8 && x.front() == 2 && x.back() == 1);
            removeOdds(x);
            assert(x.size() == 4);
            sort(x.begin(), x.end());
            int expect[4] = {2, 4, 6, 8};
            for (int k = 0; k < 4; k++)
                assert(x[k] == expect[k]);
        } break;
        case 3: {  // removeBad(<list>&)
            int a[8] = {85, 80, 30, 70, 20, 15, 90, 10};
            list<Movie*> x;
            for (int k = 0; k < 8; k++)
                x.push_back(new Movie(a[k]));
            assert(x.size() == 8 && x.front()->rating() == 85 &&
                   x.back()->rating() == 10);
            removeBad(x);
            assert(x.size() == 4 && destroyedOnes.size() == 4);
            vector<int> v;
            for (list<Movie*>::iterator p = x.begin(); p !=
                                                       x.end();
                 p++) {
                Movie* mp = *p;
                v.push_back(mp->rating());
            }
            sort(v.begin(), v.end());
            int expect[4] = {70, 80, 85, 90};
            for (int k = 0; k < 4; k++)
                assert(v[k] == expect[k]);
            sort(destroyedOnes.begin(), destroyedOnes.end());
            int expectGone[4] = {10, 15, 20, 30};
            for (int k = 0; k < 4; k++)
                assert(destroyedOnes[k] == expectGone[k]);
        } break;
        case 4: {  // removeBad(<vector>&)
            destroyedOnes.clear();
            int a[8] = {85, 80, 30, 70, 20, 15, 90, 10};
            vector<Movie*> x;
            for (int k = 0; k < 8; k++)
                x.push_back(new Movie(a[k]));
            assert(x.size() == 8 && x.front()->rating() == 85 &&
                   x.back()->rating() == 10);
            removeBad(x);
            assert(x.size() == 4 && destroyedOnes.size() == 4);
            vector<int> v;
            for (int k = 0; k < 4; k++)
                v.push_back(x[k]->rating());
            sort(v.begin(), v.end());
            int expect[4] = {70, 80, 85, 90};
            for (int k = 0; k < 4; k++)
                assert(v[k] == expect[k]);
            sort(destroyedOnes.begin(), destroyedOnes.end());
            int expectGone[4] = {10, 15, 20, 30};
            for (int k = 0; k < 4; k++)
                assert(destroyedOnes[k] == expectGone[k]);
        }
    }
}
int main() {
    for (unsigned int index = 1; index <= 4; ++index) {
        testone(index);
    }
    std::cout << "Passed\n";
}