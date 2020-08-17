#include "VideoEncoder.hpp"

#include <stdexcept>

extern "C"
{
	#include <libavutil/opt.h>
}

#include <opencv2/core.hpp>

VideoEncoder::VideoEncoder(const std::string& codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate) 
	: Codec(codec_name.c_str(), avcodec_find_encoder_by_name)
{
	init(res, format, bit_rate, frame_rate);
}

VideoEncoder::VideoEncoder(const char* codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate)
	: Codec(codec_name, avcodec_find_encoder_by_name)
{
	init(res, format, bit_rate, frame_rate);
}

VideoEncoder::VideoEncoder(const AVCodecID& codec_id, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate)
	: Codec(codec_id, avcodec_find_encoder)
{
	init(res, format, bit_rate, frame_rate);
}

void VideoEncoder::init(const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate)
{
	_codec_context->pix_fmt = format;
	_codec_context->width = res.width;
	_codec_context->height = res.height;
	_codec_context->bit_rate = bit_rate;
	_codec_context->time_base = (AVRational){1, frame_rate};
	_codec_context->time_base = (AVRational){frame_rate, 1};
	_codec_context->gop_size = 10;
	_codec_context->max_b_frames = 1;
	_codec_context->thread_count = 4;


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
	_frame->pts = 0;

	ret = av_frame_get_buffer(_frame, 0);
	if(ret < 0)
	{
		char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
		throw std::runtime_error("Unable to allocate video frame data " + std::string(error));
	}
}

void VideoEncoder::copyToFrame(const cv::Mat& frame)
{
	if(frame.rows != _codec_context->height || frame.cols != _codec_context->width)
	{
		throw std::runtime_error(
			"Invalid frame expected size: " + std::to_string(_codec_context->width) + "x" + std::to_string(_codec_context->height) +
			" recieved size: " + std::to_string(frame.cols) + "x" + std::to_string(frame.rows));
	}

	if (av_frame_make_writable(_frame) < 0)
	{
		throw std::runtime_error("Unable to make frame writeable");
	}

	cv::split(frame, _channels);

	for(int channel = 0; channel < frame.channels(); ++channel)
	{
		_frame->data[channel] = _channels[channel].data;
	}
}

void VideoEncoder::encodeIntoStream(const cv::Mat& frame, std::ostream& output)
{
	encodeIntoPacket(frame, _packet);

	output.write(reinterpret_cast<char *>(_packet->data), _packet->size);

    av_packet_unref(_packet);
}

void VideoEncoder::encodeIntoPacket(const cv::Mat& frame, AVPacket* packet)
{
	copyToFrame(frame);

    if (avcodec_send_frame(_codec_context, _frame) < 0) 
	{
        throw std::runtime_error("Error sending a frame for encoding");
    }

	int ret = 0;

    while (ret >= 0) 
	{
        ret = avcodec_receive_packet(_codec_context, packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
		{
			packet->pts = _frame->pts;
			++_frame->pts;
			return;
		} else if (ret < 0) 
		{
			char error[AV_ERROR_MAX_STRING_SIZE];
			av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
            throw std::runtime_error("Error during encoding: " + std::string(error));
        }
    }
	++_frame->pts;
}

VideoEncoder::~VideoEncoder()
{
	avcodec_send_frame(_codec_context, NULL);
}