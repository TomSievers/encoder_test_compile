#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

extern "C"
{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <math.h>
    #include <libavutil/avassert.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/opt.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/timestamp.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
}

#define STREAM_DURATION 10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT AV_PIX_FMT_YUV420P /* default pix_fmt */
#define SCALE_FLAGS SWS_BICUBIC

class OutputStream 
{
public:
    OutputStream(AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id);
    virtual ~OutputStream();

    int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *_st, AVPacket *pkt);

    void open_audio(AVFormatContext *oc, AVCodec *codec, AVDictionary *opt_arg);
    int write_audio_frame(AVFormatContext *oc);

    void open_video(AVFormatContext *oc, AVCodec *codec, AVDictionary *opt_arg);
    int write_video_frame(AVFormatContext *oc);

    int64_t getPts();

    AVCodecContext* getCodecContext();

private:
    AVFrame *get_audio_frame();
    AVFrame *get_video_frame();

    AVStream * _st;
    AVCodecContext * _enc;
    int64_t _next_pts;
    int _samples_count;
    AVFrame * _frame;
    AVFrame * _tmp_frame;
    float _t, _tincr, _tincr2;
    SwsContext *_sws_ctx;
    SwrContext *_swr_ctx;
};

#endif