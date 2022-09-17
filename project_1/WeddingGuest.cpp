#include "WeddingGuest.h"

#include <iostream>  // For testing
#include <string>

WeddingGuest::WeddingGuest() {
    this->table = new GuestDetails*[this->BUCKET_SIZE];
    for (unsigned int i = 0; i < this->BUCKET_SIZE; ++i) {
        this->table[i] = nullptr;
    }
}

WeddingGuest::~WeddingGuest() {
    for (unsigned int i = 0; i < this->BUCKET_SIZE; ++i) {
        delete this->table[i];
    }
    delete[] this->table;
}

unsigned int WeddingGuest::hash(std::string lastName) {
    return lastName.empty()                          ? 26
           : (lastName[0] > 64 && lastName[0] < 91)  ? lastName[0] + 32
           : (lastName[0] > 96 && lastName[0] < 123) ? lastName[0]
                                                     : 26;
}

unsigned int WeddingGuest::hash(GuestDetails* details) {
    return details->lastName.empty()                                   ? 26
           : (details->lastName[0] > 64 && details->lastName[0] < 91)  ? details->lastName[0] + 32
           : (details->lastName[0] > 96 && details->lastName[0] < 123) ? details->lastName[0]
                                                                       : 26;
}

bool WeddingGuest::noGuests() const {
    for (unsigned int i = 0; i < this->BUCKET_SIZE; ++i) {
        if (this->table[i] != nullptr) {
            return false;
        }
    }
    return true;
}

int WeddingGuest::guestCount() const {
    unsigned int count = 0;
    for (unsigned int i = 0; i < this->BUCKET_SIZE; ++i) {
        GuestDetails* current = this->table[i];
        for (GuestDetails* current = this->table[i]; current != nullptr; current = current->next) {
            ++count;
        }
    }
    return count;
}

int main() {
    WeddingGuest weddingGuest;

    return 0;
}