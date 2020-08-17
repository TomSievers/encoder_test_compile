#include "Muxing.hpp"
#include "StreamOptions.hpp"
#include "StreamIO.hpp"
#include <stdexcept>

Muxer::Muxer(const char* filename, StreamOptions& options)
    : video_st(NULL), audio_st(NULL),
    oc(NULL),
    audio_codec(NULL),   video_codec(NULL),
    _io_buf(NULL),
    have_video(false),   have_audio(false),
    encode_video(false), encode_audio(false)
{
    initFile(filename, options);
}

Muxer::Muxer(const std::string& filename, StreamOptions& options)
    : video_st(NULL), audio_st(NULL),
    oc(NULL),
    audio_codec(NULL),   video_codec(NULL),
    _io_buf(NULL),
    have_video(false),   have_audio(false),
    encode_video(false), encode_audio(false)
{
    initFile(filename.c_str(), options);
}
    
    
Muxer::Muxer(std::ostream& output_stream, const std::string& output_format, StreamOptions& options)
    : video_st(NULL), audio_st(NULL),
    oc(NULL),
    audio_codec(NULL),   video_codec(NULL),
    _io_buf(NULL),
    have_video(false),   have_audio(false),
    encode_video(false), encode_audio(false)
{
    initStream(output_stream, output_format.c_str(), options);
}

Muxer::Muxer(std::ostream& output_stream, const char* output_format, StreamOptions& options)
    : video_st(NULL), audio_st(NULL),
    oc(NULL),
    audio_codec(NULL),   video_codec(NULL),
    _io_buf(NULL),
    have_video(false),   have_audio(false),
    encode_video(false), encode_audio(false)
{
    initStream(output_stream, output_format, options);
}

void Muxer::initFile(const char* filename, StreamOptions& options)
{
    int ret;
    avformat_alloc_output_context2(&oc, NULL, NULL, filename);
    if(oc == NULL)
    {
        throw(std::runtime_error("Unable to deduce output format from file name"));
    }

    if (oc->oformat->video_codec != AV_CODEC_ID_NONE) {
        video_st = new OutputStream(oc, &video_codec, oc->oformat->video_codec);
        have_video = true;
        encode_video = true;
    }
    
    if (oc->oformat->audio_codec != AV_CODEC_ID_NONE) {
        audio_st = new OutputStream(oc, &audio_codec, oc->oformat->audio_codec);
        have_audio = true;
        encode_audio = true;
    }

    if (have_video)
        video_st->open_video(oc, video_codec, NULL);
    if (have_audio)
        audio_st->open_audio(oc, audio_codec, NULL);
    av_dump_format(oc, 0, filename, 1);

    if (!(oc->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) 
        {
            throw(std::runtime_error("Unable to open file: " + std::string(filename)));
        }
    }
}

void Muxer::initStream(std::ostream& output_stream, const char* output_format, StreamOptions& options)
{
    _io_buf = reinterpret_cast<uint8_t *>(av_malloc(sizeof(uint8_t)*_io_buf_size));

    if(_io_buf == NULL)
    {
        throw(std::runtime_error("Out of memory, unable to allocate buffer of size: " + std::to_string(sizeof(uint8_t)*_io_buf_size)));
    }

    int ret = avformat_alloc_output_context2(&oc, NULL, output_format, NULL);
    if(ret < 0)
    {
        char error[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, ret);
        throw std::runtime_error("Unable to allocate output context using output format " + std::string(output_format) + ", error: " + std::string(error));
    }

    oc->pb = avio_alloc_context(_io_buf, _io_buf_size, 1, &output_stream, NULL, &StreamIO::write, &StreamIO::seek);

    if(oc->pb == NULL)
    {
        throw(std::runtime_error("Unable to allocate output"));
    }

    if (oc->oformat->video_codec != AV_CODEC_ID_NONE) {
        video_st = new OutputStream(oc, &video_codec, oc->oformat->video_codec);
        have_video = true;
        encode_video = true;
    }
    
    if (oc->oformat->audio_codec != AV_CODEC_ID_NONE) {
        audio_st = new OutputStream(oc, &audio_codec, oc->oformat->audio_codec);
        have_audio = true;
        encode_audio = true;
    }

    if (have_video)
        video_st->open_video(oc, video_codec, NULL);
    if (have_audio)
        audio_st->open_audio(oc, audio_codec, NULL);
    av_dump_format(oc, 0, NULL, 1);
}

void Muxer::writeHeader()
{
    if (avformat_write_header(oc, NULL) < 0) {
        throw(std::runtime_error("Error occurred when writing header"));
    }
}

void Muxer::write()
{
    while (encode_video || encode_audio) {
        /* select the stream to encode */
        if (encode_video &&
            (!encode_audio || av_compare_ts(video_st->getPts(), video_st->getCodecContext()->time_base,
                                            audio_st->getPts(), audio_st->getCodecContext()->time_base) <= 0)) 
        {
            encode_video = !video_st->write_video_frame(oc);
        } else {
            encode_audio = !audio_st->write_audio_frame(oc);
        }
    }
}

void Muxer::writeTrailer()
{
    av_write_trailer(oc);
}

Muxer::~Muxer()
{
    if (have_video)
        delete video_st;
    if (have_audio)
        delete audio_st;
    if (!(oc->oformat->flags & AVFMT_NOFILE))
        avio_closep(&oc->pb);
    if(_io_buf != NULL)
        av_free(_io_buf);
    

    avformat_free_context(oc);
}