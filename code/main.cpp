#include <raylib.h>
#include <string>

#include "definition.hpp"
#include "app.hpp"

constexpr const IntType DEFAULT_SCREEN_WIDTH = 800;
constexpr const IntType DEFAULT_SCREEN_HEIGHT = 450;
constexpr const IntType DEFAULT_TARGET_FPS = 30;
constexpr const CharType *DEFAULT_TITLE = "Gracile";

int main()
{
    auto app = App();

    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, DEFAULT_TITLE);
    InitAudioDevice();
    SetTargetFPS(DEFAULT_TARGET_FPS);
    app.Start();

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

    app.Finish();
    CloseAudioDevice();
    CloseWindow();
}