#ifndef DECODER_HPP
#define DECODER_HPP

#include "Codec.hpp"
#include <opencv2/core/mat.hpp>

class Decoder : public Codec
{
public:
    Decoder(const std::string& codec_name);

    Decoder(const char* codec_name);

    Decoder(const AVCodecID& codec_id);

    virtual ~Decoder();

    uint64_t parse(std::string& bitstream);

    void decode(cv::Mat& out);
private:

    void init();

    AVCodecParserContext* _parser;

    static CodecType _codec_functions;

};


#endif