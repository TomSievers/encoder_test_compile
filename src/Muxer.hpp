#ifndef MUXER_HPP
#define MUXER_HPP

#include <string>

#include <opencv2/core/mat.hpp>

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
    Muxer(const std::string& file_name, StreamOptions& options);
    Muxer(const char* file_name, StreamOptions& options);
    Muxer(std::ostream& output_stream, const std::string& output_format, StreamOptions& options);

    void writeHeader();

    void write(const cv::Mat& image);

    void write(const AudioFrame& sound);

    virtual ~Muxer();

private:
    void initFile(const char* file_name, StreamOptions& options);

    AVFormatContext* _format_context;

    AVIOContext* _io_context;
    int _io_buf_size = DEFAULT_CACHE_SIZE;
    uint8_t* _io_buf;

    AVOutputFormat* _format;

    std::unique_ptr<OutputStream> _output;
};




#endif