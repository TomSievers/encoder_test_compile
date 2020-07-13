project (encoder_test)
cmake_minimum_required(VERSION 3.10)

find_package(FFMPEG REQUIRED)

find_package(PkgConfig REQUIRED)

pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(RESAMPLE REQUIRED libswresample)
pkg_check_modules(AVCODEC REQUIRED libavcodec)

link_directories(${AVCODEC_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/Encoder.cpp)

include_directories(${OpenCV_INCLUDE_DIRS} ${AVUTIL_INCLUDE_DIRS} ${RESAMPLE_INCLUDE_DIRS} ${AVCODEC_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${AVUTIL_LIBRARIES} ${RESAMPLE_LIBRARIES} ${AVCODEC_LIBRARIES})