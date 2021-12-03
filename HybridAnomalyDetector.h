/*
 * HybridAnomalyDetector.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
private:
    float minCorrelation;

public:

    // constructor - get a minCorrelation (or 0.5 by default)
	HybridAnomalyDetector(): minCorrelation(0.5) {};
    explicit HybridAnomalyDetector(float _minCorrelation): minCorrelation(_minCorrelation) {};

    // return if there is an anomaly or not
    virtual bool isAnomaly(const correlatedFeatures* c, Point* &point);

    // add to cf if there is a correlation or don't add
    virtual void addIfCorrelate(float maxPearson, Point** points, size_t size, string f1, string f2);


};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
