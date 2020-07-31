#include "Codec.hpp"

#include <stdexcept>
#include <string>

Codec::Codec(const AVCodecID& codec_id, std::function<AVCodec*(AVCodecID)> find_codec)
    : _codec(find_codec(codec_id)), 
	_codec_context(avcodec_alloc_context3(_codec)),
	_packet(av_packet_alloc()),
	_frame(av_frame_alloc())
{
    checkPointers();
}

Codec::Codec(const char* codec_name, std::function<AVCodec*(const char *)> find_codec)
	: _codec(find_codec(codec_name)),
	_codec_context(avcodec_alloc_context3(_codec)),
	_packet(av_packet_alloc()),
	_frame(av_frame_alloc())
{
	checkPointers();
}

void Codec::checkPointers()
{
	if(_codec == NULL)
	{
		throw std::runtime_error("Unable to create codec");
	}

	if(_codec_context == NULL)
	{
		throw std::runtime_error("Unable to allocate video codec context");
	}

	if(_packet == NULL)
	{
		throw std::runtime_error("Unable to allocate bitstream packet");
	}

	if(_frame == NULL)
	{
		throw std::runtime_error("Unable to allocate video frame");
	}
}

AVCodec* Codec::getCodec()
{
    return _codec;
}

AVCodecContext* Codec::getContext()
{
    return _codec_context;
}


Codec::~Codec()
{
    avcodec_free_context(&_codec_context);
	av_frame_free(&_frame);
	av_packet_free(&_packet);
}