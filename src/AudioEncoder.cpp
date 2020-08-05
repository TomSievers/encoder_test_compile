#include "AudioEncoder.hpp"

AudioEncoder::AudioEncoder(/* args */)
    : Codec(AV_CODEC_ID_ALAC, avcodec_find_encoder)
{
}

AudioEncoder::~AudioEncoder()
{
}