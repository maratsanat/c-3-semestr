#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <type_traits>
#include <utility>
#include <iterator>

template <typename Iterator, typename = void>
struct is_random_access : std::false_type {};

template <typename Iterator>
struct is_random_access<Iterator,
    std::void_t<typename std::iterator_traits<Iterator>::iterator_category>>
    : std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category,
                          std::random_access_iterator_tag> {};

template <typename Iter, typename Comparator>
void verified_sort(Iter begin, Iter end, Comparator cmp) {
    static_assert(is_random_access<Iter>::value,
                  "verified_sort requires random access iterators");
    
    auto size = std::distance(begin, end);
    for (auto i = 0; i < size - 1; ++i) {
        for (auto j = 0; j < size - i - 1; ++j) {
            if (cmp(*(begin + j + 1), *(begin + j))) {
                std::iter_swap(begin + j, begin + j + 1);
            }
        }
    }
}

template <typename Container, typename ValueType, typename = void>
struct has_stack_methods : std::false_type {};

template <typename Container, typename ValueType>
struct has_stack_methods<Container, ValueType,
    std::void_t<
        decltype(std::declval<Container>().push_back(std::declval<ValueType>())),
        decltype(std::declval<Container>().pop_back()),
        decltype(std::declval<Container>().back()),
        decltype(std::declval<Container>().size()),
        decltype(std::declval<Container>().empty())
    >> : std::true_type {};

template <typename ElementType, typename BaseContainer = std::deque<ElementType>>
class SafeStack {
    static_assert(has_stack_methods<BaseContainer, ElementType>::value,
                  "container does not support stack interface");
    
private:
    BaseContainer data_;
    
public:
    void insert(ElementType val) {
        data_.push_back(val);
    }
    
    void remove() {
        if (!data_.empty()) data_.pop_back();
    }
    
    ElementType& peek() {
        return data_.back();
    }
    
    const ElementType& peek() const {
        return data_.back();
    }
    
    std::size_t count() const {
        return data_.size();
    }
    
    bool blank() const {
        return data_.empty();
    }
};

int main() {
    std::vector<int> vec_data = {64, 25, 12, 22, 11};
    std::cout << "vector before: ";
    for (int x : vec_data) std::cout << x << " ";
    verified_sort(vec_data.begin(), vec_data.end(), std::less<>());
    std::cout << "\nvector after: ";
    for (int x : vec_data) std::cout << x << " ";
    std::cout << "\n";
    
    std::deque<int> deq_data = {64, 25, 12, 22, 11};
    std::cout << "deque before: ";
    for (int x : deq_data) std::cout << x << " ";
    verified_sort(deq_data.begin(), deq_data.end(), std::less<>());
    std::cout << "\ndeque after: ";
    for (int x : deq_data) std::cout << x << " ";
    std::cout << "\n\n";
    
    SafeStack<int> s1;
    s1.insert(100);
    s1.insert(200);
    s1.insert(300);
    std::cout << "Stack<int> on deque: top=" << s1.peek() << ", size=" << s1.count() << "\n";
    s1.remove();
    std::cout << "after pop: top=" << s1.peek() << ", size=" << s1.count() << "\n";
    std::cout << "empty? " << (s1.blank() ? "yes" : "no") << "\n";
    
    SafeStack<std::string, std::vector<std::string>> s2;
    s2.insert("one");
    s2.insert("two");
    s2.insert("three");
    std::cout << "\nStack<string> on vector: top=" << s2.peek() << ", size=" << s2.count() << "\n";
    
    return 0;
}