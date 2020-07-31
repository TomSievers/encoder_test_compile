#ifndef STREAM_HPP
#define STREAM_HPP

#include "VideoEncoder.hpp"
#include "AudioEncoder.hpp"

#include <memory>

extern "C"
{
    #include "libavformat/avformat.h"
}

class Stream
{
public:
    Stream(/* args */);
    virtual ~Stream();
private:
    AVStream* _stream;

    std::shared_ptr<VideoEncoder> _video_encoder;
    std::shared_ptr<AudioEncoder> _audio_encoder;
};

#endif