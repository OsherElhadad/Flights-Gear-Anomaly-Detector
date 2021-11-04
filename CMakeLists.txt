cmake_minimum_required(VERSION 3.20)
project(BiuProject)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BiuProject
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        MainTrain.cpp AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h)
