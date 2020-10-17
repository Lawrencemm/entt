#pragma once

#include <utility>
#include <entt/entity/registry.hpp>
#include <entt/entity/view.hpp>

namespace entt::filter {
    template<typename...>
    struct basic_filter;

    template<typename Entity, typename... Components, typename... Exclude>
    struct basic_filter<Entity, type_list<Components...>, type_list<Exclude...>> {
        basic_registry<Entity> &registry;
    };

    template<typename... Component>
    class has_t {
    public:

    };

    template<typename Entity, typename... Component>
    auto operator|(basic_registry<Entity> &registry, has_t<Component...> has) {
        return basic_filter<Entity, type_list<Component...>, type_list<>>{registry};
    }

    template<typename Entity, typename... Component, typename... Exclude, typename... OtherComponent>
    auto operator|(basic_filter<Entity, type_list<Component...>, type_list<Exclude...>> lhs, has_t<OtherComponent...>) {
        return basic_filter<Entity, type_list<Component..., OtherComponent...>, type_list<Exclude...>>{lhs.registry};
    }

    template<typename... Component>
    inline constexpr has_t<Component...> has{};


    template<typename... Exclude>
    class exclude_t {
    public:

    };

    template<typename Entity, typename... Exclude>
    auto operator|(basic_registry<Entity> &registry, exclude_t<Exclude...> has) {
        return basic_filter<Entity, type_list<>, type_list<Exclude...>>{registry};
    }

    template<typename Entity, typename... Component, typename... Exclude, typename... OtherExclude>
    auto
    operator|(basic_filter<Entity, type_list<Component...>, type_list<Exclude...>> lhs, exclude_t<OtherExclude...>) {
        return basic_filter<Entity, type_list<Component...>, type_list<Exclude..., OtherExclude...>>{lhs.registry};
    }

    template<typename... Exclude>
    inline constexpr exclude_t<Exclude...> exclude{};


    struct entities_t {
    };

    inline constexpr entities_t entities{};

    template<typename Entity, typename... Component, typename... Exclude>
    auto operator|(basic_filter<Entity, type_list<Component...>, type_list<Exclude...>> filter, entities_t) {
        return filter.registry.template view<Component...>(entt::exclude<Exclude...>);
    }

    struct components_t {
    };

    inline constexpr components_t components{};

    template<typename Entity, typename... Component, typename... Exclude>
    auto operator|(basic_filter<Entity, type_list<Component...>, type_list<Exclude...>> filter, components_t) {
        return filter.registry.template view<Component...>(entt::exclude<Exclude...>).each();
    }
}
