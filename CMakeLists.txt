project (encoder_test)
cmake_minimum_required(VERSION 3.10)

find_package(PkgConfig REQUIRED)

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

add_executable(${PROJECT_NAME} src/main.cpp src/Encoder.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PKG_CONFIG_INCLUDES})

target_link_libraries(${PROJECT_NAME} ${PKG_CONFIG_LIBRARIES})