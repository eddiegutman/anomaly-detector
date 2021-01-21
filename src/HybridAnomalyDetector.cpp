
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
        Circle min = findMinCircle(points, size);
        c.x = min.center.x;
        c.y = min.center.y;
        c.threshold = min.radius * (float) 1.1;
        SimpleAnomalyDetector::cf.push_back(c);
    }
}

bool HybridAnomalyDetector::isAnomalous(Point p, const correlatedFeatures &c) {
    if (c.corrlation >= 0.9)
        return SimpleAnomalyDetector::isAnomalous(p, c);
    else
        return !isInCircle(p,Circle(Point(c.x, c.y), c.threshold));
}

