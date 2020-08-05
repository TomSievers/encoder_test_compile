#include "OutputStream.hpp"

OutputStream::OutputStream(AVFormatContext* format_context, const StreamOptions& options, std::ostream& output)
    : _output(output)
{
    int ret;
    
    if(options.video_opt.codec != AV_CODEC_ID_NONE)
    {
        _video_encoder = std::make_shared<VideoEncoder>(
        options.video_opt.codec, 
        options.video_opt.resolution, 
        options.video_opt.format, 
        options.video_opt.bit_rate, 
        options.video_opt.frame_rate);

        _video_stream = avformat_new_stream(format_context, NULL);

        if (!_video_stream) 
        {
            throw(std::runtime_error("Could not allocate video stream"));
        }

        _video_stream->id = format_context->nb_streams-1;

        ret = avcodec_parameters_from_context(_video_stream->codecpar, _video_encoder->getContext());
        if (ret < 0) {
            throw(std::runtime_error("Could not copy the video stream parameters"));
        }

    }

    if(options.audio_opt.codec != AV_CODEC_ID_NONE)
    {
        _audio_encoder = std::make_shared<AudioEncoder>(
            options.audio_opt.codec,
            options.audio_opt.sample_rate,
            options.audio_opt.sample_fmt,
            options.audio_opt.n_channels,
            options.audio_opt.bit_rate
        );

        _audio_stream = avformat_new_stream(format_context, NULL);

        if (!_audio_stream) 
        {
            throw(std::runtime_error("Could not allocate audio stream"));
        }

        _audio_stream->id = format_context->nb_streams-1;

        ret = avcodec_parameters_from_context(_audio_stream->codecpar, _audio_encoder->getContext());
        if (ret < 0) 
        {
            throw(std::runtime_error("Could not copy the audio stream parameters"));
        }
    }    
    
    if (format_context->oformat->flags & AVFMT_GLOBALHEADER)
    {
        if(_video_encoder != nullptr)
        {
            _video_encoder->getContext()->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }

        if(_audio_encoder != nullptr)
        {
            _audio_encoder->getContext()->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
    }

}


OutputStream::~OutputStream()
{
}
