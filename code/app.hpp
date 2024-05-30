#ifndef APP_HPP
#define APP_HPP

#include <memory>
#include <map>
#include <vector>
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

    template <class TSampleType, SizeType TSampleCount>
    using KeyboardWaveformType = SineWaveform<TSampleType, TSampleCount>;
    using KeyboardType = std::map<KeyboardKey, SynthType>;

    template <class TSampleType, SizeType TSampleCount>
    using ResonanceChamberWaveformType = SineWaveform<TSampleType, TSampleCount>;
    using ResonanceChamberType = std::vector<SynthType>;
    using ResonanceCacheType = std::map<FloatType, FloatType>;

    static constexpr const auto ENGRAVING = "Gracile";

    static constexpr const auto AVERAGE_AMPLITUDE = 5000.0;
    static constexpr const auto RESONANCE_CHAMBER_AMPLITUDE_FACTOR = 0.15;
    static constexpr const auto RESONANCE_NEW_PEAK_AMPLITUDE_FACTOR = 0.5;
    static constexpr const auto RESONANCE_ADJECENT_AMPLUTUDE_FACTOR = 0.8;
    static constexpr const auto RESONANCE_MAX_ACCEPTED_INTERVAL_RATIO_DIFFERENCE = 0.01;

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
    ResonanceChamberType chambers;
    ResonanceCacheType resonances;

    static auto computeResonance(FloatType pIntervalRatio) -> FloatType
    {
        const auto targetIntervalRatio = std::abs(pIntervalRatio);
        auto intervalRatioSearchDisplacement = 1.0;
        auto intervalRatio = 1.0;
        auto amplitudeFactor = 1.0;

        while (true)
        {
            const auto intervalRatioDifference = targetIntervalRatio - intervalRatio;
            const auto absoluteIntervalRatioDifference = std::abs(intervalRatioDifference);
            if (absoluteIntervalRatioDifference < RESONANCE_MAX_ACCEPTED_INTERVAL_RATIO_DIFFERENCE)
            {
                amplitudeFactor = std::lerp(amplitudeFactor, 0.0, absoluteIntervalRatioDifference / RESONANCE_MAX_ACCEPTED_INTERVAL_RATIO_DIFFERENCE);
                break;
            }

            const auto alignedWithSearchDirection = (intervalRatioSearchDisplacement * intervalRatioDifference) >= 0;
            if (alignedWithSearchDirection)
            {
                amplitudeFactor *= RESONANCE_ADJECENT_AMPLUTUDE_FACTOR;
                intervalRatio += intervalRatioSearchDisplacement;
            }
            else
            {
                amplitudeFactor *= RESONANCE_NEW_PEAK_AMPLITUDE_FACTOR;
                intervalRatioSearchDisplacement *= 0.5;
                intervalRatio += -intervalRatioSearchDisplacement;
                if (intervalRatio > targetIntervalRatio)
                    intervalRatioSearchDisplacement = std::abs(intervalRatioSearchDisplacement);
                else
                    intervalRatioSearchDisplacement = -std::abs(intervalRatioSearchDisplacement);
            }
        }
        return amplitudeFactor;
    }

public:
    FloatType loudness;

    App()
        : keyboard(), chambers(), resonances(), loudness(1600.0)
    {
        // Keyboard.
        // 4th Octave.
        keyboard.insert({KEY_Z, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(C4, 0.0)});
        keyboard.insert({KEY_X, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(D4, 0.0)});
        keyboard.insert({KEY_C, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(E4, 0.0)});
        keyboard.insert({KEY_V, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(F4, 0.0)});
        keyboard.insert({KEY_B, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(G4, 0.0)});
        keyboard.insert({KEY_N, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(A4, 0.0)});
        keyboard.insert({KEY_M, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(B4, 0.0)});

        // 5th Octave.
        keyboard.insert({KEY_Q, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(C5, 0.0)});
        keyboard.insert({KEY_W, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(D5, 0.0)});
        keyboard.insert({KEY_E, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(E5, 0.0)});
        keyboard.insert({KEY_R, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(F5, 0.0)});
        keyboard.insert({KEY_T, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(G5, 0.0)});
        keyboard.insert({KEY_Y, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(A5, 0.0)});
        keyboard.insert({KEY_U, SynthType::CreateSynthFromWaveform<KeyboardWaveformType>(B5, 0.0)});

        // Chamber.
        // chambers.push_back(SynthType::CreateSynthFromWaveform<ResonanceChamberWaveformType>(C4, 0.0));
        chambers.push_back(SynthType::CreateSynthFromWaveform<ResonanceChamberWaveformType>(D4, 0.0));
        chambers.push_back(SynthType::CreateSynthFromWaveform<ResonanceChamberWaveformType>(E4, 0.0));
        // chambers.push_back(SynthType::CreateSynthFromWaveform<ResonanceChamberWaveformType>(F4, 0.0));
        chambers.push_back(SynthType::CreateSynthFromWaveform<ResonanceChamberWaveformType>(G4, 0.0));
        chambers.push_back(SynthType::CreateSynthFromWaveform<ResonanceChamberWaveformType>(A4, 0.0));
        // chamber.push_back(SynthType::CreateSynthFromWaveform<ResonanceChamberWaveformType>(B4, 0.0));
    }
    ~App() override = default;

    auto Start() -> void override
    {
        for (auto &[key, synth] : keyboard)
            synth.Start();
        for (auto &chamber : chambers)
            chamber.Start();
    }

    auto Process() -> void override
    {
        const auto mouseSpeed = Vector2Length(GetMouseDelta());

        for (auto &[key, synth] : keyboard)
        {
            const auto amplitude = loudness * std::log(
                                                  (IsKeyDown(key) ? mouseSpeed : 0.0) + 1);
            synth.waveform->amplitude.target = amplitude;
            synth.Process();
        }

        for (auto &chamber : chambers)
        {
            auto &chamberAmplitude = chamber.waveform->amplitude.target;
            chamberAmplitude = 0.0;

            for (auto &[key, synth] : keyboard)
            {
                const auto &synthFrequency = synth.waveform->frequency.ViewCurrent();
                const auto &chamberFrequency = chamber.waveform->frequency.ViewCurrent();
                const auto &synthAmplitude = synth.waveform->amplitude.ViewCurrent();
                const auto intervalRatio = synthFrequency > chamberFrequency ? synthFrequency / chamberFrequency : chamberFrequency / synthFrequency;
                if (!resonances.contains(intervalRatio))
                    resonances.insert({intervalRatio, computeResonance(intervalRatio)});
                const auto amplitudeFactor = resonances[intervalRatio];
                chamberAmplitude += synthAmplitude * amplitudeFactor;
            }

            chamber.Process();
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
            auto chamberIndex = SizeType(0);
            const auto chamberCount = chambers.size();
            for (const auto &chamber : chambers)
            {
                const auto chamberIndexProportion = FloatType(chamberIndex) / (chamberCount - 1);
                const auto baseY = std::lerp(GetScreenHeight() * 0.05, GetScreenHeight() * 0.95, chamberIndexProportion);

                const auto &samples = chamber.waveform->ViewSamples();
                const auto sampleCount = samples.size();
                for (SizeType sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++)
                {
                    const auto sampleIndexProportion = FloatType(sampleIndex) / sampleCount;
                    DrawPixel(
                        GetScreenWidth() * sampleIndexProportion,
                        baseY + FloatType(samples[sampleIndex]) * 16.0 / AVERAGE_AMPLITUDE,
                        DARK_GREY_COLOR);
                }

                chamberIndex++;
            }
        }

        {
            auto keyIndex = SizeType(0);
            const auto keyCount = keyboard.size();
            const auto maxAmplitudeDisplacement = (shortestScreenEdgeLength / 2) * 0.8;
            const auto minAmplitudeDisplacement = maxAmplitudeDisplacement * 0.9;
            for (const auto &[key, synth] : keyboard)
            {
                const auto keyIndexDiminishedProportion = FloatType(keyIndex) / (keyCount);

                const auto &samples = synth.waveform->ViewSamples();
                const auto sampleCount = samples.size();
                for (SizeType sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++)
                {
                    const auto sampleIndexProportion = FloatType(sampleIndex) / sampleCount;
                    const auto frequencyBaseRadius = std::lerp(centerCircleSize * 1.5, longestScreenEdgeLength * 0.8, keyIndexDiminishedProportion);
                    const auto frequencyRadius = frequencyBaseRadius + FloatType(samples[sampleIndex]) * 8.0 / AVERAGE_AMPLITUDE;
                    DrawPixel(
                        screenCenterX + frequencyRadius * std::sin(2 * PI * sampleIndexProportion),
                        screenCenterY + frequencyRadius * std::cos(2 * PI * sampleIndexProportion),
                        DARK_GREY_COLOR);
                }

                const auto amplitudeDisplacement = std::lerp(minAmplitudeDisplacement, maxAmplitudeDisplacement, std::clamp(synth.waveform->amplitude.ViewCurrent() / AVERAGE_AMPLITUDE, 0.0, 1.0));
                DrawCircle(
                    screenCenterX + amplitudeDisplacement * std::sin(2 * PI * keyIndexDiminishedProportion),
                    screenCenterY + amplitudeDisplacement * std::cos(2 * PI * keyIndexDiminishedProportion),
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
        for (auto &chamber : chambers)
            chamber.Finish();
    }
};

#endif // APP_HPP