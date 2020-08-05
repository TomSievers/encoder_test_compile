#include "AudioEncoder.hpp"

#include <iostream>

extern "C"
{
    #include <libavutil/samplefmt.h>
    #include <libavcodec/avcodec.h>
}


AudioEncoder::AudioEncoder(const AVCodecID& codec, int sample_rate, const AVSampleFormat& sample_format, uint8_t n_channels, uint32_t bit_rate)
    : Codec(codec, avcodec_find_encoder)
{
    _codec_context->bit_rate = bit_rate;

    if(!checkSampleFormat(sample_format))
    {
        std::cout << "Unsupported sample format default to first supported format" << std::endl;
        _codec_context->sample_fmt = _codec->sample_fmts ? _codec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
    }

    _codec_context->sample_fmt = sample_format;

    _codec_context->sample_rate = 44100;

    if(checkSampleRate(sample_rate))
    {
        _codec_context->sample_rate = sample_rate;
    }

    _codec_context->channels = n_channels;
    _codec_context->channel_layout = selectChannelLayout(n_channels);
}

bool AudioEncoder::checkSampleFormat(const AVSampleFormat& sample_fmt)
{
    const enum AVSampleFormat *p = _codec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE) 
    {
        std::cout << *p << std::endl;
        if (*p == sample_fmt)
        {
            return true;
        }
        ++p;
    }
    return false;
}

bool AudioEncoder::checkSampleRate(int sample_rate)
{
    if(_codec->supported_samplerates == NULL)
    {
        return false;
    }

    const int* p = _codec->supported_samplerates;

    while(*p)
    {
        if(*p == sample_rate)
        {
            return true;
        }
        ++p;
    }
    return false;
}

uint64_t AudioEncoder::selectChannelLayout(uint8_t n_channels)
{
    if(!_codec->channel_layouts)
    {
        return AV_CH_LAYOUT_STEREO;
    }
        
    const uint64_t* p = _codec->channel_layouts;

    while(*p)
    {
        if(av_get_channel_layout_nb_channels(*p) == n_channels)
        {
            return *p;
        }
    }

    return AV_CH_LAYOUT_STEREO;
}

AudioEncoder::~AudioEncoder()
{

}