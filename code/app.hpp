#ifndef APP_HPP
#define APP_HPP

#include <memory>
#include <map>
#include <raylib.h>
#include <raymath.h>

#include "parts/part.hpp"
#include "parts/waveforms/sine_waveform.hpp"
#include "parts/synth.hpp"

template <class = void>
class App final : public Part<>
{
public:
    using SynthType = Synth<short>;
    using KeyboardType = std::map<KeyboardKey, SynthType>;

    static constexpr const auto C4 = 261.6f;
    static constexpr const auto D4 = 293.7f;
    static constexpr const auto E4 = 329.6f;
    static constexpr const auto F4 = 349.6f;
    static constexpr const auto G4 = 392.0f;
    static constexpr const auto A4 = 440.0f;
    static constexpr const auto B4 = 493.9f;
    static constexpr const auto C5 = 523.3f;
    static constexpr const auto D5 = 587.3f;
    static constexpr const auto E5 = 659.3f;
    static constexpr const auto F5 = 698.5f;
    static constexpr const auto G5 = 784.0f;
    static constexpr const auto A5 = 880.0f;
    static constexpr const auto B5 = 987.8f;

    static constexpr const auto DARK_COLOR = (Color){13, 27, 42, 255};
    static constexpr const auto GREY_COLOR = (Color){119, 141, 169, 255};
    static constexpr const auto LIGHT_COLOR = (Color){224, 225, 221, 255};

private:
    KeyboardType keyboard;

public:
    FloatType loudness;

    App()
        : keyboard(), loudness(50.0f)
    {
        // 4th Octave.
        keyboard.insert({KEY_Z, SynthType::CreateSynthFromWaveform<SineWaveform>(C4, 0.0f)});
        keyboard.insert({KEY_X, SynthType::CreateSynthFromWaveform<SineWaveform>(D4, 0.0f)});
        keyboard.insert({KEY_C, SynthType::CreateSynthFromWaveform<SineWaveform>(E4, 0.0f)});
        keyboard.insert({KEY_V, SynthType::CreateSynthFromWaveform<SineWaveform>(F4, 0.0f)});
        keyboard.insert({KEY_B, SynthType::CreateSynthFromWaveform<SineWaveform>(G4, 0.0f)});
        keyboard.insert({KEY_N, SynthType::CreateSynthFromWaveform<SineWaveform>(A4, 0.0f)});
        keyboard.insert({KEY_M, SynthType::CreateSynthFromWaveform<SineWaveform>(B4, 0.0f)});

        // 5th Octave.
        keyboard.insert({KEY_Q, SynthType::CreateSynthFromWaveform<SineWaveform>(C5, 0.0f)});
        keyboard.insert({KEY_W, SynthType::CreateSynthFromWaveform<SineWaveform>(D5, 0.0f)});
        keyboard.insert({KEY_E, SynthType::CreateSynthFromWaveform<SineWaveform>(E5, 0.0f)});
        keyboard.insert({KEY_R, SynthType::CreateSynthFromWaveform<SineWaveform>(F5, 0.0f)});
        keyboard.insert({KEY_T, SynthType::CreateSynthFromWaveform<SineWaveform>(G5, 0.0f)});
        keyboard.insert({KEY_Y, SynthType::CreateSynthFromWaveform<SineWaveform>(A5, 0.0f)});
        keyboard.insert({KEY_U, SynthType::CreateSynthFromWaveform<SineWaveform>(B5, 0.0f)});
    }
    ~App() override = default;

    auto Start() -> void override
    {
        for (auto &[key, synth] : keyboard)
            synth.Start();
    }

    auto Process() -> void override
    {
        const auto mouseSpeed = Vector2Length(GetMouseDelta());
        for (auto &[key, synth] : keyboard)
        {
            const auto amplitude = IsKeyDown(key) ? mouseSpeed * loudness : 0.0f;
            synth.waveform->amplitude.target = amplitude;
            synth.Process();
        }
    }

    auto Draw() -> void override
    {
        const auto screenCenterX = GetScreenWidth() / 2;
        const auto screenCenterY = GetScreenHeight() / 2;
        const auto mouseSpeed = Vector2Length(GetMouseDelta());
        const auto shortestScreenEdgeLength = std::min(GetScreenHeight(), GetScreenHeight());
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARK_COLOR);
        DrawCircle(screenCenterX, screenCenterY, shortestScreenEdgeLength * 0.05, GREY_COLOR);

        {
            auto keyIndex = 0.0f;
            const auto maxKeyIndex = keyboard.size();
            const auto maxDisplacement = (shortestScreenEdgeLength / 2) * 0.8;
            const auto minDisplacement = maxDisplacement * 0.8;
            for (auto &[key, synth] : keyboard)
            {
                const auto displacement = std::lerp(minDisplacement, maxDisplacement, std::clamp(synth.waveform->amplitude.ViewCurrent() / 5000.0f, 0.0, 1.0));
                const auto keyIndexProportion = keyIndex / maxKeyIndex;
                DrawCircle(
                    screenCenterX + displacement * std::sin(2 * PI * keyIndexProportion),
                    screenCenterY + displacement * std::cos(2 * PI * keyIndexProportion),
                    shortestScreenEdgeLength * 0.01f,
                    LIGHT_COLOR);
                keyIndex++;
            }
        }
    }

    auto Finish() -> void override
    {
        for (auto &[key, synth] : keyboard)
            synth.Finish();
    }
};

#endif // APP_HPP