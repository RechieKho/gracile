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

private:
    KeyboardType keyboard;

public:
    FloatType loudness;

    App()
        : keyboard(), loudness(100.0f)
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

    auto Finish() -> void override
    {
        for (auto &[key, synth] : keyboard)
            synth.Finish();
    }
};

#endif // APP_HPP