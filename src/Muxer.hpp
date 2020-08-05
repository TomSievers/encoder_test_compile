#ifndef MUXER_HPP
#define MUXER_HPP

#include <string>

#include "OutputStream.hpp"

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
}

#define DEFAULT_CACHE_SIZE 4096

class Muxer
{

public:
    Muxer(const std::string& file_name);
    Muxer(const char* file_name);
    Muxer(std::ostream& output_stream, const std::string& output_format);

    void writeHeader();

    virtual ~Muxer();

private:
    static int write(void* opaque, uint8_t* buf, int buf_size);
    void initFile(const char* file_name);

    AVFormatContext* _format_context;

    AVIOContext* _io_context;
    int _io_buf_size = DEFAULT_CACHE_SIZE;
    uint8_t* _io_buf;

    AVOutputFormat* _format;

    std::unique_ptr<OutputStream> _output;
};




#endif