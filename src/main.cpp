#include "Encoder.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    Resolution res = {.width=1920, .height=1080};
    Encoder encoder(AV_CODEC_ID_H265, res, AV_PIX_FMT_YUVJ420P, 4000000, 30);

    cv::Mat img = cv::imread("../megumin.jpg");

    for(int i = 0; i < 30; ++i)
    {
        cv::cvtColor(img, img, cv::COLOR_BGR2YUV_I420);
        img.resize(1080);
        encoder.encode(img, std::cout);
    }
    
    return 0;
}