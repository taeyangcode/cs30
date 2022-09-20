#include "WeddingGuest.h"

#include <iostream>
#include <string>

void WeddingGuest::clearBucket(GuestDetails* head) {
    while (head != nullptr) {
        GuestDetails* next = head->next;
        delete head;
        head = next;
    }
}

WeddingGuest::WeddingGuest() {
    this->table = new GuestDetails*[this->BUCKET_SIZE];
    for (unsigned int i = 0; i < this->BUCKET_SIZE; ++i) {
        this->table[i] = nullptr;
    }
}

WeddingGuest::~WeddingGuest() {
    for (unsigned int i = 0; i < this->BUCKET_SIZE; ++i) {
        this->clearBucket(this->table[i]);
    }
    delete[] this->table;
}

void WeddingGuest::inviteGuests(WeddingGuest& current, const WeddingGuest& other) {
    std::string firstName, lastName;
    GuestType value;
    int index = 0;
    for (bool membersLeft = other.verifyGuestOnTheList(index, firstName, lastName, value); membersLeft; membersLeft = other.verifyGuestOnTheList(++index, firstName, lastName, value)) {
        current.inviteGuest(firstName, lastName, value);
    }
}

void WeddingGuest::inviteGuests(const WeddingGuest& other) {
    std::string firstName, lastName;
    GuestType value;
    int index = 0;
    for (bool membersLeft = other.verifyGuestOnTheList(index, firstName, lastName, value); membersLeft; membersLeft = other.verifyGuestOnTheList(++index, firstName, lastName, value)) {
        this->inviteGuest(firstName, lastName, value);
    }
}

WeddingGuest::WeddingGuest(const WeddingGuest& other) {
    this->table = new GuestDetails*[this->BUCKET_SIZE];
    for (unsigned int i = 0; i < this->BUCKET_SIZE; ++i) {
        this->clearBucket(this->table[i]);
        this->table[i] = nullptr;
    }
    this->inviteGuests(other);
}

WeddingGuest& WeddingGuest::operator=(const WeddingGuest& other) {
    for (unsigned int i = 0; i < this->BUCKET_SIZE; ++i) {
        this->clearBucket(this->table[i]);
        this->table[i] = nullptr;
    }
    this->inviteGuests(other);
    return *this;
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

unsigned int WeddingGuest::hash(std::string lastName) const {
    return lastName.empty()               ? 26
           : (this->isUpper(lastName[0])) ? lastName[0] - 65
           : (this->isLower(lastName[0])) ? lastName[0] - 97
                                          : 26;
}

unsigned int WeddingGuest::hash(GuestDetails* details) const {
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
 *      * Note: Method is case-sensitive
 *
 *      compare method requires two arguments, stringOne (compared), and stringTwo (comparing).
 *      Method will return an integer from [-3, 1] based on the following cases:
 *      -3 : special character found in compared string (before comparing string)
 *      -2 : special character found in comparing string (before compared string)
 *      -1 : compared string's terminating character's index is before comparing string's terminating character
 *      0  : compared and comparing strings' terminating character is found at the same index
 *      1  : comparing string's terminating character's index is before compared string's terminating character
 */

int WeddingGuest::compare(const std::string stringOne, const std::string stringTwo) const {
    for (unsigned int i = 0;; ++i) {
        const char charOne = stringOne[i];
        const char charTwo = stringTwo[i];

        if (charOne == 0x00 && charTwo != 0x00) {
            return -1;
        }
        if (charOne != 0x00 && charTwo == 0x00) {
            return 1;
        }
        if (charOne == 0x00 && charTwo == 0x00) {
            return 0;
        }
        if (!this->isLower(this->toLower(charOne))) {
            return -3;
        }
        if (!this->isLower(this->toLower(charTwo))) {
            return -2;
        }
        if (charOne == charTwo) {
            continue;
        }
        return (this->toLower(charOne) > this->toLower(charTwo)) * 2 - 1;
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
                if (this->table[bucketIndex] == bucket) {
                    this->table[bucketIndex] = new GuestDetails(firstName, lastName, value, nullptr, bucket);
                    bucket->prev = this->table[bucketIndex];
                    return true;
                }
                GuestDetails* newGuest = new GuestDetails(firstName, lastName, value, bucket->prev, bucket);
                if (bucket->prev != nullptr) {
                    bucket->prev->next = newGuest;
                }
                if (bucket->next != nullptr) {
                    bucket->next->prev = newGuest;
                }
                return true;
            }
        }

        if (lastNameComparison == -1) {
            if (this->table[bucketIndex] == bucket) {
                this->table[bucketIndex] = new GuestDetails(firstName, lastName, value, nullptr, bucket);
                bucket->prev = this->table[bucketIndex];
                return true;
            }
            GuestDetails* newGuest = new GuestDetails(firstName, lastName, value, bucket->prev, bucket);
            if (bucket->prev != nullptr) {
                bucket->prev->next = newGuest;
            }
            if (bucket->next != nullptr) {
                bucket->next->prev = newGuest;
            }
            return true;
        }

        if (bucket->next == nullptr) {
            bucket->next = new GuestDetails(firstName, lastName, value, bucket);
            return true;
        }

        bucket = bucket->next;
    }
    return true;
}

bool WeddingGuest::alterGuest(const std::string& firstName, const std::string& lastName, const GuestType& value) {
    const unsigned int bucketIndex = this->hash(lastName);
    GuestDetails* current = this->table[bucketIndex];

    while (current != nullptr) {
        if (firstName == current->firstName && lastName == current->lastName) {
            current->value = value;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool WeddingGuest::inviteOrAlter(const std::string& firstName, const std::string& lastName, const GuestType& value) {
    return this->alterGuest(firstName, lastName, value) || this->inviteGuest(firstName, lastName, value);
}

bool WeddingGuest::crossGuestOff(const std::string& firstName, const std::string& lastName) {
    const unsigned int bucketIndex = this->hash(lastName);
    GuestDetails* bucket = this->table[bucketIndex];

    while (bucket != nullptr) {
        if (firstName == bucket->firstName && lastName == bucket->lastName) {
            if (bucket == this->table[bucketIndex]) {
                if (bucket->next == nullptr) {
                    delete this->table[bucketIndex];
                    this->table[bucketIndex] = nullptr;
                } else {
                    bucket->next->prev = nullptr;
                    delete this->table[bucketIndex];
                    this->table[bucketIndex] = bucket->next;
                }
            } else {
                bucket->prev->next = bucket->next;
                if (bucket->next != nullptr) {
                    bucket->next->prev = bucket->prev;
                }
                delete bucket;
            }
            return true;
        }
        bucket = bucket->next;
    }
    return false;
}

bool WeddingGuest::invitedToTheWedding(const std::string& firstName, const std::string& lastName) const {
    for (GuestDetails* current = this->table[this->hash(lastName)]; current != nullptr; current = current->next) {
        if (firstName == current->firstName && lastName == current->lastName) {
            return true;
        }
    }
    return false;
}

bool WeddingGuest::matchInvitedGuest(const std::string& firstName, const std::string& lastName, GuestType& value) const {
    for (GuestDetails* current = this->table[this->hash(lastName)]; current != nullptr; current = current->next) {
        if (firstName == current->firstName && lastName == current->lastName) {
            value = current->value;
            return true;
        }
    }
    return false;
}

bool WeddingGuest::verifyGuestOnTheList(int i, std::string& firstName, std::string& lastName, GuestType& value) const {
    int index = -1;
    for (unsigned int j = 0; j < this->BUCKET_SIZE; ++j) {
        for (GuestDetails* current = this->table[j]; current != nullptr; current = current->next) {
            ++index;
            if (index == i) {
                firstName = current->firstName;
                lastName = current->lastName;
                value = current->value;
                return true;
            }
            if (index > i) {
                return false;
            }
        }
    }
    return false;
}

void WeddingGuest::swapWeddingGuests(WeddingGuest& other) {
    WeddingGuest copy = *this;
    this->inviteGuests(other);
    other = copy;
}

bool joinGuests(const WeddingGuest& odOne, const WeddingGuest& odTwo, WeddingGuest& odJoined) {
    bool result = true;

    auto addGuest = [](const WeddingGuest& od, WeddingGuest& odJoined) -> bool {
        bool result = true;
        std::string firstName, lastName;
        GuestType value, existingValue;
        unsigned int index = 0;
        for (bool guestLeft = od.verifyGuestOnTheList(index, firstName, lastName, value); guestLeft; guestLeft = od.verifyGuestOnTheList(++index, firstName, lastName, value)) {
            if (odJoined.matchInvitedGuest(firstName, lastName, existingValue) && (value != existingValue)) {
                odJoined.crossGuestOff(firstName, lastName);
                result = false;
                continue;
            }
            odJoined.inviteGuest(firstName, lastName, value);
        }
        return result;
    };

    result = addGuest(odOne, odJoined);
    return (result = addGuest(odTwo, odJoined));
}

void attestGuests(const std::string& fsearch, const std::string& lsearch, const WeddingGuest& odOne, WeddingGuest& odResult) {
    std::string firstName, lastName;
    GuestType value;
    int index = 0;
    for (bool guestLeft = odOne.verifyGuestOnTheList(index, firstName, lastName, value); guestLeft; guestLeft = odOne.verifyGuestOnTheList(++index, firstName, lastName, value)) {
        if ((fsearch == "*" || firstName == fsearch) && (lsearch == "*" || lastName == lsearch)) {
            odResult.inviteGuest(firstName, lastName, value);
        }
    }
}

int main() {
    WeddingGuest wg;
    wg.inviteGuest("Corey", "Mostero", "v1");
    wg.inviteGuest("Corey", "Lyu", "v1");
    wg.inviteGuest("Corey", "Lostero", "v1");
    wg.inviteGuest("Corey", "Postero", "v1");
    wg.crossGuestOff("Corey", "Lyu");
    std::cout << wg.guestCount();

    return 0;
}