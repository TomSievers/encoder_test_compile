#include "Example.hpp"

#include <stdexcept>

OutputStream::OutputStream(AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id)
    : _st(NULL),
    _enc(NULL),
    _next_pts(0),
    _samples_count(0),
    _frame(NULL),
    _tmp_frame(NULL),
    _t(0), _tincr(0), _tincr2(0),
    _sws_ctx(NULL),
    _swr_ctx(NULL)
{
    AVCodecContext *c;
    int i;
    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        throw(std::runtime_error("Could not find encoder for " + std::string(avcodec_get_name(codec_id))));
    }
    _st = avformat_new_stream(oc, NULL);
    if (!_st) {
        throw(std::runtime_error("Could not allocate stream"));
    }
    _st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c) {
        throw(std::runtime_error("Could not alloc an encoding context"));
    }
    _enc = c;
    switch ((*codec)->type) {
    case AVMEDIA_TYPE_AUDIO:
        c->sample_fmt  = (*codec)->sample_fmts ?
            (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
        c->bit_rate    = 64000;
        c->sample_rate = 44100;
        if ((*codec)->supported_samplerates) {
            c->sample_rate = (*codec)->supported_samplerates[0];
            for (i = 0; (*codec)->supported_samplerates[i]; i++) {
                if ((*codec)->supported_samplerates[i] == 44100)
                    c->sample_rate = 44100;
            }
        }
        c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
        c->channel_layout = AV_CH_LAYOUT_STEREO;
        if ((*codec)->channel_layouts) {
            c->channel_layout = (*codec)->channel_layouts[0];
            for (i = 0; (*codec)->channel_layouts[i]; i++) {
                if ((*codec)->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
                    c->channel_layout = AV_CH_LAYOUT_STEREO;
            }
        }
        c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
        _st->time_base = (AVRational){ 1, c->sample_rate };
        break;
    case AVMEDIA_TYPE_VIDEO:
        c->codec_id = codec_id;
        c->bit_rate = 400000;
        /* Resolution must be a multiple of two. */
        c->width    = 352;
        c->height   = 288;
        /* timebase: This is the fundamental unit of time (in seconds) in terms
         * of which frame timestamps are represented. For fixed-fps content,
         * timebase should be 1/framerate and timestamp increments should be
         * identical to 1. */
        _st->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
        c->time_base = _st->time_base;
        c->gop_size = 12; /* emit one intra frame every twelve frames at most */
        c->pix_fmt = STREAM_PIX_FMT;
        if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            /* just for testing, we also add B-frames */
            c->max_b_frames = 2;
        }
        if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
            /* Needed to avoid using macroblocks in which some coeffs overflow.
             * This does not happen with normal video, it just happens here as
             * the motion of the chroma plane does not match the luma plane. */
            c->mb_decision = 2;
        }
    break;
    default:
        break;
    }
    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

static AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
                                  uint64_t channel_layout,
                                  int sample_rate, int nb_samples)
{
    AVFrame *frame = av_frame_alloc();
    int ret;
    if (!frame) {
        throw(std::runtime_error("Error allocating an audio frame"));
    }
    frame->format = sample_fmt;
    frame->channel_layout = channel_layout;
    frame->sample_rate = sample_rate;
    frame->nb_samples = nb_samples;
    if (nb_samples) {
        ret = av_frame_get_buffer(frame, 0);
        if (ret < 0) {
            throw(std::runtime_error("Error allocating an audio buffer"));
        }
    }
    return frame;
}

void OutputStream::open_audio(AVFormatContext *oc, AVCodec *codec, AVDictionary *opt_arg)
{
    (void) oc;
    int nb_samples;
    int ret;
    AVDictionary *opt = NULL;
    /* open it */
    av_dict_copy(&opt, opt_arg, 0);
    ret = avcodec_open2(_enc, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        throw(std::runtime_error("Could not open audio codec"));
    }
    /* init signal generator */
    _t     = 0;
    _tincr = static_cast<float>(2 * M_PI * 110.0 / _enc->sample_rate);
    /* increment frequency by 110 Hz per second */
    _tincr2 = static_cast<float>(2 * M_PI * 110.0 / _enc->sample_rate / _enc->sample_rate);
    if (_enc->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
        nb_samples = 10000;
    else
        nb_samples = _enc->frame_size;
    _frame     = alloc_audio_frame(_enc->sample_fmt, _enc->channel_layout,
                                       _enc->sample_rate, nb_samples);
    _tmp_frame = alloc_audio_frame(AV_SAMPLE_FMT_S16, _enc->channel_layout,
                                       _enc->sample_rate, nb_samples);
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(_st->codecpar, _enc);
    if (ret < 0) {
        throw(std::runtime_error("Could not copy the stream parameters"));
    }
    /* create resampler context */
        _swr_ctx = swr_alloc();
        if (!_swr_ctx) {
            throw(std::runtime_error("Could not allocate resampler context"));
        }
        /* set options */
        av_opt_set_int       (_swr_ctx, "in_channel_count",   _enc->channels,       0);
        av_opt_set_int       (_swr_ctx, "in_sample_rate",     _enc->sample_rate,    0);
        av_opt_set_sample_fmt(_swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
        av_opt_set_int       (_swr_ctx, "out_channel_count",  _enc->channels,       0);
        av_opt_set_int       (_swr_ctx, "out_sample_rate",    _enc->sample_rate,    0);
        av_opt_set_sample_fmt(_swr_ctx, "out_sample_fmt",     _enc->sample_fmt,     0);
        /* initialize the resampling context */
        if ((ret = swr_init(_swr_ctx)) < 0) {
            throw(std::runtime_error("Failed to initialize the resampling context"));
        }
}
/* Prepare a 16 bit dummy audio frame of 'frame_size' samples and
 * 'nb_channels' channels. */
AVFrame * OutputStream::get_audio_frame()
{
    AVFrame *frame = _tmp_frame;
    int j, i, v;
    int16_t *q = (int16_t*)frame->data[0];
    /* check if we want to generate more frames */
    if (av_compare_ts(_next_pts, _enc->time_base,
                      STREAM_DURATION, (AVRational){ 1, 1 }) >= 0)
        return NULL;
    for (j = 0; j <frame->nb_samples; j++) {
        v = (int)(sin(_t) * 10000);
        for (i = 0; i < _enc->channels; i++)
            *q++ = static_cast<int16_t>(v);
        _t     += _tincr;
        _tincr += _tincr2;
    }
    _tmp_frame->pts = _next_pts;
    _next_pts  += _tmp_frame->nb_samples;
    return _tmp_frame;
}
/*
 * encode one audio frame and send it to the muxer
 * return 1 when encoding is finished, 0 otherwise
 */
int OutputStream::write_audio_frame(AVFormatContext *oc)
{
    AVPacket pkt = {};
    AVFrame *frame;
    int ret;
    int got_packet;
    int dst_nb_samples;
    av_init_packet(&pkt);
    frame = get_audio_frame();
    if (frame) {
        /* convert samples from native format to destination codec format, using the resampler */
            /* compute destination number of samples */
            dst_nb_samples = static_cast<int>(av_rescale_rnd(swr_get_delay(_swr_ctx, _enc->sample_rate) + frame->nb_samples,
                                            _enc->sample_rate, _enc->sample_rate, AV_ROUND_UP));
            av_assert0(dst_nb_samples == frame->nb_samples);
        /* when we pass a frame to the encoder, it may keep a reference to it
         * internally;
         * make sure we do not overwrite it here
         */
        ret = av_frame_make_writable(frame);
        if (ret < 0)
            throw(std::runtime_error("Frame unwritable"));
        /* convert to destination format */
        ret = swr_convert(_swr_ctx,
                          _frame->data, dst_nb_samples,
                          (const uint8_t **)frame->data, frame->nb_samples);
        if (ret < 0) {
            throw(std::runtime_error("Error while converting"));
        }
        frame = _frame;
        frame->pts = av_rescale_q(_samples_count, (AVRational){1, _enc->sample_rate}, _enc->time_base);
        _samples_count += dst_nb_samples;
    }
    ret = avcodec_encode_audio2(_enc, &pkt, frame, &got_packet);
    if (ret < 0) {
        throw(std::runtime_error("Error encoding audio frame"));
    }
    if (got_packet) {
        ret = write_frame(oc, &_enc->time_base, _st, &pkt);
        if (ret < 0) {
            throw(std::runtime_error("Error while writing audio frame"));
        }
    }
    return (frame || got_packet) ? 0 : 1;
}

static AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    int ret;
    picture = av_frame_alloc();
    if (!picture)
        return NULL;
    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;
    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        throw(std::runtime_error("Could not allocate frame data"));
    }
    return picture;
}

void OutputStream::open_video(AVFormatContext *oc, AVCodec *codec, AVDictionary *opt_arg)
{
    (void) oc;
    int ret;
    AVDictionary *opt = NULL;
    av_dict_copy(&opt, opt_arg, 0);
    /* open the codec */
    ret = avcodec_open2(_enc, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        throw(std::runtime_error("Could not open video codec"));
    }
    /* allocate and init a re-usable frame */
    _frame = alloc_picture(_enc->pix_fmt, _enc->width, _enc->height);
    if (!_frame) {
        throw(std::runtime_error("Could not allocate video frame"));
    }
    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    _tmp_frame = NULL;
    if (_enc->pix_fmt != AV_PIX_FMT_YUV420P) {
        _tmp_frame = alloc_picture(AV_PIX_FMT_YUV420P, _enc->width, _enc->height);
        if (!_tmp_frame) {
            throw(std::runtime_error("Could not allocate temporary picture"));
        }
    }
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(_st->codecpar, _enc);
    if (ret < 0) {
        throw(std::runtime_error("Could not copy the stream parameters"));
    }
}

static void fill_yuv_image(AVFrame *pict, int64_t frame_index,
                           int width, int height)
{
    int x, y;
    int64_t i = frame_index;
    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            pict->data[0][y * pict->linesize[0] + x] = static_cast<uint8_t>(x + y + i * 3);
    /* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = static_cast<uint8_t>(128 + y + i * 2);
            pict->data[2][y * pict->linesize[2] + x] = static_cast<uint8_t>(64 + x + i * 5);
        }
    }
}

AVFrame * OutputStream::get_video_frame()
{
    /* check if we want to generate more frames */
    if (av_compare_ts(_next_pts, _enc->time_base,
                      STREAM_DURATION, (AVRational){ 1, 1 }) >= 0)
        return NULL;
    /* when we pass a frame to the encoder, it may keep a reference to it
     * internally; make sure we do not overwrite it here */
    if (av_frame_make_writable(_frame) < 0)
        throw(std::runtime_error("Frame unwriteable"));
    if (_enc->pix_fmt != AV_PIX_FMT_YUV420P) {
        /* as we only generate a YUV420P picture, we must convert it
         * to the codec pixel format if needed */
        if (!_sws_ctx) {
            _sws_ctx = sws_getContext(_enc->width, _enc->height,
                                          AV_PIX_FMT_YUV420P,
                                          _enc->width, _enc->height,
                                          _enc->pix_fmt,
                                          SCALE_FLAGS, NULL, NULL, NULL);
            if (!_sws_ctx) {
                throw(std::runtime_error("Could not initialize the conversion context"));
            }
        }
        fill_yuv_image(_tmp_frame, _next_pts, _enc->width, _enc->height);
        sws_scale(_sws_ctx, (const uint8_t * const *) _tmp_frame->data,
                  _tmp_frame->linesize, 0, _enc->height, _frame->data,
                  _frame->linesize);
    } else {
        fill_yuv_image(_frame, _next_pts, _enc->width, _enc->height);
    }
    _frame->pts = _next_pts++;
    return _frame;
}

int OutputStream::write_video_frame(AVFormatContext *oc)
{
    int ret;
    AVFrame *frame;
    int got_packet = 0;
    AVPacket pkt = {};
    frame = get_video_frame();
    av_init_packet(&pkt);
    /* encode the image */
    ret = avcodec_encode_video2(_enc, &pkt, frame, &got_packet);
    if (ret < 0) {
        throw(std::runtime_error("Error encoding video frame"));
    }
    if (got_packet) {
        ret = write_frame(oc, &_enc->time_base, _st, &pkt);
    } else {
        ret = 0;
    }
    if (ret < 0) {
        throw(std::runtime_error("Error while writing video frame"));
    }
    return (frame || got_packet) ? 0 : 1;
}

int OutputStream::write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *_st, AVPacket *pkt)
{
    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, *time_base, _st->time_base);
    pkt->stream_index = _st->index;
    /* Write the compressed frame to the media file. */
    return av_interleaved_write_frame(fmt_ctx, pkt);
}

int64_t OutputStream::getPts()
{
    return _next_pts;
}

AVCodecContext* OutputStream::getCodecContext()
{
    return _enc;
}

OutputStream::~OutputStream()
{
    avcodec_free_context(&_enc);
    av_frame_free(&_frame);
    av_frame_free(&_tmp_frame);
    sws_freeContext(_sws_ctx);
    swr_free(&_swr_ctx);
}