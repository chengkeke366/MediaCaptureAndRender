cmake_minimum_required(VERSION 3.15)

project(mediacapture)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("FFmpegHome," $ENV{FFmpegHome})

set(CMAKE_PREFIX_PATH $ENV{QTDIR})
include_directories($ENV{FFmpegHome}/include)
link_directories($ENV{FFmpegHome}/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

file(GLOB video_capture_SRC VideoCapture/*.cpp VideoCapture/*.h)
add_executable(mediacapture  MainForm.ui
MainForm.cpp
main.cpp
${video_capture_SRC}
)

target_link_libraries(mediacapture  Qt5::Widgets Qt5::Core )