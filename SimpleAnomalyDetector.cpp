/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#include "SimpleAnomalyDetector.h"

// learnNormal - gets a TimeSeries and adds the correlatedFeatures to cf
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {

    // get a map of names of columns (keys)  and a vector of floats (values)
    auto dataMap = ts.getMap();

    // loop that go over the all map items (features) and compare them to the next items
    for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
        float maxP = 0;
        string correlatedIndex;
        auto nextIt = it;
        int size = (int)it->second.size();

        // loop that find the feature that has the max correlation with the current feature
        for (auto secIt = ++nextIt; secIt != dataMap.end(); ++secIt) {

            // check the correlation between the two columns
            float p = pearson(&it->second[0], &secIt->second[0], size);

            // save the max correlation in maxP and saves the name of the second feature at correlatedIndex
            if (abs(p) > maxP) {
                maxP = p;
                correlatedIndex = secIt->first;
            }
        }

        // check if there is a max correlated feature that has more correlation than the threshold value
        if (!correlatedIndex.empty() && maxP > threshold) {
            auto** arrP = new Point*[size];

            // get a line from the 2 vectors of the 2 features and fill arrP with points from the vectors
            Line l = SimpleAnomalyDetector::vectorsToLine(arrP, it->second,
                                                           dataMap.find(correlatedIndex)->second, size);

            // get the max threshold between the line and the points in arrP
            float maxT = SimpleAnomalyDetector::maxThreshold(l, arrP, size);

            // add the correlatedFeatures to cf
            correlatedFeatures correlatedF = {it->first, correlatedIndex, maxP, l, (float(1.1) * maxT)};
            cf.push_back(correlatedF);

            // free the all points and the array of them
            for (int i = 0; i < size; ++i) {
                delete arrP[i];
            }
            delete[] arrP;
        }
    }
}

// detect - gets a TimeSeries and checks if there are anomalies from the cf
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {

    // get a map of names of columns (keys)  and a vector of floats (values)
    auto& dataMap = ts.getMap();
    vector<AnomalyReport> repoVec;

    // loop that go over the rows in the map
    for (long i = 0; i < ts.getRowNumber(); ++i) {

        // get a map of one row of floats values and a string keys
        auto rowI = ts.getMapRow(i);

        // loop that go over the correlatedFeatures and check if there is anomalies
        for (auto it = cf.begin(); it != cf.end(); ++it) {

            // create a point from the 2 correlated features and their float values in that row
            auto point = new Point(rowI->find(it->feature1)->second, rowI->find(it->feature2)->second);

            // check if the dev between the point and the line of these cf is bigger then the threshold.
            if (dev(*point, it->lin_reg) > it->threshold) {

                // add an anomaly report of these features to the vector of the reports
                AnomalyReport anomalyReport(it->feature1 + "-" + it->feature2, i + 1);
                repoVec.push_back(anomalyReport);
            }
            delete point;
        }
        delete rowI;
    }
    return repoVec;
}

// maxThreshold - get a line and points and checks the max dev from the points to the line
float SimpleAnomalyDetector::maxThreshold(const Line& l, Point** points, long size) {
    float max = 0;

    // loop that get the max dev between the points and the line
    for (long j = 0; j < size; j++) {
        float d = dev(*points[j], l);
        if (d > max) {
            max = d;
        }
    }
    return max;
}

// turn 2 vectors of floats to a line, and fill arrP with points from the vectors.
Line SimpleAnomalyDetector::vectorsToLine(Point** arrP, vector<float> vec1, vector<float> vec2, int size) {

    // loop that fill the arrP with points (x from vec1, and y from vec2)
    for (int i = 0; i < size; ++i) {
        arrP[i] = new Point(vec1[i], vec2[i]);
    }

    // get a linear reg from these points and returns it
    Line l = linear_reg(arrP, size);
    return l;
}
