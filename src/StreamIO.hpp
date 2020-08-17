#ifndef STREAM_IO_HPP
#define STREAM_IO_HPP

#include <cstdint>
#include <iostream>
#include <stdexcept>

extern "C"
{
    #include <libavutil/error.h>
}

namespace StreamIO
{
    int write(void* opaque, uint8_t* buf, int buf_size)
    {
        if(opaque == NULL)
        {
            return AVERROR(AVERROR_INVALIDDATA);
        }
        std::ostream* output = reinterpret_cast<std::ostream*>(opaque);
        
        output->write(reinterpret_cast<char*>(buf), buf_size);
        output->flush();
        return 0;
    }

    int64_t seek(void* opaque, int64_t offset, int whence)
    {
        if(opaque == NULL)
        {
            return AVERROR(AVERROR_INVALIDDATA);
        }
        std::ostream* output = reinterpret_cast<std::ostream*>(opaque);

        switch (whence)
        {
        case SEEK_CUR:
            output->seekp(offset, std::ios_base::cur);
            break;
        case SEEK_SET:
            output->seekp(offset, std::ios_base::beg);
            break;
        case SEEK_END:
            output->seekp(offset, std::ios_base::end);
            break;
        default:
            throw(std::runtime_error("Invalid whence in seek"));
            break;
        }

        return output->tellp(); 
    }
}



#endif