#ifndef FORWARDLIST_HPP
#define FORWARDLIST_HPP

template<typename T>
class subforwardlist {
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* begin;

public:
    subforwardlist() : begin(nullptr) {}
    
    ~subforwardlist() {
        clear();
    }

    subforwardlist(const subforwardlist& other) : begin(nullptr) {
        Node* other_current = other.begin;
        while (other_current) {
            push_back(other_current->data);
            other_current = other_current->next;
        }
    }

    subforwardlist& operator=(const subforwardlist& other) {
        if (this != &other) {
            clear();
            Node* other_current = other.begin;
            while (other_current) {
                push_back(other_current->data);
                other_current = other_current->next;
            }
        }
        return *this;
    }

    subforwardlist(subforwardlist&& other) : begin(other.begin) {
        other.begin = nullptr;
    }

    subforwardlist& operator=(subforwardlist&& other) {
        if (this != &other) {
            clear();
            begin = other.begin;
            other.begin = nullptr;
        }
        return *this;
    }

    void push_back(const T& data) {
        Node* new_node = new Node(data);
        if (!begin) {
            begin = new_node;
        } else {
            Node* current = begin;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
    }

    T pop_back() {
        if (!begin) return T{};
        
        if (!begin->next) {
            T data = begin->data;
            delete begin;
            begin = nullptr;
            return data;
        }
        
        Node* current = begin;
        while (current->next->next) {
            current = current->next;
        }
        T data = current->next->data;
        delete current->next;
        current->next = nullptr;
        return data;
    }

    void push_forward(const T& data) {
        Node* new_node = new Node(data);
        new_node->next = begin;
        begin = new_node;
    }

    T pop_forward() {
        if (!begin) return T{};
        
        Node* temp = begin;
        T data = begin->data;
        begin = begin->next;
        delete temp;
        return data;
    }

    void push_where(unsigned int where, const T& data) {
        if (where == 0) {
            push_forward(data);
            return;
        }
        
        Node* current = begin;
        for (unsigned int i = 0; i < where - 1 && current; i++) {
            current = current->next;
        }
        
        if (!current) {
            push_back(data);
            return;
        }
        
        Node* new_node = new Node(data);
        new_node->next = current->next;
        current->next = new_node;
    }

    T erase_where(unsigned int where) {
        if (!begin) return T{};
        
        if (where == 0) {
            return pop_forward();
        }
        
        Node* current = begin;
        for (unsigned int i = 0; i < where - 1 && current; i++) {
            current = current->next;
        }
        
        if (!current || !current->next) return T{};
        
        Node* to_delete = current->next;
        T data = to_delete->data;
        current->next = to_delete->next;
        delete to_delete;
        return data;
    }

    unsigned int size() {
        unsigned int count = 0;
        Node* current = begin;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }

    void clear() {
        while (begin) {
            Node* temp = begin;
            begin = begin->next;
            delete temp;
        }
    }
};

#endif // FORWARDLIST_HPP