#include "Muxer.hpp"

#include <iostream>

Muxer::Muxer(const std::string& file_name)
{
}

Muxer::Muxer(const char* file_name)
{
}

void Muxer::init(const char* file_name)
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

Muxer::~Muxer()
{
}