#include "Encoder.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <boost/asio.hpp>

#include <chrono>

#include <iostream>

int main(int argc, char const *argv[])
{
    Resolution res = {.width=1920, .height=1080};
    Encoder encoder(AV_CODEC_ID_H265, res, AV_PIX_FMT_YUVJ420P, 4000000, 30);

    cv::Mat img = cv::imread("../megumin.jpg");

    cv::Mat frame;

    boost::asio::streambuf buf;

    std::ostream stream(&buf);

    cv::cvtColor(img, frame, cv::COLOR_BGR2YUV_I420);
    frame.resize(1080);

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 30; ++i)
    {
        encoder.encode(frame, stream);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Elapsed: " <<  std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9 << std::endl;
    
    return 0;
}