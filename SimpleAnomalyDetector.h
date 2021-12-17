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
#include "minCircle.h"
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

    // the center of min circle of the correlated features
    Point centerCircle;

    // the max dev from the points to the line
    float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector {
public:

    // constructor - get a threshold (or 0.9 by default) and initiate the cf vector
    SimpleAnomalyDetector() : threshold(0.9), cf() {};

    explicit SimpleAnomalyDetector(float _threshold) : threshold(_threshold), cf() {};

    // learnNormal - gets a TimeSeries and adds the correlatedFeatures to cf
    virtual void learnNormal(const TimeSeries &ts);

    // detect - gets a TimeSeries and checks if there are anomalies from the cf
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    // return if there is an anomaly or not
    virtual bool isAnomaly(const correlatedFeatures* c, Point* &point);

    // add to cf if there is a correlation or don't add
    virtual void addIfCorrelate(float maxPearson, Point** points, size_t size, string f1, string f2);

    // getter - returns the correlatedFeatures vector
    vector<correlatedFeatures> getNormalModel() const {
        return cf;
    }

    // getter - return the threshold value
    float getThreshold() const {
        return this->threshold;
    }

    // setter - set the threshold value
    void setThreshold(float _threshold) {
        this->threshold = _threshold;
    }

protected:
    float threshold;
    vector<correlatedFeatures> cf;

    // maxThreshold - get a line and points and checks the max dev from the points to the line
    static float maxThreshold(const Line &l, Point **points, long size);

    // turn array of points to a line
    static Line pointsToLine(Point **arrP, int size);

    // fill arrP with points from the vectors
    static void vectorsToPoints(Point **arrP, vector<float> vec1, vector<float> vec2);

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
