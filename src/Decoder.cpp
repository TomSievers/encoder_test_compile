#include "Decoder.hpp"

#include <stdexcept>

#include <iostream>

Decoder::Decoder(const std::string& codec_name)
    : Codec(codec_name.c_str(), avcodec_find_decoder_by_name),
    _parser(av_parser_init(_codec->id))
{
}

Decoder::Decoder(const char* codec_name)
    : Codec(codec_name, avcodec_find_decoder_by_name),
    _parser(av_parser_init(_codec->id))
{
}

Decoder::Decoder(const AVCodecID& codec_id)
    : Codec(codec_id, avcodec_find_decoder),
    _parser(av_parser_init(_codec->id))
{
}

void Decoder::init()
{

    int ret = avcodec_open2(_codec_context, _codec, NULL);
	if(ret < 0)
	{
		char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
		throw std::runtime_error("Unable to open codec " + std::string(error));
	}

    _parser = av_parser_init(_codec->id);

    
}

uint64_t Decoder::parse(std::string& bitstream)
{
    int ret = av_parser_parse2(_parser, _codec_context, &_packet->data, &_packet->size,
        reinterpret_cast<const uint8_t *>(bitstream.c_str()), bitstream.size(), AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

    if(ret < 0)
    {
        throw std::runtime_error("Unable to parse supplied bitstream");
    }

    bitstream.erase(0, ret);

    return ret;
}

void Decoder::decode(cv::Mat& out)
{
    int ret = avcodec_send_packet(_codec_context, _packet);
    if(ret < 0)
    {
        char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
        throw std::runtime_error("Error sending a packet for decoding: " + std::string(error));
    }    

    while (ret >= 0) 
    {
        ret = avcodec_receive_frame(_codec_context, _frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return;
        }
        else if (ret < 0) 
        {
            throw std::runtime_error("Error during decoding");
        }
        out.data = _frame->data[0];
    }
}

Decoder::~Decoder()
{
    avcodec_send_packet(_codec_context, NULL);
    av_parser_close(_parser);
}