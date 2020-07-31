#ifndef AUDIOENCODER_HPP
#define AUDIOENCODER_HPP

#include "Codec.hpp"

class AudioEncoder : public Codec
{
private:
    /* data */
public:
    AudioEncoder(/* args */);
    virtual ~AudioEncoder();
};

AudioEncoder::AudioEncoder(/* args */)
    : Codec(AV_CODEC_ID_ALAC, avcodec_find_encoder)
{
}

AudioEncoder::~AudioEncoder()
{
}


#endif //AUDIOENCODER_HPP