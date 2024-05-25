#include <raylib.h>
#include <string>

#include "components/component.hpp"
#include "app.hpp"

static_assert(IsComponent<decltype(app)>, "App must be a component.");

constexpr const int DEFAULT_SCREEN_WIDTH = 800;
constexpr const int DEFAULT_SCREEN_HEIGHT = 450;
constexpr const int DEFAULT_TARGET_FPS = 60;
constexpr const int DEFAULT_AUDIO_STREAM_BUFFER_SIZE = 4096;
constexpr const std::string_view DEFAULT_TITLE = "Hello world";

int main()
{
    decltype(app)::ProcessingType processing;

    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, DEFAULT_TITLE.data());
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(DEFAULT_AUDIO_STREAM_BUFFER_SIZE);
    SetTargetFPS(DEFAULT_TARGET_FPS);

    while (!WindowShouldClose())
    {
        {
            processing = app.Process(processing);
        }

        {
            BeginDrawing();
            app.Draw();
            EndDrawing();
        }
    }

    CloseWindow();
}