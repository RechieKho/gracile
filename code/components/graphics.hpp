#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <raylib.h>

#include "component.hpp"

template <class = void>
class Graphics
{
public:
    using ProcessingType = bool;

private:
public:
    auto Prepare() -> void {}

    auto Process(ProcessingType pProcessing) -> ProcessingType { return pProcessing; }

    auto Draw() -> void
    {
        ClearBackground(RAYWHITE);
        if (IsKeyDown(KEY_A))
            DrawCircle(10, 10, 5, MAROON);
    }
};

static_assert(IsComponent<Graphics<>>, "Graphics must be a component.");

#endif // GRAPHICS_HPP