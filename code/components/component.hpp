#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <concepts>

template <class T>
concept IsComponent = requires(T pComponent) {
    pComponent.Process();
    pComponent.Draw();
};

template <class = void>
class DummyComponent
{
public:
private:
public:
    auto Process() -> void {}
    auto Draw() -> void {}
};

static_assert(IsComponent<DummyComponent<>>, "`DummyComponent` must be a component.");

#endif // COMPONENT_HPP