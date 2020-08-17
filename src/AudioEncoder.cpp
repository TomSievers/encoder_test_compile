#include "AudioEncoder.hpp"

#include <iostream>

extern "C"
{
    #include <libavutil/samplefmt.h>
    #include <libavcodec/avcodec.h>
}


AudioEncoder::AudioEncoder(const AVCodecID& codec, int sample_rate, const AVSampleFormat& sample_format, uint8_t n_channels, uint32_t bit_rate)
    : Codec(codec, avcodec_find_encoder)
{
    _codec_context->bit_rate = bit_rate;

    _codec_context->sample_fmt = sample_format;

    if(!checkSampleFormat(sample_format))
    {
        std::cout << "Unsupported sample format default to first supported format" << std::endl;
        _codec_context->sample_fmt = _codec->sample_fmts != NULL ? _codec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
    }

    _codec_context->sample_rate = 44100;

    if(checkSampleRate(sample_rate))
    {
        _codec_context->sample_rate = sample_rate;
    }

    _codec_context->channels = n_channels;
    _codec_context->channel_layout = selectChannelLayout(n_channels);

    int ret = avcodec_open2(_codec_context, _codec, NULL);
    
	if(ret < 0)
	{
		char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
		throw std::runtime_error("Unable to open codec " + std::string(error));
	}

    _frame->format = _codec_context->sample_fmt;
    _frame->channel_layout = _codec_context->channel_layout;
    _frame->sample_rate = _codec_context->sample_rate;

    if (_codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
    {
        _frame->nb_samples = 10000;
    } else 
    {
        _frame->nb_samples = _codec_context->frame_size;
    }
}

void AudioEncoder::encodeIntoStream(const AudioFrame& frame, std::ostream& output)
{
    encodeIntoPacket(frame, _packet);

	output.write(reinterpret_cast<char *>(_packet->data), _packet->size);

    av_packet_unref(_packet);

}

void AudioEncoder::encodeIntoPacket(const AudioFrame& frame, AVPacket* packet)
{
    if (av_frame_make_writable(_frame) < 0)
	{
		throw std::runtime_error("Unable to make frame writeable");
	}

    _last_frame = frame;

    for(int i = 0; i < AV_NUM_DATA_POINTERS; ++i)
    {
        _frame->data[i] = frame.data[i];
    }

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

bool AudioEncoder::checkSampleFormat(const AVSampleFormat& sample_fmt)
{
    const enum AVSampleFormat *p = _codec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE) 
    {
        if (*p == sample_fmt)
        {
            return true;
        }
        ++p;
    }
    return false;
}

bool AudioEncoder::checkSampleRate(int sample_rate)
{
    if(_codec->supported_samplerates == NULL)
    {
        return false;
    }

    const int* p = _codec->supported_samplerates;

    while(*p)
    {
        if(*p == sample_rate)
        {
            return true;
        }
        ++p;
    }
    return false;
}

uint64_t AudioEncoder::selectChannelLayout(uint8_t n_channels)
{
    if(!_codec->channel_layouts)
    {
        return AV_CH_LAYOUT_STEREO;
    }
        
    const uint64_t* p = _codec->channel_layouts;

    while(*p)
    {
        if(av_get_channel_layout_nb_channels(*p) == n_channels)
        {
            return *p;
        }
    }

    return AV_CH_LAYOUT_STEREO;
}



AudioEncoder::~AudioEncoder()
{

}