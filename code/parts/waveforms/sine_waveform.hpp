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
    SineWaveform(FloatType pFrequency, FloatType pAmplitude, FloatType pOffset = 0.0) : WaveformParentType(pFrequency, pAmplitude, pOffset) {}
    ~SineWaveform() override = default;

    auto UpdateSamples() -> void override
    {
        const auto newOffset = std::fmod(WaveformParentType::samples.size() * WaveformParentType::frequency.Interpolate() + WaveformParentType::offset, 1);

        if (
            !WaveformParentType::amplitude.IsDifferenceSignificant() &&
            !WaveformParentType::frequency.IsDifferenceSignificant() &&
            WaveformParentType::offset == newOffset)
            return;

        for (SizeType i = 0; i < WaveformParentType::samples.size(); i++)
        {
            const auto indexDiminishedProportion = (FloatType(i) / WaveformParentType::samples.size());
            const auto amplitude = WaveformParentType::amplitude.Interpolate(indexDiminishedProportion);
            const auto phase = 2.0 * std::numbers::pi * (i * WaveformParentType::frequency.Interpolate(indexDiminishedProportion) + WaveformParentType::offset);
            WaveformParentType::samples[i] = static_cast<typename WaveformParentType::SampleType>(
                std::clamp(
                    amplitude * std::sin(phase),
                    FloatType(std::numeric_limits<typename WaveformParentType::SampleType>::min()),
                    FloatType(std::numeric_limits<typename WaveformParentType::SampleType>::max())));
        }
        WaveformParentType::offset = newOffset;
    }
};

#endif // SINE_WAVEFORM_HPP