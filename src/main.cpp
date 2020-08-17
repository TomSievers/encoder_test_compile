/*#include "VideoEncoder.hpp"

#include "Decoder.hpp"

#include "Muxer.hpp"

#include "Demuxer.hpp"



#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <boost/asio.hpp>

#include <chrono>

#include <fstream>

#include <iostream>

int main(int argc, char const *argv[])
{
    try
    {
        (void) argc;
        (void) argv;

        Resolution res = {.width=1920, .height=1080};
        VideoEncoder encoder(AV_CODEC_ID_H264, res, AV_PIX_FMT_YUV420P, 4000000, 30);

        Decoder decoder(AV_CODEC_ID_H264);

        cv::Mat img = cv::imread("../testscreen.jpg");

        std::ofstream file("test2.mp4");

        StreamOptions options;

        options.audio_opt.sample_rate = 44100;
        options.audio_opt.sample_fmt = AV_SAMPLE_FMT_FLT;
        options.audio_opt.n_channels = 2;
        options.audio_opt.bit_rate = 64000;

        Resolution res = {.width=1920, .height=1080};

        options.video_opt.resolution = res;
        options.video_opt.format = AV_PIX_FMT_YUVJ420P;
        options.video_opt.bit_rate = 4000000;
        options.video_opt.frame_rate = 30;

        Muxer muxer(file, "mp4", options);

        muxer.writeHeader();

        for(int i = 0; i < 60; ++i)
        {
            muxer.write(img);
        }

        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}*/

#include "Muxing.hpp"
#include <stdexcept>
#include <iostream>

int main(int argc, char **argv)
{
    try
    {
        if (argc < 2) {
            printf("usage: %s output_file\n"
                "API example program to output a media file with libavformat.\n"
                "This program generates a synthetic audio and video stream, encodes and\n"
                "muxes them into a file named output_file.\n"
                "The output format is automatically guessed according to the file extension.\n"
                "Raw images can also be output by using '%%d' in the filename.\n"
                "\n", argv[0]);
            return 1;
        }

        StreamOptions options;
        
        Muxer muxer(argv[1], options);

        muxer.writeHeader();
        muxer.write();
        muxer.writeTrailer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}