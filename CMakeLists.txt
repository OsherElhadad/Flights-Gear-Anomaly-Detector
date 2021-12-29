cmake_minimum_required(VERSION 3.20)
project(BiuProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(.)

add_executable(BiuProject
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        MainTrain.cpp AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h
        timeseries.cpp timeseries.h minCircle.h minCircle.cpp HybridAnomalyDetector.cpp
        HybridAnomalyDetector.h CLI.h CLI.cpp commands.h
        Server.cpp Server.h)
