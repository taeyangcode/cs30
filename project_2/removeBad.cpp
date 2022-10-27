#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <vector>

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