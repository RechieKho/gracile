#include <raylib.h>
#include <string>

#include "components/component.hpp"
#include "app.hpp"

static_assert(IsComponent<decltype(app)>, "App must be a component.");

constexpr const int defaultScreenWidth = 800;
constexpr const int defaultScreenHeight = 450;
constexpr const int defaultTargetFPS = 60;
constexpr const std::string_view defaultTitle = "Hello world";

int main()
{
    InitWindow(defaultScreenWidth, defaultScreenHeight, defaultTitle.data());
    SetTargetFPS(defaultTargetFPS);

    while (!WindowShouldClose())
    {
        {
            app.Process();
        }

        {
            BeginDrawing();
            app.Draw();
            EndDrawing();
        }
    }

    CloseWindow();
}