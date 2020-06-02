#ifndef ENTT_ENTITY_RANGE_HPP
#define ENTT_ENTITY_RANGE_HPP

#include "utility.hpp"

namespace entt
{
template <typename...>
class components_iterator;

template <typename Entity, typename... Exclude, typename... Component>
class components_iterator<Entity, exclude_t<Exclude...>, Component...> {
public:
    using value_type = std::tuple<Entity, Component...>;
    using reference = std::tuple<Entity, Component&...>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using iterator_category = std::input_iterator_tag;

    using sparse_set_iterator = typename sparse_set<Entity>::iterator;

    using pool_set_type = typename pool_set<Entity, Exclude..., Component...>::tuple_type;

    components_iterator(const pool_set_type &pools, sparse_set_iterator underlying) : pools{pools}, underlying{underlying} {}

    reference operator*() {
        return reference{*underlying, std::get<pool_type<Component> *>(pools)->get(*underlying)...};
    }

    components_iterator &operator++() {
        underlying++;
        return *this;
    }

    bool operator==(components_iterator const &other) {
        return underlying == other.underlying;
    }

    bool operator!=(components_iterator const &other) {
        return !operator==(other);
    }

private:
    const pool_set_type pools;
    sparse_set_iterator underlying;
};

template <typename...>
class components_range;
template <typename Entity, typename... Exclude, typename... Component>
class components_range<Entity, exclude_t<Exclude...>, Component...> {
    using iterator_type = components_iterator<Entity, exclude_t<Exclude...>, Component...>;

    const sparse_set<Entity> & get_smallest_pool() const ENTT_NOEXCEPT {
        return *std::min({ static_cast<const sparse_set<Entity> *>(std::get<pool_type<Component> *>(pools))... }, [](const auto *lhs, const auto *rhs) {
            return lhs->size() < rhs->size();
        });
    }

    using pool_set_type = typename iterator_type::pool_set_type;

public:
    explicit components_range(const pool_set_type &pools) : pools{pools} {}

    iterator_type begin() {
        return iterator_type{pools, get_smallest_pool().begin()};
    }

    iterator_type end() {
        return iterator_type{pools, get_smallest_pool().end()};
    }

private:
    const pool_set_type pools;
};
}

#endif
