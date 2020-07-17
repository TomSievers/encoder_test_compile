#ifndef CODEC_HPP
#define CODEC_HPP

extern "C"
{
	#include <libavcodec/avcodec.h>
}

class Codec
{
public:

    Codec(const AVCodecID& codec_id);

    Codec(const char* codec_name);

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