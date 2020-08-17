#include "OutputStream.hpp"

extern "C"
{
    #include <libavutil/opt.h>
    #include <libavutil/timestamp.h>
}

OutputStream::OutputStream(AVFormatContext* format_context, const StreamOptions& options)
    : _format_context(format_context)
{
    if(_format_context == NULL)
    {
        throw(std::runtime_error("Invalid format context supplied"));
    }
    
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

        _video_stream->time_base = _video_encoder->getContext()->time_base;

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

        _audio_stream->time_base = (AVRational){ 1, _audio_encoder->getContext()->sample_rate}; 
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

void OutputStream::writeAudio(AVPacket* packet)
{
    write(packet, _audio_stream, &_audio_encoder->getContext()->time_base);
}

void OutputStream::writeVideo(AVPacket* packet)
{
    write(packet, _video_stream, &_video_encoder->getContext()->time_base);
}

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
    char pkt_pts[AV_TS_MAX_STRING_SIZE];
    av_ts_make_string(pkt_pts, pkt->pts);
    char pkt_dts[AV_TS_MAX_STRING_SIZE];
    av_ts_make_string(pkt_dts, pkt->dts);
    char pkt_dur[AV_TS_MAX_STRING_SIZE];
    av_ts_make_string(pkt_dur, pkt->duration);
    char pkt_pts_time[AV_TS_MAX_STRING_SIZE];
    av_ts_make_time_string(pkt_pts_time, pkt->pts, time_base);
    char pkt_dts_time[AV_TS_MAX_STRING_SIZE];
    av_ts_make_time_string(pkt_dts_time, pkt->dts, time_base);
    char pkt_dur_time[AV_TS_MAX_STRING_SIZE];
    av_ts_make_time_string(pkt_dur_time, pkt->duration, time_base);
    printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           pkt_pts, pkt_pts_time,
           pkt_dts, pkt_dts_time,
           pkt_dur, pkt_dur_time,
           pkt->stream_index);
}

void OutputStream::write(AVPacket* packet, AVStream* stream, const AVRational* time_base)
{
    av_packet_rescale_ts(packet, *time_base, stream->time_base);
    packet->stream_index = stream->index;

    log_packet(_format_context, packet);
    int ret = av_interleaved_write_frame(_format_context, packet);

    if(ret < 0)
    {
        char error[AV_ERROR_MAX_STRING_SIZE];
		av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
		throw std::runtime_error("Unable to write packet " + std::string(error));
    }
}

std::shared_ptr<VideoEncoder> OutputStream::getVideoEncoder()
{
    return _video_encoder;
}

std::shared_ptr<AudioEncoder> OutputStream::getAudioEncoder()
{
    return _audio_encoder;
}


OutputStream::~OutputStream()
{
    
}
