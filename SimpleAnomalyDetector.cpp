#include "SimpleAnomalyDetector.h"

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    auto dataMap = ts.getMap();
    for (map<string, vector<float>>::iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
        float maxP = 0;
        string correlatedIndex = "";
        map<string, vector<float>>::iterator nextIt = it;
        for (map<string, vector<float>>::iterator secIt = ++nextIt; secIt != dataMap.end(); ++secIt) {
            float p = pearson(&it->second[0], &secIt->second[0], it->second.size());
            if (abs(p) > maxP) {
                maxP = p;
                correlatedIndex = secIt->first;
            }
        }
        if (!correlatedIndex.empty() && maxP > threshold) {
            Point ** arrP = new Point*[it->second.size()];
            auto floatVec2 = dataMap.find(correlatedIndex)->second;
            for (int i = 0; i < it->second.size(); ++i) {
                arrP[i] = new Point(it->second[i], floatVec2[i]);
            }
            Line l = linear_reg(arrP, it->second.size());
            float maxT = SimpleAnomalyDetector::maxThreshold(l, arrP, it->second.size());
            struct correlatedFeatures correlatedF = {it->first, correlatedIndex, maxP,
                    linear_reg(arrP, it->second.size()), (float(1.1) * maxT)};
            cf.push_back(correlatedF);

            for (int i = 0; i < it->second.size(); ++i) {
                delete arrP[i];
            }
            delete[] arrP;
        }
    }
}

float SimpleAnomalyDetector::maxThreshold(const Line& l, Point** points, long size) const {
    float max = 0;
    for (long j = 0; j < size; j++) {
        float d = dev(*points[j], l);
        if (d > max) {
            max = d;
        }
    }
    return max;
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    auto dataMap = ts.getMap();
    vector<AnomalyReport> repoVec;
    for (long i = 0; i < dataMap.size() - 1; ++i) {
        auto rowI = ts.getMapRow(i);
        for (vector<correlatedFeatures>::iterator it = cf.begin(); it != cf.end(); ++it) {
            unique_ptr<Point> point = make_unique<Point>(rowI->find(it->feature1)->second, rowI->find(it->feature2)->second);
            if (dev(*point, it->lin_reg) > it->threshold) {
                AnomalyReport anomalyReport(it->feature1 + "-" +it->feature2, i + 1);
                repoVec.push_back(anomalyReport);
            }
        }
        delete rowI;
    }
    return repoVec;
}
