#ifndef CODEC_HPP
#define CODEC_HPP

#include "CodecType.hpp"

extern "C"
{
	#include <libavcodec/avcodec.h>
}

class Codec
{
public:

    Codec(const AVCodecID& codec_id, std::function<AVCodec*(AVCodecID)> find_codec);

    Codec(const char* codec_name, std::function<AVCodec*(const char *)> find_codec);

    AVCodec* getCodec();

    AVCodecContext* getContext();

    virtual ~Codec();

protected:
    AVCodec* _codec;
	AVCodecContext* _codec_context;
	
	AVPacket* _packet;
	AVFrame* _frame;

private:
    void checkPointers();
};


#endif