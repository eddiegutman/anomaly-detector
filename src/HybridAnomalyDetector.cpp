
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() = default;

HybridAnomalyDetector::~HybridAnomalyDetector() = default;


void HybridAnomalyDetector::addNormal(string &f1, string &f2, float pearson, Point **points, const TimeSeries &ts) {
    if (pearson >= 0.9) {
        SimpleAnomalyDetector::addNormal(f1, f2, pearson, points, ts);
    }
    if (pearson < 0.9 && pearson > 0.5) {
        int size = ts.attributesSize();
        correlatedFeatures c;
        c.feature1 = f1;
        c.feature2 = f2;
        c.corrlation = pearson;
        c.lin_reg = linear_reg(points, size);
        Circle min = findMinCircle(points, size);
        c.center = &min.center;
        c.threshold = min.radius * 1.1;
        c.isLinear = false;
        SimpleAnomalyDetector::cf.push_back(c);
    }
}

bool HybridAnomalyDetector::isAnomalous(Point p, const correlatedFeatures &c) {
    if (c.isLinear)
        return SimpleAnomalyDetector::isAnomalous(p, c);
    else
        return !isInCircle(p,Circle(*c.center, c.threshold));
}

