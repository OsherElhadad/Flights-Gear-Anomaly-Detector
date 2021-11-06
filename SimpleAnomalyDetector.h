#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <map>
#include <memory>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;
    float threshold;
public:
    SimpleAnomalyDetector() : threshold(0.9) {};
    SimpleAnomalyDetector(float _threshold) : threshold(_threshold) {};

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }

    void setThreshold(float _threshold){
        threshold = _threshold;
    }

protected:
    float maxThreshold(const Line& l, Point** points, long size) const;

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
