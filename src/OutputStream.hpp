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
    OutputStream(AVFormatContext* format_context, const StreamOptions& options, std::ostream& output);
    virtual ~OutputStream();

    void writeVideo(const cv::Mat& frame);
    void writeAudio();
private:
    AVStream* _video_stream;
    AVStream* _audio_stream;

    std::ostream& _output;

    std::shared_ptr<VideoEncoder> _video_encoder;
    std::shared_ptr<AudioEncoder> _audio_encoder;
};

#endif