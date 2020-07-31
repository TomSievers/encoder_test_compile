#ifndef VIDEOENCODER_HPP
#define VIDEOENCODER_HPP

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

class VideoEncoder : public Codec
{
public:

	/**
	 * @brief Construct a new Video Encoder object
	 * 
	 * @param codec_name name of the codec to use
	 * @param res resolution of the video to encode
	 * @param format pixel format that is supplied
	 * @param bit_rate bit rate of the resulting bit stream
	 * @param frame_rate frame rate of the video
	 */
	VideoEncoder(const std::string& codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

	/**
	 * @brief Construct a new Video Encoder object
	 * 
	 * @param codec_name name of the codec to use
	 * @param res resolution of the video to encode
	 * @param format pixel format that is supplied
	 * @param bit_rate bit rate of the resulting bit stream
	 * @param frame_rate frame rate of the video
	 */
	VideoEncoder(const char* codec_name, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

	/**
	 * @brief Construct a new Video Encoder object
	 * 
	 * @param codec_id id of the codec to use
	 * @param res resolution of the video to encode
	 * @param format pixel format that is supplied
	 * @param bit_rate bit rate of the resulting bit stream
	 * @param frame_rate frame rate of the video
	 */
	VideoEncoder(const AVCodecID& codec_id, const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

	virtual ~VideoEncoder();

	/**
	 * @brief Encode frame and copy resulting bitstream to ouput stream
	 * 
	 * @param frame frame to encode
	 * @param output output stream to put bitstream into
	 */
	void encode(const cv::Mat& frame, std::ostream& output);
private:

	void copyToFrame(const cv::Mat& frame);

	void init(const Resolution& res, const AVPixelFormat& format, const uint32_t bit_rate, uint8_t frame_rate);

	cv::Mat _channels[8];
};


#endif //VIDEOENCODER_HPP