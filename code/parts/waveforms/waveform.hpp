#ifndef WAVEFORM_HPP
#define WAVEFORM_HPP

#include <array>

#include "definition.hpp"
#include "parts/part.hpp"
#include "parts/interpolated.hpp"

template <class TSampleType, SizeType TSampleCount>
class Waveform : public Part<>
{
public:
    using SizeType = ::SizeType;
    using FloatType = ::FloatType;
    using SampleType = TSampleType;
    using BufferType = std::array<SampleType, TSampleCount>;

    static constexpr const SizeType SAMPLE_BIT_SIZE = sizeof(SampleType) * 8;
    static constexpr const SizeType SAMPLE_COUNT = TSampleCount;
    static constexpr const FloatType DEFAULT_FREQUENCY_MAX_DIFFERENCE = 0.0005f;
    static constexpr const FloatType DEFAULT_AMPLITUDE_MAX_DIFFERENCE = 0.0005f;

protected:
    BufferType samples;

private:
public:
    Interpolated<FloatType> frequency;
    Interpolated<FloatType> amplitude;
    FloatType offset;

    Waveform(
        FloatType pFrequencyValue,
        FloatType pAmplitudeValue,
        FloatType pOffset = 0.0f)
        : samples(),
          frequency(pFrequencyValue, DEFAULT_FREQUENCY_MAX_DIFFERENCE, 0.08f, 0.15f),
          amplitude(pAmplitudeValue, DEFAULT_AMPLITUDE_MAX_DIFFERENCE, 0.08f, 0.15f),
          offset(pOffset) {}
    Waveform() : Waveform(0.0f, 0.0f, 0.0f) {}
    virtual ~Waveform() = 0;

    virtual auto UpdateSamples() -> void = 0;

    virtual auto ViewSamples() const -> const BufferType & final
    {
        return samples;
    }

    auto Start() -> void override
    {
        frequency.Start();
        amplitude.Start();
    }

    auto Process() -> void override
    {
        frequency.Process();
        amplitude.Process();
    }

    auto Draw() -> void override
    {
        frequency.Draw();
        amplitude.Draw();
    }

    auto Finish() -> void override
    {
        frequency.Finish();
        amplitude.Finish();
    }
};

template <class Type, SizeType SampleCount>
Waveform<Type, SampleCount>::~Waveform() {}

#endif // WAVEFORM_HPP