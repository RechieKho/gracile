#ifndef WAVEFORM_HPP
#define WAVEFORM_HPP

#include <array>

#include "definition.hpp"
#include "parts/part.hpp"

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

protected:
    BufferType samples;

private:
public:
    FloatType frequency;
    FloatType amplitude;

    Waveform() = default;
    Waveform(FloatType pFrequency, FloatType pAmplitude) : frequency(pFrequency), amplitude(pAmplitude) {}
    virtual ~Waveform() = 0;

    virtual auto UpdateSamples() -> void = 0;

    virtual auto ViewSamples() const -> const BufferType & final
    {
        return samples;
    }

    auto Start() -> void override
    {
        UpdateSamples();
    }
};

template <class Type, SizeType SampleCount>
Waveform<Type, SampleCount>::~Waveform() {}

#endif // WAVEFORM_HPP