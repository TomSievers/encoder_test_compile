#ifndef STREAM_OPTIONS_HPP
#define STREAM_OPTIONS_HPP

#include "VideoEncoder.hpp"

struct VideoOptions
{
    AVCodecID codec;
    Resolution resolution;
    AVPixelFormat format;
    uint64_t bit_rate;
    uint16_t frame_rate;
};

struct AudioOptions
{
    AVCodecID codec;
    int sample_rate;
    AVSampleFormat sample_fmt;
    uint8_t n_channels;
    uint64_t bit_rate;
};

struct StreamOptions
{
    VideoOptions video_opt;
    AudioOptions audio_opt;
};

#endif //STREAM_OPTIONS_HPP