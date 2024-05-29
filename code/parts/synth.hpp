#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <memory>
#include <concepts>
#include <raylib.h>

#include "definition.hpp"
#include "part.hpp"
#include "waveforms/waveform.hpp"

template <class TType>
concept IsSynth =
    std::derived_from<TType, Part<>> &&
    requires {
        typename TType::WaveformType;
        typename TType::WaveformLeashType;
        requires std::same_as<decltype(std::declval<TType>().waveform), typename TType::WaveformLeashType>;
    };

template <class TSampleType, SizeType TSampleCountPerUpdate = 4096, SizeType TSampleRate = 44100, SizeType TChannelCount = 1>
class Synth final : public Part<>
{
public:
    using WaveformType = Waveform<TSampleType, TSampleCountPerUpdate>;
    using WaveformLeashType = std::unique_ptr<WaveformType>;

    static constexpr const SizeType SAMPLE_COUNT_PER_UPDATE = TSampleCountPerUpdate;
    static constexpr const SizeType SAMPLE_RATE = TSampleRate;
    static constexpr const SizeType CHANNEL_COUNT = TChannelCount;

private:
    AudioStream stream;

public:
    WaveformLeashType waveform;

    template <template <class, SizeType> class TWaveformTemplateType>
        requires std::derived_from<TWaveformTemplateType<TSampleType, TSampleCountPerUpdate>, WaveformType>
    static auto CreateSynthFromWaveform(FloatType pFrequency, FloatType pAmplitude) -> Synth
    {
        return Synth(WaveformLeashType(new TWaveformTemplateType<TSampleType, TSampleCountPerUpdate>(pFrequency / SAMPLE_RATE, pAmplitude)));
    }

    Synth(WaveformLeashType pWaveform) : stream(), waveform(std::move(pWaveform)) {}

    auto Start() -> void override
    {
        waveform->Start();

        SetAudioStreamBufferSizeDefault(SAMPLE_COUNT_PER_UPDATE);
        stream = LoadAudioStream(SAMPLE_RATE, WaveformType::SAMPLE_BIT_SIZE, CHANNEL_COUNT);
        PlayAudioStream(stream);
    }

    auto Process() -> void override
    {
        waveform->Process();

        if (!IsAudioStreamProcessed(stream))
            return;
        waveform->UpdateSamples();
        UpdateAudioStream(stream, waveform->ViewSamples().data(), waveform->ViewSamples().size());
    }

    auto Draw() -> void override
    {
        waveform->Draw();
    }

    auto Finish() -> void override
    {
        waveform->Finish();

        UnloadAudioStream(stream);
    }
};

#endif // SYNTH_HPP