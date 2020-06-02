#include <utility>
#include <type_traits>
#include <gtest/gtest.h>
#include <entt/entity/registry.hpp>
#include <entt/entity/view.hpp>

struct empty_type {};

TEST(Ranges, MultiComponents) {
    entt::registry registry;

    const auto e0 = registry.create();
    registry.emplace<int>(e0, 10);
    registry.emplace<char>(e0);

    const auto e1 = registry.create();
    registry.emplace<int>(e1, 8);
    registry.emplace<char>(e1);

    auto view = registry.view<int, char>();
    std::size_t cnt = 0;

    for (auto [ent, _int, _char] : view.each()) { ++cnt; _int++; };

    ASSERT_EQ(cnt, std::size_t{2});
    ASSERT_EQ(registry.get<int>(e0), 11);
    ASSERT_EQ(registry.get<int>(e1), 9);

    auto cview = std::as_const(registry).view<const int, const char>();

    for (auto [ent, _int, _char] : cview.each()) { ++cnt; };

    ASSERT_EQ(cnt, std::size_t{4});
    ASSERT_EQ(registry.get<int>(e0), 11);
    ASSERT_EQ(registry.get<int>(e1), 9);

    auto const &const_view = view;
}

TEST(Ranges, SingleComponent) {
    entt::registry registry;

    registry.emplace<int>(registry.create());
    registry.emplace<int>(registry.create());

    auto view = registry.view<int>();
    std::size_t cnt = 0;

    for (auto [entity, _int] : view.each())
    {
        ++cnt;
    }

    ASSERT_EQ(cnt, std::size_t{2});
}
