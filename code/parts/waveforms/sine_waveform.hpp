#ifndef SINE_WAVEFORM_HPP
#define SINE_WAVEFORM_HPP

#include <algorithm>
#include <limits>
#include <cmath>
#include <numbers>

#include "waveform.hpp"

template <class TSampleType, SizeType TSampleCount>
class SineWaveform final : public Waveform<TSampleType, TSampleCount>
{
public:
    using WaveformType = Waveform<TSampleType, TSampleCount>;

protected:
private:
public:
    SineWaveform() = default;
    SineWaveform(FloatType pFrequency, FloatType pAmplitude) : WaveformType(pFrequency, pAmplitude) {}
    ~SineWaveform() override = default;

    auto UpdateSamples() -> void override
    {
        static double position = 0;
        for (SizeType i = 0; i < WaveformType::samples.size(); i++)
        {
            position += WaveformType::frequency;
            if (position > 1)
                position -= 1;
            WaveformType::samples[i] = static_cast<typename WaveformType::SampleType>(
                std::clamp(WaveformType::amplitude * std::sin(2 * std::numbers::pi * position), double(std::numeric_limits<typename WaveformType::SampleType>::min()), double(std::numeric_limits<typename WaveformType::SampleType>::max())));
        }
    }

    auto Process() -> void override
    {
        WaveformType::Process();
        UpdateSamples();
    }
};

#endif // SINE_WAVEFORM_HPP