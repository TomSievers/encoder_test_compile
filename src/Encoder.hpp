#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <string>
#include <cstdint>
#include <ostream>

#include <opencv2/core/mat.hpp>

#include "Codec.hpp"

struct Resolution
{
	uint16_t width;
	uint16_t height;
};

class Encoder : public Codec
{
public:
	Encoder(const std::string& codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

	Encoder(const char* codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

	Encoder(const AVCodecID& codec_id, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

	virtual ~Encoder();

	void encode(const cv::Mat& frame, std::ostream& output);
private:
	void copyToFrame(const cv::Mat& frame);

	void init(const std::string& codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

	cv::Mat _channels[8];
};


#endif