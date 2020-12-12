
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    for (auto firstIt = ts.Data.begin(); std::next(firstIt, 1) != ts.Data.end(); firstIt++) {
        float maxPearson = 0;
        std::string secondFeature;
        for (auto secondIt = std::next(firstIt, 1); secondIt != ts.Data.end(); secondIt++) {
            float p = pearson((float *) firstIt->second.data(), (float *) secondIt->second.data(),
                              ts.linesSize());
            p = std::abs(p);
            if (p >= 0.9 && p > maxPearson) {
                maxPearson = p;
                secondFeature = secondIt->first;
            }
        }
        if (maxPearson != 0) {
            int size = ts.linesSize();
            Point *points[size];
            for (int i = 0; i < size; i++) {
                points[i] = new Point(firstIt->second[i], ts.getValue(secondFeature, i));
            }

            Line lineReg = linear_reg(points, size);
            float maxDev = 0;
            for (int i = 0; i < size; i++) {
                float curDev = dev(*points[i], lineReg);
                if (curDev > maxDev) {
                    maxDev = curDev;
                }
                delete points[i];
            }
            float threshold = maxDev * (float) 1.1;

            correlatedFeatures correlatedFeatures = {firstIt->first,
                                                     secondFeature,
                                                     maxPearson,
                                                     lineReg,
                                                     threshold};

            cf.push_back(correlatedFeatures);
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    std::vector<AnomalyReport> reports;
    int lines = ts.linesSize();
    for (int i = 0; i < lines; i++) {
        for (auto &it : cf) {
            float x = ts.getValue(it.feature1, i);
            float y = ts.getValue(it.feature2, i);
            Point p = Point(x, y);
            if (dev(p, it.lin_reg) > it.threshold) {
                AnomalyReport r = AnomalyReport(it.feature1 + "-" + it.feature2, i + 1);
                reports.push_back(r);
            }
        }
    }
    return reports;
}

