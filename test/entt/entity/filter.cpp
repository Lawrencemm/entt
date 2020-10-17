#include <gtest/gtest.h>
#include <entt/entity/filter.h>


TEST(filter, iteration) {
    entt::registry registry;

    auto int_entity = registry.create();
    registry.emplace<int>(int_entity, 7);
    registry.emplace<char>(int_entity, 'A');

    auto float_entity = registry.create();
    registry.emplace<char>(float_entity, 'B');
    registry.emplace<float>(float_entity, 1.5f);

    using namespace entt::filter;

    for (auto ent : registry | has<char> | exclude<float> | entities) {
        assert(registry.get<char>(ent) == 'A');
        assert(registry.get<int>(ent) == 7);
    }

    for (auto[ent, letter, num] : registry | has<char> | has<int> | components) {
        assert(letter == 'A');
        assert(num == 7);
    }

    for (auto ent : registry | has<float> | entities) {
        assert(registry.get<char>(ent) == 'B');
        assert(registry.get<float>(ent) == 1.5f);
    }

    for (auto[ent, letter, num] : registry | has<char> | has<float> | components) {
        assert(letter == 'B');
        assert(num == 1.5f);
    }
}
