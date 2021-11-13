/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#include "SimpleAnomalyDetector.h"

// learnNormal - gets a TimeSeries and adds the correlatedFeatures to cf
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {

    // loop that go over the all map items (features) and compare them to the next items
    for (int i = 0; i < ts.getNumOfColumns(); i++) {
        vector<float> currentColumn = ts.getHeaderDataByNum(i);
        float maxP = 0;
        string correlatedIndex;
        int size = currentColumn.size();

        // loop that find the feature that has the max correlation with the current feature
        for (int j = i + 1; j  < ts.getNumOfColumns(); j++) {

            // check the correlation between the two columns
            float p = pearson(&currentColumn[0], &ts.getHeaderDataByNum(j)[0], size);

            // save the max correlation in maxP and saves the name of the second feature at correlatedIndex
            if (abs(p) > maxP) {
                maxP = p;
                correlatedIndex = ts.getHeaders().at(j);
            }
        }

        // check if there is a max correlated feature that has more correlation than the threshold value
        if (!correlatedIndex.empty() && maxP > threshold) {
            auto** arrP = new Point*[size];

            // get a line from the 2 vectors of the 2 features and fill arrP with points from the vectors
            Line l = SimpleAnomalyDetector::vectorsToLine(arrP, currentColumn,
                                                          ts.getHeaderDataByName(correlatedIndex), size);

            // get the max threshold between the line and the points in arrP
            float maxT = SimpleAnomalyDetector::maxThreshold(l, arrP, size);

            // add the correlatedFeatures to cf
            correlatedFeatures correlatedF = {ts.getHeaders().at(i), correlatedIndex, maxP, l,
                                              (float(1.2) * maxT)};
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
    vector<AnomalyReport> repoVec;

    // loop that go over the rows in the map
    for (long i = 0; i < ts.getRowNumber(); ++i) {

        // get a map of one row of floats values and a string keys
        auto rowI = ts.getMapRow(i);

        // loop that go over the correlatedFeatures and check if there is anomalies
        for (int j = 0; j < cf.size(); ++j) {

            // create a point from the 2 correlated features and their float values in that row
            auto point = new Point(rowI->find(cf[j].feature1)->second, rowI->find(cf[j].feature2)->second);

            // check if the dev between the point and the line of these cf is bigger then the threshold.
            if (dev(*point, cf[j].lin_reg) > cf[j].threshold) {

                // add an anomaly report of these features to the vector of the reports
                AnomalyReport anomalyReport(cf[j].feature1 + "-" + cf[j].feature2, i + 1);
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
