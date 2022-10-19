#ifndef SMART_PTR_H_
#define SMART_PTR_H_

#include <stdexcept>

class null_ptr_exception : public std::runtime_error {
   public:
    null_ptr_exception(const std::string& what_arg = "A null_ptr_exception error has occurred") : std::runtime_error(what_arg) {
    }
};

template <typename T>
class smart_ptr {
   private:
    T* ptr_;
    unsigned int* ref_;

   public:
    ~smart_ptr() {
        if (this->ref_ != nullptr && --*this->ref_ == 0) {
            delete this->ptr_;
            delete this->ref_;
        }
    }

    smart_ptr() {
        this->ptr_ = nullptr;
        this->ref_ = nullptr;
    }

    explicit smart_ptr(T* raw_ptr) {
        if (raw_ptr != nullptr) {
            this->ptr_ = raw_ptr;
            this->ref_ = new unsigned int(1);
            return;
        }
        this->ptr_ = nullptr;
        this->ref_ = nullptr;
    }

    smart_ptr(const smart_ptr& rhs) {
        if (rhs.ptr_ != nullptr) {
            this->ptr_ = rhs.ptr_;
            this->ref_ = rhs.ref_;
            ++*this->ref_;
            return;
        }
        this->ptr_ = nullptr;
        this->ref_ = nullptr;
    }

    smart_ptr(smart_ptr&& rhs) {
        this->ptr_ = rhs.ptr_;
        this->ref_ = rhs.ref_;

        rhs.ptr_ = nullptr;
        rhs.ref_ = nullptr;
    }

    smart_ptr& operator=(const smart_ptr& rhs) {
        if (this != &rhs) {
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

    smart_ptr& operator=(smart_ptr&& rhs) {
        if (this != &rhs) {
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
        if (this == nullptr || *this->ref_ == 1) {
            return false;
        }

        --*this->ref_;
        this->ptr_ = new T(*this->ptr_);
        this->ref_ = new unsigned int(1);
        return true;
    }

    int ref_count() const {
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

    explicit operator bool() const {
        return this->ptr_ != nullptr;
    }
};

#endif