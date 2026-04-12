#include <iostream>
#include <tuple>
#include <string>
#include <type_traits>

template <std::size_t Current, std::size_t Total, typename TupleType, typename Operation>
struct TupleWalker {
    static void walk(const TupleType& data, Operation op) {
        op(std::get<Current>(data));
        TupleWalker<Current + 1, Total, TupleType, Operation>::walk(data, op);
    }
};

template <std::size_t Total, typename TupleType, typename Operation>
struct TupleWalker<Total, Total, TupleType, Operation> {
    static void walk(const TupleType&, Operation) {
    }
};

template <typename... ElementTypes, typename UnaryOperation>
void apply_to_each(const std::tuple<ElementTypes...>& elements, UnaryOperation operation) {
    TupleWalker<0, sizeof...(ElementTypes), std::tuple<ElementTypes...>, UnaryOperation>::walk(elements, operation);
}

template <typename Operation, typename... Types>
auto transform_tuple(const std::tuple<Types...>& source, Operation op) {
    return std::tuple<decltype(op(std::declval<Types>()))...>(
        op(std::get<Types>(source))...
    );
}

template <std::size_t Idx, typename... Args>
struct TuplePrinter {
    static void print(const std::tuple<Args...>& t, std::ostream& out) {
        TuplePrinter<Idx - 1, Args...>::print(t, out);
        out << std::get<Idx - 1>(t);
        if constexpr (Idx < sizeof...(Args)) {
            out << ", ";
        }
    }
};

template <typename... Args>
struct TuplePrinter<0, Args...> {
    static void print(const std::tuple<Args...>&, std::ostream&) {}
};

template <typename... Args>
std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& t) {
    out << "(";
    TuplePrinter<sizeof...(Args), Args...>::print(t, out);
    out << ")";
    return out;
}

int main() {
    auto my_tuple = std::make_tuple(42, 3.14159, std::string("hello"), 'X', 100500);
    
    std::cout << "Original tuple: " << my_tuple << "\n";
    
    std::cout << "Print each element: ";
    apply_to_each(my_tuple, [](const auto& value) {
        std::cout << value << " ";
    });
    std::cout << "\n";
    
    auto multiplied = transform_tuple(my_tuple, [](const auto& value) -> auto {
        if constexpr (std::is_arithmetic_v<std::decay_t<decltype(value)>>) {
            return value * 2;
        } else {
            return value;
        }
    });
    
    std::cout << "After multiplying numbers by 2: " << multiplied << "\n";
    
    int total = 0;
    apply_to_each(my_tuple, [&total](const auto& value) {
        if constexpr (std::is_integral_v<std::decay_t<decltype(value)>>) {
            total += value;
        }
    });
    std::cout << "Sum of integers: " << total << "\n";
    
    int maximum = 0;
    apply_to_each(my_tuple, [&maximum](const auto& value) {
        if constexpr (std::is_integral_v<std::decay_t<decltype(value)>>) {
            if (value > maximum) maximum = value;
        }
    });
    std::cout << "Maximum integer: " << maximum << "\n";
    
    std::tuple<> empty_tuple;
    std::cout << "Empty tuple: " << empty_tuple << "\n";
    apply_to_each(empty_tuple, [](auto) {});
    std::cout << "apply_to_each on empty tuple works\n";
    
    return 0;
}