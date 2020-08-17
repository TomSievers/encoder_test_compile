#ifndef AUDIOENCODER_HPP
#define AUDIOENCODER_HPP

#include "Audio.hpp"

#include "Codec.hpp"

class AudioEncoder : public Codec
{
public:
    AudioEncoder(const AVCodecID& codec, int sample_rate, const AVSampleFormat& sample_format, uint8_t n_channels, uint32_t bit_rate);
    virtual ~AudioEncoder();

    void encodeIntoStream(const AudioFrame& frame, std::ostream& output);

	void encodeIntoPacket(const AudioFrame& frame, AVPacket* packet);

private:
    bool checkSampleFormat(const AVSampleFormat& sample_fmt);
    bool checkSampleRate(int sample_rate);
    uint64_t selectChannelLayout(uint8_t n_channels);
    AudioFrame _last_frame;
};

#endif //AUDIOENCODER_HPP