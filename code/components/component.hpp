#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <concepts>

#include "def.hpp"

template <class Type>
concept IsComponent = requires(Type pComponent) {
    typename Type::ProcessingType;

    pComponent.Prepare();
    { pComponent.Process(std::declval<typename Type::ProcessingType>()) } -> std::same_as<typename Type::ProcessingType>;
    pComponent.Draw();
};

template <class = void>
class DummyComponent
{
public:
    using ProcessingType = bool;

private:
public:
    auto Prepare() -> void {}
    auto Process(ProcessingType pProcessing) -> ProcessingType {}
    auto Draw() -> void {}
};

static_assert(IsComponent<DummyComponent<>>, "`DummyComponent` must be a component.");

template <IsComponent First, IsComponent... Rest>
class ComponentGroup : public ComponentGroup<Rest...>
{
    static_assert(std::same_as<typename First::ProcessingType, typename TypeAt<0, Rest...>::CurrentType::ProcessingType>, "`ProcessingType` must be the same.");

public:
    static constexpr const std::size_t COUNT = 1 + sizeof...(Rest);
    using ProcessingType = typename First::ProcessingType;
    using ValueType = First;

private:
protected:
    ValueType component;

public:
    ComponentGroup(ValueType pValue, Rest... pRest) : ComponentGroup<Rest...>(pRest...), component(pValue) {}

    template <std::size_t Index>
    auto Get() -> TypeAt<Index, First, Rest...>::CurrentType
    {
        static_assert(Index < COUNT, "Index must not be bigger than component count.");
        return TypeAt<Index, First, Rest...>::template AllType<ComponentGroup>::component;
    }

    template <>
    auto Get<0>() -> ValueType
    {
        return component;
    }

    auto Prepare() -> void
    {
        component.Prepare();
        ComponentGroup<Rest...>::Prepare();
    }

    auto Process(ProcessingType pProcessing) -> ProcessingType
    {
        return ComponentGroup<Rest...>::Process(component.Process(pProcessing));
    }

    auto Draw() -> void
    {
        component.Draw();
        ComponentGroup<Rest...>::Draw();
    }
};

template <IsComponent Component>
class ComponentGroup<Component>
{
public:
    static constexpr const std::size_t COUNT = 1;
    using ProcessingType = typename Component::ProcessingType;
    using ValueType = Component;

private:
protected:
    ValueType component;

public:
    ComponentGroup(ValueType pValue) : component(pValue) {}

    template <std::size_t Index>
    auto Get() -> ValueType
    {
        static_assert(Index < COUNT, "Index must not be bigger than component count.");
        return component;
    }

    auto Prepare() -> void
    {
        component.Prepare();
    }

    auto Process(ProcessingType pProcessing) -> ProcessingType
    {
        return component.Process(pProcessing);
    }

    auto Draw() -> void
    {
        component.Draw();
    }
};

static_assert(IsComponent<ComponentGroup<DummyComponent<>>>, "`ComponentGroup` must be a component.");

#endif // COMPONENT_HPP