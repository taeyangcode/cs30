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

char WeddingGuest::toLower(char letter) const {
    return (letter > 64 && letter < 91) ? letter + 32 : letter;
}

bool WeddingGuest::isLower(char letter) const {
    return (letter > 96 && letter < 123);
}

bool WeddingGuest::isUpper(char letter) const {
    return (letter > 64 && letter < 91);
}

unsigned int WeddingGuest::hash(std::string lastName) {
    return lastName.empty()               ? 26
           : (this->isUpper(lastName[0])) ? lastName[0] - 65
           : (this->isLower(lastName[0])) ? lastName[0] - 97
                                          : 26;
}

unsigned int WeddingGuest::hash(GuestDetails* details) {
    return details->lastName.empty()               ? 26
           : (this->isUpper(details->lastName[0])) ? details->lastName[0] - 65
           : (this->isLower(details->lastName[0])) ? details->lastName[0] - 97
                                                   : 26;
}

/*
 *  @param stringOne : compared value
 *  @param stringTwo : comparing value
 *
 *  @returns int : [-3, 1]
 *
 *  @description
 *      compare method requires two arguments, stringOne (compared), and stringTwo (comparing).
 *      Method will return an integer from [-3, 1] based on the following cases:
 *      -3 : special character found in compared string (before comparing string)
 *      -2 : special character found in comparing string (before compared string)
 *      -1 : compared string's terminating character's index is before comparing string's terminating character
 *      0  : compared and comparing strings' terminating character is found at the same index
 *      1  : comparing string's terminating character's index is before compared string's terminating character
 *
 *      * Note: Method is case-insensitive
 */

int WeddingGuest::compare(const std::string stringOne, const std::string stringTwo) const {
    for (unsigned int i = 0;; ++i) {
        const char charOne = this->toLower(stringOne[i]);
        const char charTwo = this->toLower(stringTwo[i]);

        if (charOne == 0x00 && charTwo != 0x00) {
            return -1;
        }
        if (charOne != 0x00 && charTwo == 0x00) {
            return 1;
        }
        if (charOne == 0x00 && charTwo == 0x00) {
            return 0;
        }
        if (!isLower(charOne)) {
            return -3;
        }
        if (!isLower(charTwo)) {
            return -2;
        }
        if (charOne == charTwo) {
            continue;
        }
        return (charOne > charTwo) * 2 - 1;
    }
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

bool WeddingGuest::inviteGuest(const std::string& firstName, const std::string& lastName, const GuestType& value) {
    const unsigned int bucketIndex = this->hash(lastName);
    GuestDetails* bucket = this->table[bucketIndex];

    if (bucket == nullptr) {
        this->table[bucketIndex] = new GuestDetails(firstName, lastName, value);
        return true;
    }

    while (bucket != nullptr) {
        const int lastNameComparison = this->compare(lastName, bucket->lastName);
        if (lastNameComparison == 0) {
            const int firstNameComparison = this->compare(firstName, bucket->firstName);
            if (firstNameComparison == 0) {
                return false;
            }
            if (firstNameComparison == -1) {
                GuestDetails* newGuest = new GuestDetails(firstName, lastName, value, bucket->prev, bucket);
                if (bucket->prev != nullptr) {
                    bucket->prev->next = newGuest;
                } else {
                    this->table[bucketIndex] = newGuest;
                }
                bucket->prev = newGuest;
                return true;
            }
        }

        if (lastNameComparison == -1) {
            GuestDetails* newGuest = new GuestDetails(firstName, lastName, value, bucket->prev, bucket);
            if (bucket->prev != nullptr) {
                bucket->prev->next = newGuest;
            } else {
                this->table[bucketIndex] = newGuest;
            }
            bucket->prev = newGuest;
            return true;
        }

        bucket = bucket->next;
    }
    GuestDetails* newGuest = new GuestDetails(firstName, lastName, value, bucket->prev, bucket);
    if (bucket->prev != nullptr) {
        bucket->prev->next = newGuest;
    } else {
        this->table[bucketIndex] = newGuest;
    }
    bucket->prev = newGuest;
    return true;
}

int main() {
    // WeddingGuest weddingGuest;

    // weddingGuest.inviteGuest("Corey", "Mostero", "value");
    // weddingGuest.inviteGuest("Corey", "M", "value");
    // weddingGuest.inviteGuest("Core", "M", "value");
    // weddingGuest.inviteGuest("D", "M", "value");

    // std::cout << weddingGuest.guestCount();

    return 0;
}