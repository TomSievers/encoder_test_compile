#include "Encoder.hpp"

#include <exception>

extern "C"
{
	#include <libavutil/opt.h>
}


Encoder::Encoder(const std::string& codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate) 
	: _codec(avcodec_find_encoder_by_name(codec_name.c_str())), 
	_codec_context(avcodec_alloc_context3(_codec)),
	_packet(av_packet_alloc()),
	_frame(av_frame_alloc())
{
	init(codec_name, res, format, bit_rate, frame_rate);
}

Encoder::Encoder(const char* codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate)
	: _codec(avcodec_find_encoder_by_name(codec_name)), 
	_codec_context(avcodec_alloc_context3(_codec)),
	_packet(av_packet_alloc()),
	_frame(av_frame_alloc())
{
	init(codec_name, res, format, bit_rate, frame_rate);
}

Encoder::Encoder(const AVCodecID& codec_id, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate)
	: _codec(avcodec_find_encoder(codec_id)), 
	_codec_context(avcodec_alloc_context3(_codec)),
	_packet(av_packet_alloc()),
	_frame(av_frame_alloc())
{
	init(_codec->long_name, res, format, bit_rate, frame_rate);
}

void Encoder::init(const std::string& codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate)
{
	if(_codec == nullptr)
	{
		throw std::runtime_error("Unable to create codec " + codec_name);
	}

	if(_codec_context == nullptr)
	{
		throw std::runtime_error("Unable to allocate video codec context");
	}

	if(_packet == nullptr)
	{
		throw std::runtime_error("Unable to allocate bitstream packet");
	}

	if(_frame == nullptr)
	{
		throw std::runtime_error("Unable to allocate video frame");
	}

	_codec_context->width = res.width;
	_codec_context->height = res.height;
	_codec_context->bit_rate = bit_rate;
	_codec_context->time_base = (AVRational){1, frame_rate};
	_codec_context->time_base = (AVRational){frame_rate, 1};
	_codec_context->gop_size = 10;
	_codec_context->max_b_frames = 1;

	if (_codec->id == AV_CODEC_ID_H264)
	{
		av_opt_set(_codec_context->priv_data, "preset", "slow", 0);
	}
	int ret = avcodec_open2(_codec_context, _codec, NULL);
	if(ret < 0)
	{
		
		char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
		throw std::runtime_error("Unable to open codec " + std::string(error));
	}

	_frame->width = res.width;
	_frame->height = res.height;
	_frame->format = format;

	ret = av_frame_get_buffer(_frame, 0);
	if(ret < 0)
	{
		char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
		throw std::runtime_error("Unable to allocate video frame data " + std::string(error));
	}
}

Encoder::~Encoder()
{
	avcodec_free_context(&_codec_context);
	av_frame_free(&_frame);
	av_packet_free(&_packet);
}