#ifndef SUBVECTOR_HPP
#define SUBVECTOR_HPP

template<typename T>
class subvector {
public:
    T *mas;
    unsigned int top;
    unsigned int capacity;

    subvector() : mas(nullptr), top(0), capacity(0) {}

    ~subvector() {
        delete[] mas;
    }

    subvector(const subvector& other) : mas(nullptr), top(0), capacity(0) {
        if (other.capacity > 0) {
            mas = new T[other.capacity];
            capacity = other.capacity;
            top = other.top;
            for (unsigned int i = 0; i < top; i++) {
                mas[i] = other.mas[i];
            }
        }
    }

    subvector& operator=(const subvector& other) {
        if (this != &other) {
            delete[] mas;
            mas = nullptr;
            top = 0;
            capacity = 0;
            
            if (other.capacity > 0) {
                mas = new T[other.capacity];
                capacity = other.capacity;
                top = other.top;
                for (unsigned int i = 0; i < top; i++) {
                    mas[i] = other.mas[i];
                }
            }
        }
        return *this;
    }

    bool push_back(const T& d) {
        if (top >= capacity) {
            unsigned int new_capacity = (capacity == 0) ? 1 : capacity * 2;
            if (!resize(new_capacity)) {
                return false;
            }
        }
        mas[top] = d;
        top++;
        return true;
    }

    T pop_back() {
        if (top == 0) {
            return T{};
        }
        top--;
        return mas[top];
    }

    bool resize(unsigned int new_capacity) {
        if (new_capacity == capacity) {
            return true;
        }
        
        if (new_capacity == 0) {
            delete[] mas;
            mas = nullptr;
            top = 0;
            capacity = 0;
            return true;
        }
        
        T* new_mas = new T[new_capacity];
        unsigned int elements_to_copy = (top < new_capacity) ? top : new_capacity;
        
        for (unsigned int i = 0; i < elements_to_copy; i++) {
            new_mas[i] = mas[i];
        }
        
        delete[] mas;
        mas = new_mas;
        capacity = new_capacity;
        top = elements_to_copy;
        
        return true;
    }

    void shrink_to_fit() {
        if (top == 0) {
            delete[] mas;
            mas = nullptr;
            capacity = 0;
            return;
        }
        
        if (top == capacity) {
            return;
        }
        
        T* new_mas = new T[top];
        for (unsigned int i = 0; i < top; i++) {
            new_mas[i] = mas[i];
        }
        
        delete[] mas;
        mas = new_mas;
        capacity = top;
    }

    void clear() {
        top = 0;
    }
};

#endif // SUBVECTOR_HPP