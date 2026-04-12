
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <chrono>
#include <random>
#include <iterator>
#include <type_traits>
#include <algorithm>

template <typename RandomIt, typename Compare>
void merge(RandomIt first, RandomIt middle, RandomIt last, Compare comp) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    std::vector<value_type> buffer;
    buffer.reserve(last - first);
    
    auto left = first;
    auto right = middle;
    
    while (left < middle && right < last) {
        if (comp(*left, *right)) {
            buffer.push_back(std::move(*left));
            ++left;
        } else {
            buffer.push_back(std::move(*right));
            ++right;
        }
    }
    
    while (left < middle) {
        buffer.push_back(std::move(*left));
        ++left;
    }
    
    while (right < last) {
        buffer.push_back(std::move(*right));
        ++right;
    }
    
    std::move(buffer.begin(), buffer.end(), first);
}

template <typename RandomIt, typename Compare>
void merge_sort(RandomIt first, RandomIt last, Compare comp) {
    static_assert(
        std::is_same_v<
            typename std::iterator_traits<RandomIt>::iterator_category,
            std::random_access_iterator_tag
        >,
        "merge_sort requires random access iterators"
    );
    
    auto size = std::distance(first, last);
    if (size <= 1) return;
    
    auto middle = first + size / 2;
    merge_sort(first, middle, comp);
    merge_sort(middle, last, comp);
    merge(first, middle, last, comp);
}

bool ascending(int x, int y) { return x < y; }

struct descending_order {
    bool operator()(int a, int b) const { return a > b; }
};

template <typename Container>
double performance_test(Container data) {
    auto begin = std::chrono::high_resolution_clock::now();
    merge_sort(data.begin(), data.end(), std::less<>());
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

int main() {
    std::vector<int> numbers1 = {64, 25, 12, 22, 11, 90, 33, 44, 55, 77};
    std::cout << "vector: ";
    for (int x : numbers1) std::cout << x << " ";
    merge_sort(numbers1.begin(), numbers1.end(), ascending);
    std::cout << "-> ";
    for (int x : numbers1) std::cout << x << " ";
    std::cout << "\n";
    
    std::deque<int> numbers2 = {64, 25, 12, 22, 11, 90, 33, 44, 55, 77};
    std::cout << "deque: ";
    for (int x : numbers2) std::cout << x << " ";
    merge_sort(numbers2.begin(), numbers2.end(), [](int a, int b) { return a < b; });
    std::cout << "-> ";
    for (int x : numbers2) std::cout << x << " ";
    std::cout << "\n";
    
    std::vector<int> numbers3 = {64, 25, 12, 22, 11, 90, 33, 44, 55, 77};
    std::cout << "vector descending: ";
    for (int x : numbers3) std::cout << x << " ";
    merge_sort(numbers3.begin(), numbers3.end(), descending_order());
    std::cout << "-> ";
    for (int x : numbers3) std::cout << x << " ";
    std::cout << "\n\n";
    
    std::cout << "not working: std::list, std::forward_list, std::set\n\n";
    
    const int ELEMENTS = 300000;
    std::mt19937 engine(42);
    std::uniform_int_distribution<int> dist(1, 1000000);
    
    std::vector<int> source;
    source.reserve(ELEMENTS);
    for (int i = 0; i < ELEMENTS; ++i) source.push_back(dist(engine));
    
    std::vector<int> vec_copy(source.begin(), source.end());
    std::deque<int> deq_copy(source.begin(), source.end());
    
    double vec_time = performance_test(vec_copy);
    double deq_time = performance_test(deq_copy);
    
    std::cout << "Vector : " << vec_time << " ms\n";
    std::cout << "Deque : " << deq_time << " ms\n";
    std::cout << "faster: " << (vec_time < deq_time ? "VECTOR" : "DEQUE") << "\n";
    
    return 0;
}