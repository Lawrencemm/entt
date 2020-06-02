#ifndef ENTT_ENTITY_UTILITY_HPP
#define ENTT_ENTITY_UTILITY_HPP


#include "../core/type_traits.hpp"


namespace entt {


/**
 * @brief Alias for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
struct exclude_t: type_list<Type...> {};


/**
 * @brief Variable template for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr exclude_t<Type...> exclude{};


/**
 * @brief Alias for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
struct get_t: type_list<Type...>{};


/**
 * @brief Variable template for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr get_t<Type...> get{};

template <typename...>
class pool_set;

template <typename Entity, typename... Exclude, typename... Component>
class pool_set<Entity, exclude_t<Exclude...>, Component...> {
    // I could have used std::conditional_t ...
    template<typename Comp>
    struct pool { using type = storage<Entity, Comp>; };

    // ... if only MSVC didn't have a bug ...
    template<typename Comp>
    struct pool<const Comp> { using type = const storage<Entity, std::remove_const_t<Comp>>; };

    // ... that forces me to do the same in a worse way! :(
    template<typename Comp>
    using pool_type = typename pool<Comp>::type;

    using tuple_type = std::tuple<pool_type<Component> *..., pool_type<Exclude> *...>;

public:
    pool_set(const pool_set &other) : pools{other.pools} {}

private:
    tuple_type pools;
};
}


#endif
