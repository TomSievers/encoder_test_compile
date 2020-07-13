#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <string>
#include <cstdint>
#include <memory>

#include <libavcodec/avcodec.h>

struct Resolution
{
    uint16_t width;
    uint16_t height;
};

class Encoder
{
public:
    Encoder(const std::string& codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);
    Encoder(const char* codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);
    Encoder(const AVCodecID& codec_id, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);
    ~Encoder();

    
private:
    void init(const std::string& codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

    AVCodec* _codec;
    AVCodecContext* _codec_context;
    
    AVPacket* _packet;
    AVFrame* _frame;
};


#endif