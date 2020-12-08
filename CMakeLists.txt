cmake_minimum_required(VERSION 3.17)
project(anomaly_detector)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(anomaly_detector
        src/anomaly_detection_util.cpp
        src/anomaly_detection_util.h
        src/MainTrain.cpp)
