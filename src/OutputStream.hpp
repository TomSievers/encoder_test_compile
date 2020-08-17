#ifndef OUTPUT_STREAM_HPP
#define OUTPUT_STREAM_HPP

#include "VideoEncoder.hpp"
#include "AudioEncoder.hpp"

#include "StreamOptions.hpp"

#include <memory>

extern "C"
{
    #include "libavformat/avformat.h"
}

class OutputStream
{
public:
    OutputStream(AVFormatContext* format_context, const StreamOptions& options);
    virtual ~OutputStream();

    std::shared_ptr<VideoEncoder> getVideoEncoder();
    std::shared_ptr<AudioEncoder> getAudioEncoder();

    void writeAudio(AVPacket* packet);
    void writeVideo(AVPacket* packet);
private:
    void write(AVPacket* packet, AVStream* stream, const AVRational* time_base);

    AVStream* _video_stream;
    AVStream* _audio_stream;

    AVFormatContext* _format_context;

    std::shared_ptr<VideoEncoder> _video_encoder;
    std::shared_ptr<AudioEncoder> _audio_encoder;
};

#endif