
#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <string>

template <typename ValueType, typename UnderlyingContainer = std::deque<ValueType>>
class FlexibleQueue {
private:
    UnderlyingContainer storage;
    
public:
    void enqueue(const ValueType& item) {
        storage.push_back(item);
    }
    
    void dequeue() {
        if (!storage.empty()) {
            storage.pop_front();
        }
    }
    
    ValueType& front() {
        return storage.front();
    }
    
    const ValueType& front() const {
        return storage.front();
    }
    
    ValueType& back() {
        return storage.back();
    }
    
    const ValueType& back() const {
        return storage.back();
    }
    
    std::size_t get_size() const {
        return storage.size();
    }
    
    bool is_void() const {
        return storage.empty();
    }
};

template <>
class FlexibleQueue<char, std::string> {
private:
    std::string storage;
    
public:
    void enqueue(char item) {
        storage.push_back(item);
    }
    
    void dequeue() {
        if (!storage.empty()) {
            storage.erase(storage.begin());
        }
    }
    
    char& front() {
        return storage.front();
    }
    
    const char& front() const {
        return storage.front();
    }
    
    char& back() {
        return storage.back();
    }
    
    const char& back() const {
        return storage.back();
    }
    
    std::size_t get_size() const {
        return storage.size();
    }
    
    bool is_void() const {
        return storage.empty();
    }
};

int main() {
    FlexibleQueue<int> q1;
    q1.enqueue(10);
    q1.enqueue(20);
    q1.enqueue(30);
    q1.enqueue(40);
    
    std::cout << "Queue<int> size: " << q1.get_size() << "\n";
    std::cout << "front: " << q1.front() << ", back: " << q1.back() << "\n";
    q1.dequeue();
    std::cout << "after dequeue, front: " << q1.front() << "\n";
    std::cout << "empty? " << (q1.is_void() ? "yes" : "no") << "\n\n";
    
    FlexibleQueue<double, std::vector<double>> q2;
    q2.enqueue(1.5);
    q2.enqueue(2.7);
    q2.enqueue(3.9);
    
    std::cout << "Queue<double> on vector size: " << q2.get_size() << "\n";
    std::cout << "front: " << q2.front() << ", back: " << q2.back() << "\n\n";
    
    FlexibleQueue<std::string, std::list<std::string>> q3;
    q3.enqueue("first");
    q3.enqueue("second");
    q3.enqueue("third");
    
    std::cout << "Queue<string> on list size: " << q3.get_size() << "\n";
    std::cout << "front: " << q3.front() << ", back: " << q3.back() << "\n";
    q3.dequeue();
    std::cout << "after dequeue, front: " << q3.front() << "\n\n";
    
    FlexibleQueue<char, std::string> char_queue;
    char_queue.enqueue('A');
    char_queue.enqueue('B');
    char_queue.enqueue('C');
    char_queue.enqueue('D');
    
    std::cout << "Char queue (specialization) size: " << char_queue.get_size() << "\n";
    std::cout << "front: " << char_queue.front() << ", back: " << char_queue.back() << "\n";
    
    return 0;
}