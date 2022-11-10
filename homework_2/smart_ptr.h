#ifndef SMART_PTR_H_
#define SMART_PTR_H_

#include <stdexcept>
#include <string>

class null_ptr_exception : public std::runtime_error {
   public:
    // Inherting constructor (to parent std::runtime_error class)
    null_ptr_exception(const std::string& what_arg = "A null_ptr_exception error has occurred") : std::runtime_error(what_arg) {
    }
};

template <typename T>
class smart_ptr {
   private:
    T* ptr_;
    unsigned int* ref_;

   public:
    ~smart_ptr() noexcept {
        // If the pre-decrement value of the reference count is 0: delete the ptr_ value
        if (this->ref_ != nullptr && --*this->ref_ == 0) {
            delete this->ptr_;
            delete this->ref_;
        }
    }

    smart_ptr() noexcept {
        this->ptr_ = nullptr;
        this->ref_ = nullptr;
    }

    explicit smart_ptr(T* raw_ptr) noexcept {
        // In the case that raw_ptr isn't nullptr, instantiate ptr_ and ref_ respectively.
        // It is also implied through my constructor implementations that smart_ptr objects
        // can only have both nullptr, or non-nullptr fields; no in-between.
        if (raw_ptr != nullptr) {
            this->ptr_ = raw_ptr;
            this->ref_ = new unsigned int(1);
            return;
        }
        this->ptr_ = nullptr;
        this->ref_ = nullptr;
    }

    smart_ptr(const smart_ptr& rhs) noexcept {
        // Simply copy rhs fields and increase the shared reference count variable in the case
        // the pointer of rhs isn't nullptr.
        // This conditional is also an example of only having to check a single smart_ptr field for nullptr,
        // as through my constructors' implementation, both are either nullptr or non-nullptr.
        if (rhs.ptr_ != nullptr) {
            this->ptr_ = rhs.ptr_;
            this->ref_ = rhs.ref_;
            ++*this->ref_;
            return;
        }
        this->ptr_ = nullptr;
        this->ref_ = nullptr;
    }

    smart_ptr(smart_ptr&& rhs) noexcept {
        this->ptr_ = rhs.ptr_;
        this->ref_ = rhs.ref_;

        rhs.ptr_ = nullptr;
        rhs.ref_ = nullptr;
    }

    smart_ptr& operator=(const smart_ptr& rhs) noexcept {
        // In the case that one attempts to assign a smart_ptr object to itself,
        // logically no action should be taken; hence the conditional
        if (this != &rhs) {
            // Due to the the reassignment of the current object, it is essential to check if
            // ptr_ is the last reference, as the destructor will never run in this instance
            if (this->ref_ != nullptr && --*this->ref_ == 0) {
                delete this->ptr_;
                delete this->ref_;
            }
            this->ptr_ = rhs.ptr_;
            this->ref_ = rhs.ref_;
            if (this->ref_ != nullptr) {
                ++*this->ref_;
            }
        }
        return *this;
    }

    smart_ptr& operator=(smart_ptr&& rhs) noexcept {
        // Conditional follows the same logic as the copy assignment overload function
        if (this != &rhs) {
            // Conditional follows the same logic as the copy assignment overload function
            if (this->ref_ != nullptr && --*this->ref_ == 0) {
                delete this->ptr_;
                delete this->ref_;
            }
            this->ptr_ = rhs.ptr_;
            this->ref_ = rhs.ref_;

            rhs.ptr_ = nullptr;
            rhs.ref_ = nullptr;
        }
        return *this;
    }

    bool clone() {
        if (this->ptr_ == nullptr || *this->ref_ == 1) {
            return false;
        }
        // Initialization will only work with primitive data types* and objects that have
        // an overloaded copy constructor
        // *Based on my current knowledge of cpp primitive data types

        T* oldPtr = this->ptr_;
        try {
            this->ptr_ = new T(*this->ptr_);
        } catch (std::bad_alloc) {
            this->ptr_ = oldPtr;
            throw std::bad_alloc();
        }

        unsigned int* newRef;
        try {
            newRef = new unsigned int(1);
        } catch (std::bad_alloc) {
            delete this->ptr_;
            this->ptr_ = oldPtr;
            throw std::bad_alloc();
        }

        --*this->ref_;
        this->ref_ = newRef;

        return true;
    }

    int ref_count() const noexcept {
        // Ternary operator can be expanded as such:
        // if (this->ref_ == nullptr)
        //     return 0
        // else
        //    return *this->ref_
        // end if
        return (this->ref_ == nullptr) ? 0 : *this->ref_;
    }

    T& operator*() {
        if (this->ptr_ == nullptr) {
            throw null_ptr_exception();
        }
        return *this->ptr_;
    }

    T* operator->() {
        if (this->ptr_ == nullptr) {
            throw null_ptr_exception();
        }
        return this->ptr_;
    }
};

#endif