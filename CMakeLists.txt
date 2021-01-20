cmake_minimum_required(VERSION 3.17)
project(anomaly_detector)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(anomaly_detector src/MainTrain4.cpp
        src/AnomalyDetector.h
        src/SimpleAnomalyDetector.cpp
        src/SimpleAnomalyDetector.h
        src/timeseries.cpp
        src/timeseries.h
        src/anomaly_detection_util.cpp
        src/anomaly_detection_util.h
        src/minCircle.cpp
        src/minCircle.h
        src/HybridAnomalyDetector.cpp
        src/HybridAnomalyDetector.h)
