#include "VideoEncoder.hpp"

#include "Decoder.hpp"

#include "Muxer.hpp"

#include "Demuxer.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <boost/asio.hpp>

#include <chrono>

#include <fstream>

int main(int argc, char const *argv[])
{
    (void) argc;
    (void) argv;

    Resolution res = {.width=1920, .height=1080};
    VideoEncoder encoder(AV_CODEC_ID_H264, res, AV_PIX_FMT_YUV420P, 4000000, 30);

    Decoder decoder(AV_CODEC_ID_H264);

    std::ofstream file;

    Muxer muxer(file, "mp4");

    muxer.writeHeader();

    
    return 0;
}