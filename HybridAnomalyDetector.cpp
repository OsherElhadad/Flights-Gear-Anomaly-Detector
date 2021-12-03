/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */
#include "HybridAnomalyDetector.h"

// return if there is an anomaly or not
bool HybridAnomalyDetector::isAnomaly(const correlatedFeatures* c, Point* &point) {
    return (c->corrlation > this->threshold && SimpleAnomalyDetector::isAnomaly(c, point))
    || (c->corrlation < this->threshold && c->threshold > this->minCorrelation
        && point->distance(c->centerCircle) > c->threshold);
}

// add to cf if there is a correlation or don't add
void HybridAnomalyDetector::addIfCorrelate(float maxPearson, Point** points, size_t size, string f1, string f2) {
    SimpleAnomalyDetector::addIfCorrelate(maxPearson, points, size, f1, f2);
    if (maxPearson > this->minCorrelation && maxPearson <= this->threshold) {
        Circle circle = findMinCircle(points, size);

        // add the correlatedFeatures to cf
        correlatedFeatures correlatedF = {f1, f2, maxPearson, Line(0, 0), circle.getCenter(),
                                          (float(1.2) * circle.getRadius())};
        cf.push_back(correlatedF);
    }
}

