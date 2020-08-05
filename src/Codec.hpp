#ifndef CODEC_HPP
#define CODEC_HPP

#include "CodecType.hpp"

extern "C"
{
	#include <libavcodec/avcodec.h>
}

/**
 * @brief DO NOT USE, Is the base class for every encoder
 * 
 */
class Codec
{
public:
    AVCodec* getCodec();

    AVCodecContext* getContext();

protected:

    Codec(const AVCodecID& codec_id, std::function<AVCodec*(AVCodecID)> find_codec);

    Codec(const char* codec_name, std::function<AVCodec*(const char *)> find_codec);

    virtual ~Codec();

    AVCodec* _codec;
	AVCodecContext* _codec_context;
	
	AVPacket* _packet;
	AVFrame* _frame;

private:
    void checkPointers();
};


#endif