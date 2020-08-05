project (VideoEncoder_test)
cmake_minimum_required(VERSION 3.10)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.68 REQUIRED COMPONENTS system)

find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)

add_definitions("-Wall -Wextra -Wconversion")

set(PKG_CONFIG_LIBS "libavdevice"
    "libavformat"
    "libavfilter"
    "libavcodec"
    "libswresample"
    "libswscale"
    "libavutil"
)

set(PKG_CONFIG_LIBRARIES "")

set(PKG_CONFIG_INCLUDES "")

foreach(LIB IN LISTS PKG_CONFIG_LIBS)
    pkg_check_modules(${LIB} REQUIRED ${LIB})
    set(PKG_CONFIG_INCLUDES "${PKG_CONFIG_INCLUDES}" "${${LIB}_INCLUDE_DIRS}")
    link_directories(${${LIB}_LIBRARY_DIRS})
    set(PKG_CONFIG_LIBRARIES "${PKG_CONFIG_LIBRARIES}" "${${LIB}_LIBRARIES}")
endforeach()


link_directories(${AVCODEC_LIBRARY_DIRS})

include_directories(${PKG_CONFIG_INCLUDES} ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

set(SRC_FILES
    src/main.cpp 
    src/Codec.cpp 
    src/VideoEncoder.cpp 
    src/Decoder.cpp 
    src/Muxer.cpp
    src/OutputStream.cpp
    src/AudioEncoder.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

if(UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()

target_link_libraries(${PROJECT_NAME} ${PKG_CONFIG_LIBRARIES} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})

