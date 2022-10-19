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
    T* _data;
    unsigned int* _referenceCount;

   public:
    ~smart_ptr() {
        if (this->_referenceCount != nullptr && --*this->_referenceCount == 0) {
            delete this->_data;
            delete this->_referenceCount;
        }
    }

    smart_ptr() {
        this->_data = nullptr;
        this->_referenceCount = nullptr;
    }

    explicit smart_ptr(T* raw_ptr) {
        if (raw_ptr != nullptr) {
            this->_data = raw_ptr;
            this->_referenceCount = new unsigned int(1);
            return;
        }
        this->_data = nullptr;
        this->_referenceCount = nullptr;
    }

    smart_ptr(const smart_ptr& rhs) {
        this->_data = rhs._data;
        this->_referenceCount = rhs._referenceCount;
        if (this->_referenceCount != nullptr) {
            ++*this->_referenceCount;
        }
    }

    smart_ptr(smart_ptr&& rhs) {
        this->_data = rhs._data;
        this->_referenceCount = rhs._referenceCount;

        rhs._data = nullptr;
        rhs._referenceCount = nullptr;
    }

    smart_ptr& operator=(const smart_ptr& rhs) {
        if (this != &rhs) {
            if (this->_referenceCount != nullptr && --*this->_referenceCount == 0) {
                delete this->_data;
                delete this->_referenceCount;
            }
            this->_data = rhs._data;
            this->_referenceCount = rhs._referenceCount;
            if (this->_referenceCount != nullptr) {
                ++*this->_referenceCount;
            }
        }
        return *this;
    }

    smart_ptr& operator=(smart_ptr&& rhs) {
        if (this != &rhs) {
            if (this->_referenceCount != nullptr && --*this->_referenceCount == 0) {
                delete this->_data;
                delete this->_referenceCount;
            }
            this->_data = rhs._data;
            this->_referenceCount = rhs._referenceCount;

            rhs._data = nullptr;
            rhs._referenceCount = nullptr;
        }
        return *this;
    }

    bool clone() {
        if (this == nullptr || *this->_referenceCount == 1) {
            return false;
        }

        --*this->_referenceCount;
        this->_data = new T(*this->_data);
        this->_referenceCount = new unsigned int(1);
        return true;
    }

    int ref_count() const {
        return (this->_referenceCount == nullptr) ? 0 : *this->_referenceCount;
    }

    T& operator*() {
        if (this->_data == nullptr) {
            throw null_ptr_exception();
        }
        return *this->_data;
    }

    T* operator->() {
        if (this->_data == nullptr) {
            throw null_ptr_exception();
        }
        return this->_data;
    }

    explicit operator bool() const {
        return this->_data != nullptr;
    }
};

#endif