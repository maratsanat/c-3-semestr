#include <array>
#include <iostream>
#include <type_traits>

template <typename NumberType, std::size_t Order, std::size_t Target>
struct RecurrentSequence {
private:
    template <std::size_t CoefficientIndex>
    static constexpr NumberType weighted_sum(const std::array<NumberType, Order>& initial,
                                              const std::array<NumberType, Order>& factors) {
        if constexpr (CoefficientIndex >= Order) {
            return NumberType(0);
        } else {
            return factors[CoefficientIndex] * 
                   RecurrentSequence<NumberType, Order, Target - Order + CoefficientIndex>::compute(initial, factors) +
                   weighted_sum<CoefficientIndex + 1>(initial, factors);
        }
    }
    
public:
    static constexpr NumberType compute(const std::array<NumberType, Order>& initial,
                                        const std::array<NumberType, Order>& factors) {
        if constexpr (Target < Order) {
            return initial[Target];
        } else {
            return weighted_sum<0>(initial, factors);
        }
    }
};

template <typename NumberType, std::size_t Order, std::size_t Target>
constexpr NumberType get_recurrent_value(const std::array<NumberType, Order>& start,
                                         const std::array<NumberType, Order>& coefficients) {
    return RecurrentSequence<NumberType, Order, Target>::compute(start, coefficients);
}

int main() {
    constexpr std::array<int, 2> fib_start = {0, 1};
    constexpr std::array<int, 2> fib_coeff = {1, 1};
    
    std::cout << "Fibonacci:\n";
    for (int n = 0; n <= 10; ++n) {
        constexpr auto val = get_recurrent_value<int, 2, n>(fib_start, fib_coeff);
        std::cout << "F(" << n << ") = " << val << "\n";
    }
    static_assert(get_recurrent_value<int, 2, 10>(fib_start, fib_coeff) == 55);
    std::cout << "\n";
    
    constexpr std::array<double, 3> custom_start = {1.0, 2.0, 3.0};
    constexpr std::array<double, 3> custom_coeff = {2.0, 3.0, 1.0};
    
    std::cout << "Custom recurrence order 3:\n";
    constexpr double x3 = get_recurrent_value<double, 3, 3>(custom_start, custom_coeff);
    constexpr double x4 = get_recurrent_value<double, 3, 4>(custom_start, custom_coeff);
    constexpr double x5 = get_recurrent_value<double, 3, 5>(custom_start, custom_coeff);
    
    std::cout << "x3 = " << x3 << "\n";
    std::cout << "x4 = " << x4 << "\n";
    std::cout << "x5 = " << x5 << "\n\n";
    
    constexpr std::array<long long, 1> geo_start = {3};
    constexpr std::array<long long, 1> geo_coeff = {2};
    
    std::cout << "Geometric progression:\n";
    for (int n = 0; n <= 8; ++n) {
        constexpr auto val = get_recurrent_value<long long, 1, n>(geo_start, geo_coeff);
        std::cout << "x[" << n << "] = " << val << "\n";
    }
    std::cout << "\n";
    
    constexpr std::array<int, 1> const_start = {42};
    constexpr std::array<int, 1> const_coeff = {1};
    
    std::cout << "Constant sequence:\n";
    for (int n = 0; n <= 5; ++n) {
        constexpr auto val = get_recurrent_value<int, 1, n>(const_start, const_coeff);
        std::cout << "x[" << n << "] = " << val << "\n";
    }
    
    return 0;
}