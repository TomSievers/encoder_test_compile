#ifndef MUXING_HPP
#define MUXING_HPP

#include "Example.hpp"
#include "StreamOptions.hpp"

#define DEFAULT_CACHE_SIZE 4096

class Muxer
{
public:
    Muxer(const char* filename, StreamOptions& options);
    Muxer(const std::string& filename, StreamOptions& options);
    Muxer(std::ostream& output_stream, const std::string& output_format, StreamOptions& options);
    Muxer(std::ostream& output_stream, const char* output_format, StreamOptions& options);
    virtual ~Muxer();
    void writeHeader();
    void write();
    void writeTrailer();
private:
    void initFile(const char* filename, StreamOptions& options);
    void initStream(std::ostream& output_stream, const char* output_format, StreamOptions& options);

    OutputStream* video_st,* audio_st;
    AVFormatContext *oc;
    AVCodec *audio_codec, *video_codec;

    int _io_buf_size = DEFAULT_CACHE_SIZE;
    uint8_t* _io_buf;

    bool have_video, have_audio;
    bool encode_video, encode_audio;
};


#endif