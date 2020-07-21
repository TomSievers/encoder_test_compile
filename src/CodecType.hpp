#ifndef CODECTYPE_HPP
#define CODECTYPE_HPP

#include <functional>

extern "C"
{
    #include <libavcodec/avcodec.h>
}

struct CodecType
{
public: 
    std::function<AVCodec*(const char *)> find_by_name;
    std::function<AVCodec*(AVCodecID)> find_by_id;
};


#endif