#ifndef AUDIOENCODER_HPP
#define AUDIOENCODER_HPP

#include "Codec.hpp"

class AudioEncoder : public Codec
{
public:
    AudioEncoder(const AVCodecID& codec, int sample_rate, const AVSampleFormat& sample_format, uint8_t n_channels, uint32_t bit_rate);
    virtual ~AudioEncoder();

private:
    bool checkSampleFormat(const AVSampleFormat& sample_fmt);
    bool checkSampleRate(int sample_rate);
    uint64_t selectChannelLayout(uint8_t n_channels);
    /* data */
};

#endif //AUDIOENCODER_HPP