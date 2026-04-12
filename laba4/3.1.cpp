#include <array>
#include <tuple>
#include <iostream>
#include <utility>
#include <type_traits>

template <typename... Arrays>
struct CartesianProduct;

template <typename FirstArray, typename... RemainingArrays>
struct CartesianProduct<FirstArray, RemainingArrays...> {
    using FirstType = typename FirstArray::value_type;
    using RemainingType = typename CartesianProduct<RemainingArrays...>::type;
    
    static constexpr std::size_t first_size = std::tuple_size_v<FirstArray>;
    static constexpr std::size_t remaining_size = CartesianProduct<RemainingArrays...>::total_size;
    static constexpr std::size_t total_size = first_size * remaining_size;
    
    using type = std::array<RemainingType, total_size>;
    
    template <typename... Collected>
    static constexpr type build(const FirstArray& first, const RemainingArrays&... rest,
                                Collected&&... collected) {
        return build_impl<0>(first, rest..., std::index_sequence<>{}, std::forward<Collected>(collected)...);
    }
    
private:
    template <std::size_t I, typename... Collected, typename... Rest>
    static constexpr auto build_impl(const FirstArray& first, const Rest&... rest,
                                     std::index_sequence<Collected...>, Collected&&... collected) {
        if constexpr (I < first_size) {
            return concat_arrays(
                CartesianProduct<RemainingArrays...>::build(rest..., collected..., first[I]),
                build_impl<I + 1>(first, rest..., std::index_sequence<Collected...>{}, collected...)
            );
        } else {
            return type{};
        }
    }
    
    template <typename T, std::size_t N, std::size_t M>
    static constexpr auto concat_arrays(const std::array<T, N>& a, const std::array<T, M>& b) {
        std::array<T, N + M> result{};
        for (std::size_t i = 0; i < N; ++i) result[i] = a[i];
        for (std::size_t i = 0; i < M; ++i) result[N + i] = b[i];
        return result;
    }
};

template <typename LastArray>
struct CartesianProduct<LastArray> {
    using LastType = typename LastArray::value_type;
    static constexpr std::size_t total_size = std::tuple_size_v<LastArray>;
    using type = std::array<std::tuple<LastType>, total_size>;
    
    template <typename... Collected>
    static constexpr type build(const LastArray& last, Collected&&... collected) {
        return build_impl<0>(last, collected...);
    }
    
private:
    template <std::size_t I, typename... Collected>
    static constexpr auto build_impl(const LastArray& last, Collected&&... collected) {
        if constexpr (I < std::tuple_size_v<LastArray>) {
            type result{};
            result[I] = std::make_tuple(collected..., last[I]);
            if constexpr (I + 1 < std::tuple_size_v<LastArray>) {
                auto rest = build_impl<I + 1>(last, collected...);
                for (std::size_t j = I + 1; j < total_size; ++j) {
                    result[j] = rest[j - (I + 1)];
                }
            }
            return result;
        } else {
            return type{};
        }
    }
};

template <typename... Arrays>
constexpr auto full_cartesian(const Arrays&... arrays) {
    return CartesianProduct<Arrays...>::build(arrays...);
}

template <std::size_t Idx, typename Array>
constexpr void validate_index() {
    static_assert(Idx < std::tuple_size_v<std::remove_reference_t<Array>>,
                  "index out of bounds");
}

template <std::size_t FirstIdx, std::size_t... RestIdxs, typename FirstArray, typename... RestArrays>
constexpr void validate_all_indices() {
    validate_index<FirstIdx, FirstArray>();
    if constexpr (sizeof...(RestIdxs) > 0) {
        validate_all_indices<RestIdxs..., RestArrays...>();
    }
}

template <std::size_t... Indices, typename... Arrays>
constexpr auto select_elements(const std::tuple<Arrays...>& containers,
                               std::index_sequence<Indices...>) {
    return std::make_tuple(std::get<Indices>(containers)[Indices]...);
}

template <typename... Arrays, typename... Indices>
constexpr auto element_by_indices(const Arrays&... containers, Indices... positions) {
    static_assert(sizeof...(Arrays) == sizeof...(Indices),
                  "number of indices must match number of arrays");
    
    validate_all_indices<Indices..., Arrays...>();
    
    auto bundle = std::tie(containers...);
    return select_elements(bundle, std::index_sequence<positions...>{});
}

int main() {
    constexpr std::array<int, 2> digits = {1, 2};
    constexpr std::array<char, 3> letters = {'a', 'b', 'c'};
    constexpr std::array<float, 2> floats = {0.1f, 0.2f};
    
    constexpr auto product = full_cartesian(digits, letters);
    
    std::cout << "digits x letters (2x3=6):\n";
    for (std::size_t i = 0; i < product.size(); ++i) {
        std::cout << "product[" << i << "] = ("
                  << std::get<0>(product[i]) << ", "
                  << std::get<1>(product[i]) << ")\n";
    }
    
    constexpr auto elem = element_by_indices(digits, letters, floats, 1, 2, 0);
    std::cout << "\nelement_by_indices(1,2,0) = ("
              << std::get<0>(elem) << ", "
              << std::get<1>(elem) << ", "
              << std::get<2>(elem) << ")\n";
    
    constexpr auto elem2 = element_by_indices(digits, letters, 0, 1);
    std::cout << "element_by_indices(0,1) = ("
              << std::get<0>(elem2) << ", "
              << std::get<1>(elem2) << ")\n";
    
    static_assert(std::get<0>(product[0]) == 1);
    static_assert(std::get<1>(product[0]) == 'a');
    static_assert(std::get<0>(product[3]) == 2);
    static_assert(std::get<1>(product[3]) == 'a');
    
    return 0;
}