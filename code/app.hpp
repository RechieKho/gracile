#ifndef APP_HPP
#define APP_HPP

#include <memory>

#include "parts/part.hpp"
#include "parts/waveforms/sine_waveform.hpp"
#include "parts/synth.hpp"

template <class = void>
class App final : public Part<>
{
public:
    using SynthType = Synth<short>;

private:
    SynthType synth;

public:
    App() : synth(SynthType::CreateSynthFromWaveform<SineWaveform>(440.0f / SynthType::SAMPLE_RATE, 16000)) {}
    ~App() override = default;

    auto Start() -> void override
    {
        synth.Start();
    }

    auto Process() -> void override
    {
        synth.Process();
    }

    auto Draw() -> void override
    {
        synth.Draw();
    }

    auto Finish() -> void override
    {
        synth.Finish();
    }
};

#endif // APP_HPP