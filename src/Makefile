LDLIBS := -LC:/lib_install/lib  -L/mingw64/lib  -L/c/lib_install/lib  -lavdevice -lpsapi -luuid -loleaut32 -lshlwapi -lgdi32 -lvfw32 -lavfilter -lswscale -lpostproc -lavformat -lsecur32 -lws2_32 -lavcodec -lvpx -lpthread -lz -lx264 -lx265 -lstdc++ -liconv -lgcc_s -lgcc -lmfplat -lmfuuid -lstrmiids -lole32 -lswresample -lavutil -lm -luser32 -lbcrypt -lavformat -lsecur32 -lws2_32 -lavcodec -lvpx -lpthread -lz -lx264 -lx265 -lstdc++ -liconv -lgcc_s -lgcc -lmfplat -lmfuuid -lstrmiids -lole32 -lswresample -lavutil -lm -luser32 -lbcrypt -lavfilter -lswscale -lpostproc -lavformat -lsecur32 -lws2_32 -lavcodec -lvpx -lpthread -lz -lx264 -lx265 -lstdc++ -liconv -lgcc_s -lgcc -lmfplat -lmfuuid -lstrmiids -lole32 -lswresample -lavutil -lm -luser32 -lbcrypt -lavcodec -lvpx -lpthread -lz -lx264 -lx265 -lstdc++ -liconv -lgcc_s -lgcc -lmfplat -lmfuuid -lstrmiids -lole32 -lswresample -lavutil -lm -luser32 -lbcrypt -lswresample -lavutil -lm -luser32 -lbcrypt -lswscale -lavutil -lm -luser32 -lbcrypt -lavutil -lm -luser32 -lbcrypt -lavfilter -lpostproc -lavfilter -lswscale -lpostproc -lavformat -lsecur32 -lws2_32 -lavcodec -lvpx -lpthread -lz -lx264 -lx265 -lstdc++ -liconv -lgcc_s -lgcc -lmfplat -lmfuuid -lstrmiids -lole32 -lswresample -lavutil -lm -luser32 -lbcrypt -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32

FFMPEG_LIBS=    libavdevice                        \
                libavformat                        \
                libavfilter                        \
                libavcodec                         \
                libswresample                      \
                libswscale                         \
                libavutil                          \

CXXFLAGS += -Wall -g
CXXFLAGS := $(shell pkg-config --cflags $(FFMPEG_LIBS)) $(CFLAGS)

FILES = main Encoder

OBJS=$(addsuffix .o,$(FILES))

.phony: all clean-test clean

all: $(OBJS) main

clean-test:
	$(RM) test*.pgm test.h264 test.mp2 test.sw test.mpg

clean: clean-test
	$(RM) $(EXAMPLES) $(OBJS)
