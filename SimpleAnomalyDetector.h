/*
 * SimpleAnomalyDetector.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <map>
#include <memory>

struct correlatedFeatures {

    // names of the correlated features
    string feature1,feature2;

    // |pearson(f1,f2)|
    float corrlation;

    // the line we get from the points of f1, f2
    Line lin_reg;

    // the max dev from the points to the line
    float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector {

private:
    vector<correlatedFeatures> cf;
    float threshold;

public:

    // constructor - get a threshold (or 0.9 by default) and initiate the cf vector
    SimpleAnomalyDetector() : threshold(0.9), cf() {};

    explicit SimpleAnomalyDetector(float _threshold) : threshold(_threshold), cf() {};

    // learnNormal - gets a TimeSeries and adds the correlatedFeatures to cf
    virtual void learnNormal(const TimeSeries &ts);

    // detect - gets a TimeSeries and checks if there are anomalies from the cf
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    // getter - returns the correlatedFeatures vector
    vector<correlatedFeatures> getNormalModel() const {
        return cf;
    }

    // setter - set the threshold value
    void setThreshold(float _threshold) {
        threshold = _threshold;
    }

protected:

    // maxThreshold - get a line and points and checks the max dev from the points to the line
    static float maxThreshold(const Line &l, Point **points, long size);

    // turn 2 vectors of floats to a line, and fill arrP with points from the vectors.
    static Line vectorsToLine(Point **arrP, vector<float> vec1, vector<float> vec2, int size);

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
