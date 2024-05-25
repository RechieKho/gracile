#ifndef GROUP_HPP
#define GROUP_HPP

#include "def.hpp"
#include "component.hpp"

template <IsComponent First, IsComponent... Rest>
class Group : public Group<Rest...>
{
public:
    static constexpr const std::size_t COUNT = 1 + sizeof...(Rest);
    using ValueType = First;

private:
protected:
    ValueType component;

public:
    Group(ValueType pValue, Rest... pRest) : Group<Rest...>(pRest...), component(pValue) {}

    template <std::size_t Index>
    auto Get() -> TypeAt<Index, First, Rest...>::CurrentType
    {
        static_assert(Index < COUNT, "Index must not be bigger than component count.");
        return TypeAt<Index, First, Rest...>::template AllType<Group>::component;
    }

    template <>
    auto Get<0>() -> ValueType
    {
        return component;
    }

    auto Process() -> void
    {
        component.Process();
        Group<Rest...>::Process();
    }

    auto Draw() -> void
    {
        component.Draw();
        Group<Rest...>::Draw();
    }
};

template <IsComponent Component>
class Group<Component>
{
public:
    static constexpr const std::size_t COUNT = 1;
    using ValueType = Component;

private:
protected:
    ValueType component;

public:
    Group(ValueType pValue) : component(pValue) {}

    template <std::size_t Index>
    auto Get() -> ValueType
    {
        static_assert(Index < COUNT, "Index must not be bigger than component count.");
        return component;
    }

    auto Process() -> void
    {
        component.Process();
    }

    auto Draw() -> void
    {
        component.Draw();
    }
};

static_assert(IsComponent<Group<DummyComponent<>>>, "`Group` must be a component.");

#endif // GROUP_HPP