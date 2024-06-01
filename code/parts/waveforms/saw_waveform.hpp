#ifndef SAW_WAVEFORM_HPP
#define SAW_WAVEFORM_HPP

#include "waveform.hpp"

template <class TSampleType, SizeType TSampleCount>
class SawWaveform final : public Waveform<TSampleType, TSampleCount>
{
public:
    using WaveformParentType = Waveform<TSampleType, TSampleCount>;

protected:
private:
public:
    SawWaveform() = default;
    SawWaveform(FloatType pFrequency, FloatType pAmplitude, FloatType pOffset = 0.0) : WaveformParentType(pFrequency, pAmplitude, pOffset) {}
    ~SawWaveform() override = default;

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
            const auto phase = i * WaveformParentType::frequency.Interpolate(indexDiminishedProportion) + WaveformParentType::offset;
            WaveformParentType::samples[i] = static_cast<typename WaveformParentType::SampleType>(
                std::clamp(
                    amplitude * std::fmod(phase, 1),
                    FloatType(std::numeric_limits<typename WaveformParentType::SampleType>::min()),
                    FloatType(std::numeric_limits<typename WaveformParentType::SampleType>::max())));
        }
        WaveformParentType::offset = newOffset;
    }
};

#endif // SAW_WAVEFORM_HPP