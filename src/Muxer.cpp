#include "Muxer.hpp"

#include <iostream>
#include <functional>

Muxer::Muxer(const std::string& file_name)
{
    initFile(file_name.c_str());
}

Muxer::Muxer(const char* file_name)
{
    initFile(file_name);
}

Muxer::Muxer(std::ostream& output_stream, const std::string& output_format)
    : _io_buf(reinterpret_cast<uint8_t *>(av_malloc(sizeof(uint8_t)*_io_buf_size)))
{
    if(_io_buf == NULL)
    {
        throw(std::runtime_error("Out of memory, unable to allocate buffer of size: " + std::to_string(sizeof(uint8_t)*_io_buf_size)));
    }

    _io_context = avio_alloc_context(_io_buf, _io_buf_size, 1, &output_stream, NULL, &Muxer::write, NULL);

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

    std::cout << _format->audio_codec << std::endl;
    std::cout << "---\n" << _format->video_codec << std::endl;

    StreamOptions options;

    options.audio_opt.codec = _format->audio_codec;
    options.audio_opt.sample_rate = 44100;
    options.audio_opt.sample_fmt = AV_SAMPLE_FMT_FLT;
    options.audio_opt.n_channels = 2;
    options.audio_opt.bit_rate = 640000;

    Resolution res = {.width=1920, .height=1080};

    options.video_opt.codec = _format->video_codec;
    options.video_opt.resolution = res;
    options.video_opt.format = AV_PIX_FMT_YUVJ420P;
    options.video_opt.bit_rate = 4000000;
    options.video_opt.frame_rate = 30;

    _output = std::make_unique<OutputStream>(_format_context, options, output_stream);
}

void Muxer::initFile(const char* file_name)
{
    int ret = avformat_alloc_output_context2(&_format_context, NULL, NULL, file_name);
    if(ret < 0)
    {
        char error[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
        throw std::runtime_error("Unable to allocate output context: " + std::string(error));
    } else if(_format_context == NULL)
    {
        std::cout << "default to mpeg output type" << std::endl;
        avformat_alloc_output_context2(&_format_context, NULL, "mpeg", file_name);
    }

    _format = _format_context->oformat;

    
}

void Muxer::writeHeader()
{
    int ret = avformat_write_header(_format_context, NULL);
    if (ret < 0) {
        char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
		throw std::runtime_error("Error occurred when opening output file:  " + std::string(error));
    }
}

int Muxer::write(void* opaque, uint8_t* buf, int buf_size)
{
    if(opaque == NULL)
    {
        return AVERROR(AVERROR_INVALIDDATA);
    }
    std::ostream* output = reinterpret_cast<std::ostream*>(opaque);
    
    output->write(reinterpret_cast<char*>(buf), buf_size);
    output->flush();
    return 0;
}

Muxer::~Muxer()
{
    avio_context_free(&_format_context->pb);
    avformat_free_context(_format_context);
    av_free(_io_buf);
}