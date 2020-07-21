#include "Encoder.hpp"

#include "Decoder.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <boost/asio.hpp>

#include <chrono>

#include <iostream>

int main(int argc, char const *argv[])
{
    Resolution res = {.width=1920, .height=1080};
    Encoder encoder(AV_CODEC_ID_H264, res, AV_PIX_FMT_YUV420P, 4000000, 30);

    Decoder decoder(AV_CODEC_ID_H264);

    cv::Mat img = cv::imread("../megumin.jpg");

    cv::Mat frame;

    std::stringstream buf;

    cv::cvtColor(img, frame, cv::COLOR_BGR2YUV_I420);
    frame.resize(1080);

    for(int i = 0; i < 5; ++i)
    {
        encoder.encode(frame, buf);

        std::string bitstream = buf.str();

        decoder.parse(bitstream);

        decoder.decode(frame);

        cv::imshow("test", frame);

        cv::waitKey(1);
    }
    return 0;
}