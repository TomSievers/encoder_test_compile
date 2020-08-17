#ifndef AUDIO_HPP
#define AUDIO_HPP

extern "C"
{
    #include <libavutil/frame.h>
}

struct AudioFrame
{
    uint8_t* data[AV_NUM_DATA_POINTERS];
};


#endif