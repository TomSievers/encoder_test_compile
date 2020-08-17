#include "Muxer.hpp"

#include <iostream>
#include <functional>

#include "StreamIO.hpp"

Muxer::Muxer(const std::string& file_name, StreamOptions& options)
    : _io_context(NULL), _io_buf(NULL)
{
    initFile(file_name.c_str(), options);
}

Muxer::Muxer(const char* file_name, StreamOptions& options)
    : _io_context(NULL), _io_buf(NULL)
{
    initFile(file_name, options);
}

Muxer::Muxer(std::ostream& output_stream, const std::string& output_format, StreamOptions& options)
    : _io_buf(reinterpret_cast<uint8_t *>(av_malloc(sizeof(uint8_t)*_io_buf_size)))
{
    if(_io_buf == NULL)
    {
        throw(std::runtime_error("Out of memory, unable to allocate buffer of size: " + std::to_string(sizeof(uint8_t)*_io_buf_size)));
    }

    _io_context = avio_alloc_context(_io_buf, _io_buf_size, 1, &output_stream, NULL, &StreamIO::write, &StreamIO::seek);

    if(_io_context == NULL)
    {
        throw(std::runtime_error("Out of memory, unable to allocate AVIOContext"));
    }

    int ret = avformat_alloc_output_context2(&_format_context, NULL, output_format.c_str(), NULL);
    if(ret < 0)
    {
        char error[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
        throw std::runtime_error("Unable to allocate output context using output format " + output_format + ", error: " + std::string(error));
    }

    _format_context->pb = _io_context;

    _format = _format_context->oformat;

    options.audio_opt.codec = _format->audio_codec;
    options.video_opt.codec = _format->video_codec;

    _output = std::make_unique<OutputStream>(_format_context, options);
}

void Muxer::initFile(const char* file_name, StreamOptions& options)
{
    int ret = avformat_alloc_output_context2(&_format_context, NULL, NULL, file_name);
    if(ret < 0)
    {
        char error[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
        throw std::runtime_error("Unable to allocate output context: " + std::string(error));
    } 
    if(_format_context == NULL)
    {
        std::cout << "default to mpeg output type" << std::endl;
        avformat_alloc_output_context2(&_format_context, NULL, "mpeg", file_name);
    }

    _format = _format_context->oformat;   

    options.audio_opt.codec = _format->audio_codec;
    options.video_opt.codec = _format->video_codec;

    if (!(_format->flags & AVFMT_NOFILE)) 
    {
        ret = avio_open(&_format_context->pb, file_name, AVIO_FLAG_WRITE);
        if (ret < 0) 
        {
            char error[AV_ERROR_MAX_STRING_SIZE];
            av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
            throw std::runtime_error("Unable to open file " + std::string(file_name) + ": " + std::string(error));
        }
    }

    _output = std::make_unique<OutputStream>(_format_context, options);
}

void Muxer::writeHeader()
{
    int ret = avformat_write_header(_format_context, NULL);
    if (ret < 0) {
        char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
		throw std::runtime_error("Error occurred when writing header: " + std::string(error));
    }
}

void Muxer::write(const cv::Mat& image)
{
    AVPacket packet;

    av_init_packet(&packet);

    _output->getVideoEncoder()->encodeIntoPacket(image, &packet);

    _output->writeVideo(&packet);
}

void Muxer::write(const AudioFrame& sound)
{
    AVPacket packet;

    av_init_packet(&packet);

    _output->getAudioEncoder()->encodeIntoPacket(sound, &packet);

    _output->writeAudio(&packet);
}

Muxer::~Muxer()
{
    av_write_trailer(_format_context);
    if(_io_context != NULL)
    {
        avio_context_free(&_format_context->pb);
    } else if (!(_format->flags & AVFMT_NOFILE))
    {
        avio_closep(&_format_context->pb);
    }
    

    if(_io_buf != NULL)
    {
        av_free(_io_buf);
    }

    avformat_free_context(_format_context);
}