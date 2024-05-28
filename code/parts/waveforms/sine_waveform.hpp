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
    using WaveformParentType = Waveform<TSampleType, TSampleCount>;

protected:
private:
public:
    SineWaveform() = default;
    SineWaveform(FloatType pFrequency, FloatType pAmplitude, FloatType pOffset = 0.0f) : WaveformParentType(pFrequency, pAmplitude, pOffset) {}
    ~SineWaveform() override = default;

    auto UpdateSamples() -> void override
    {
        const auto newOffset = std::fmod(WaveformParentType::samples.size() * WaveformParentType::frequency.ViewCurrent() + WaveformParentType::offset, 1);

        if (
            !WaveformParentType::amplitude.IsDifferenceSignificant() &&
            !WaveformParentType::frequency.IsDifferenceSignificant() &&
            WaveformParentType::offset == newOffset)
            return;

        for (SizeType i = 0; i < WaveformParentType::samples.size(); i++)
        {
            const auto indexProportion = (double(i) / WaveformParentType::samples.size());
            WaveformParentType::samples[i] = static_cast<typename WaveformParentType::SampleType>(
                std::clamp(
                    (WaveformParentType::amplitude.Interpolate(indexProportion)) *
                        std::sin(2.0f * std::numbers::pi * (i * WaveformParentType::frequency.Interpolate(indexProportion) + WaveformParentType::offset)),
                    double(std::numeric_limits<typename WaveformParentType::SampleType>::min()),
                    double(std::numeric_limits<typename WaveformParentType::SampleType>::max())));
        }
        WaveformParentType::offset = newOffset;
    }
};

#endif // SINE_WAVEFORM_HPP