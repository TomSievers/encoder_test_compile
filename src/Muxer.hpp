#ifndef MUXER_HPP
#define MUXER_HPP

#include "Encoder.hpp"

#include <memory>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
}

class Muxer
{

public:
    Muxer(const std::string& file_name);
    Muxer(const char* file_name);
    virtual ~Muxer();

private:
    void init(const char* file_name);

    AVFormatContext* _format_context;

    AVOutputFormat* _format;

    std::shared_ptr<Encoder> _video_encoder;
    std::shared_ptr<Encoder> _audio_encoder;
};


#endif