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

    static constexpr const auto ENGRAVING = "Gracile";

    static constexpr const auto AVERAGE_AMPLITUDE = 5000.0;

    static constexpr const auto C4 = 261.6;
    static constexpr const auto D4 = 293.7;
    static constexpr const auto E4 = 329.6;
    static constexpr const auto F4 = 349.6;
    static constexpr const auto G4 = 392.0;
    static constexpr const auto A4 = 440.0;
    static constexpr const auto B4 = 493.9;
    static constexpr const auto C5 = 523.3;
    static constexpr const auto D5 = 587.3;
    static constexpr const auto E5 = 659.3;
    static constexpr const auto F5 = 698.5;
    static constexpr const auto G5 = 784.0;
    static constexpr const auto A5 = 880.0;
    static constexpr const auto B5 = 987.8;

    static constexpr const auto DARK_COLOR = (Color){13, 27, 42, 255};
    static constexpr const auto DARK_GREY_COLOR = (Color){46, 64, 89, 255};
    static constexpr const auto GREY_COLOR = (Color){119, 141, 169, 255};
    static constexpr const auto LIGHT_COLOR = (Color){224, 225, 221, 255};

private:
    KeyboardType keyboard;

public:
    FloatType loudness;

    App()
        : keyboard(), loudness(50.0)
    {
        // 4th Octave.
        keyboard.insert({KEY_Z, SynthType::CreateSynthFromWaveform<SineWaveform>(C4, 0.0)});
        keyboard.insert({KEY_X, SynthType::CreateSynthFromWaveform<SineWaveform>(D4, 0.0)});
        keyboard.insert({KEY_C, SynthType::CreateSynthFromWaveform<SineWaveform>(E4, 0.0)});
        keyboard.insert({KEY_V, SynthType::CreateSynthFromWaveform<SineWaveform>(F4, 0.0)});
        keyboard.insert({KEY_B, SynthType::CreateSynthFromWaveform<SineWaveform>(G4, 0.0)});
        keyboard.insert({KEY_N, SynthType::CreateSynthFromWaveform<SineWaveform>(A4, 0.0)});
        keyboard.insert({KEY_M, SynthType::CreateSynthFromWaveform<SineWaveform>(B4, 0.0)});

        // 5th Octave.
        keyboard.insert({KEY_Q, SynthType::CreateSynthFromWaveform<SineWaveform>(C5, 0.0)});
        keyboard.insert({KEY_W, SynthType::CreateSynthFromWaveform<SineWaveform>(D5, 0.0)});
        keyboard.insert({KEY_E, SynthType::CreateSynthFromWaveform<SineWaveform>(E5, 0.0)});
        keyboard.insert({KEY_R, SynthType::CreateSynthFromWaveform<SineWaveform>(F5, 0.0)});
        keyboard.insert({KEY_T, SynthType::CreateSynthFromWaveform<SineWaveform>(G5, 0.0)});
        keyboard.insert({KEY_Y, SynthType::CreateSynthFromWaveform<SineWaveform>(A5, 0.0)});
        keyboard.insert({KEY_U, SynthType::CreateSynthFromWaveform<SineWaveform>(B5, 0.0)});
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
            const auto amplitude = IsKeyDown(key) ? mouseSpeed * loudness : 0.0;
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
        const auto longestScreenEdgeLength = std::max(GetScreenHeight(), GetScreenHeight());
        const auto centerCircleSize = shortestScreenEdgeLength * 0.05;
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARK_COLOR);
        DrawCircle(screenCenterX, screenCenterY, centerCircleSize, GREY_COLOR);

        {
            auto keyIndex = 0.0;
            const auto keyCount = keyboard.size();
            const auto maxAmplitudeDisplacement = (shortestScreenEdgeLength / 2) * 0.8;
            const auto minAmplitudeDisplacement = maxAmplitudeDisplacement * 0.85;
            for (auto &[key, synth] : keyboard)
            {
                const auto keyIndexProportion = keyIndex / keyCount;

                const auto &samples = synth.waveform->ViewSamples();
                const auto sampleCount = samples.size();
                for (SizeType sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++)
                {
                    const auto sampleIndexProportion = FloatType(sampleIndex) / sampleCount;
                    const auto frequencyBaseRadius = std::lerp(centerCircleSize * 1.5, longestScreenEdgeLength * 0.9, keyIndexProportion);
                    const auto frequencyRadius = frequencyBaseRadius + FloatType(samples[sampleIndex]) * 8.0 / AVERAGE_AMPLITUDE;
                    DrawPixel(
                        screenCenterX + frequencyRadius * std::sin(2 * PI * sampleIndexProportion),
                        screenCenterY + frequencyRadius * std::cos(2 * PI * sampleIndexProportion),
                        DARK_GREY_COLOR);
                }

                const auto amplitudeDisplacement = std::lerp(minAmplitudeDisplacement, maxAmplitudeDisplacement, std::clamp(synth.waveform->amplitude.ViewCurrent() / AVERAGE_AMPLITUDE, 0.0, 1.0));
                DrawCircle(
                    screenCenterX + amplitudeDisplacement * std::sin(2 * PI * keyIndexProportion),
                    screenCenterY + amplitudeDisplacement * std::cos(2 * PI * keyIndexProportion),
                    shortestScreenEdgeLength * 0.01,
                    LIGHT_COLOR);
                keyIndex++;
            }
        }

        DrawText(ENGRAVING, 5, 5, 10, DARK_GREY_COLOR);
    }

    auto Finish() -> void override
    {
        for (auto &[key, synth] : keyboard)
            synth.Finish();
    }
};

#endif // APP_HPP