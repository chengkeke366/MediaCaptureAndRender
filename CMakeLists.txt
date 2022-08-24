cmake_minimum_required(VERSION 3.15)

project(mediacapture)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("FFmpegHome," $ENV{FFmpegHome})

set(CMAKE_PREFIX_PATH $ENV{QTDIR})
include_directories($ENV{FFmpegHome}/include)
include_directories(${PROJECT_SOURCE_DIR})
link_directories($ENV{FFmpegHome}/lib)


message("PROJECT_SOURCE_DIR," ${PROJECT_SOURCE_DIR})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

file(GLOB video_module VideoModule/*.cpp VideoModule/*.h)
file(GLOB audio_module AudioModule/*.cpp AudioModule/*.h)

source_group(video_module FILES ${video_module})
source_group(audio_module FILES ${audio_module})

if(WIN32)
file(GLOB win_video_capture VideoModule/windows/*.cpp VideoModule/windows/*.h)
file(GLOB win_audio_capture AudioModule/windows/*.cpp AudioModule/windows/*.h)
source_group(video_module\\windows FILES ${win_video_capture})
source_group(audio_module\\windows  FILES ${win_audio_capture})
endif()

link_libraries(strmbase.lib)
add_executable(mediacapture  MainForm.ui
global.h
MainForm.h
MainForm.cpp
main.cpp
${video_module}
${audio_module}
${win_video_capture}
${win_audio_capture}
)

target_link_libraries(mediacapture  Qt5::Widgets Qt5::Core)