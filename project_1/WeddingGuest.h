#ifndef WEDDING_GUEST_H
#define WEDDING_GUEST_H

#include <string>

typedef std::string GuestType;  // This can change to other types such
                                // as double and int, not just string
class WeddingGuest {
   private:
    class GuestDetails {
       public:
        std::string firstName;
        std::string lastName;
        GuestType value;

        GuestDetails* prev;
        GuestDetails* next;

        GuestDetails(std::string firstName = "", std::string lastName = "", GuestType value = "", GuestDetails* prev = nullptr, GuestDetails* next = nullptr) {
            this->firstName = firstName;
            this->lastName = lastName;
            this->value = value;
            this->prev = prev;
            this->next = next;
        }
    };

    const unsigned int BUCKET_SIZE = 27;

    GuestDetails** table;

    char toLower(char letter) const;

    bool isLower(char letter) const;

    bool isUpper(char letter) const;

    unsigned int hash(std::string lastName);

    unsigned int hash(GuestDetails* details);

    // int compare(const std::string stringOne, const std::string stringTwo) const;

   public:
    int compare(const std::string stringOne, const std::string stringTwo) const;
    WeddingGuest();  // Create an empty WeddingGuest list
    ~WeddingGuest();
    bool noGuests() const;  // Return true if the WeddingGuest list
    // is empty, otherwise false.
    int guestCount() const;  // Return the number of matches
    // on the WeddingGuest list.
    bool inviteGuest(const std::string& firstName, const std::string& lastName, const GuestType& value);
    // If the full name (both the first and last name) is not equal
    // to any full name currently in the list then add it and return
    // true. Elements should be added according to their last name.
    // Elements with the same last name should be added according to
    // their first names. Otherwise, make no change to the list and
    // return false (indicating that the name is already in the
    // list).
    bool alterGuest(const std::string& firstName, const std::string& lastName, const GuestType& value);
    // If the full name is equal to a full name currently in the
    // list, then make that full name no longer map to the value it
    // currently maps to, but instead map to the value of the third
    // parameter; return true in this case. Otherwise, make no
    // change to the list and return false.
    bool inviteOrAlter(const std::string& firstName, const std::string& lastName, const GuestType& value);
    // If full name is equal to a name currently in the list, then
    // make that full name no longer map to the value it currently
    // maps to, but instead map to the value of the third parameter;
    // return true in this case. If the full name is not equal to
    // any full name currently in the list then add it and return
    // true. In fact, this function always returns true.
    bool crossGuestOff(const std::string& firstName, const std::string& lastName);
    // If the full name is equal to a full name currently in the
    // list, remove the full name and value from the list and return
    // true. Otherwise, make no change to the list and return
    // false.
    bool invitedToTheWedding(const std::string& firstName, const std::string& lastName) const;
    // Return true if the full name is equal to a full name
    // currently in the list, otherwise false.
    bool matchInvitedGuest(const std::string& firstName, const std::string& lastName, GuestType& value) const;
    // If the full name is equal to a full name currently in the
    // list, set value to the value in the list that that full name
    // maps to, and return true. Otherwise, make no change to the
    // value parameter of this function and return false.
    bool verifyGuestOnTheList(int i, std::string& firstName,
                              std::string& lastName, GuestType& value) const;
    // If 0 <= i < size(), copy into firstName, lastName and value
    // parameters the corresponding information of the element at
    // position i in the list and return true. Otherwise, leave the
    // parameters unchanged and return false. (See below for details
    // about this function.)

    void swapWeddingGuests(WeddingGuest& other);
    // Exchange the contents of this list with the other one.
};

#endif