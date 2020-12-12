
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {

    // iterate on each feature excluding the last one.
    for (auto firstIt = ts.Data.begin(); std::next(firstIt, 1) != ts.Data.end(); firstIt++) {
        float maxPearson = 0;
        std::string secondFeature;

        // iterate on each feature from firstIt to the last one excluding it.
        for (auto secondIt = std::next(firstIt, 1); secondIt != ts.Data.end(); secondIt++) {

            // find firstIt's correlated feature.
            float p = pearson((float *) firstIt->second.data(), (float *) secondIt->second.data(),
                              ts.linesSize());
            p = std::abs(p);
            if (p >= 0.9 && p > maxPearson) {
                maxPearson = p;
                secondFeature = secondIt->first;
            }
        }

        // if a correlation is found.
        if (maxPearson != 0) {

            // create an array of points, consisting of the values that firstIt and secondIt are pointing at.
            int size = ts.linesSize();
            Point *points[size];
            for (int i = 0; i < size; i++) {
                points[i] = new Point(firstIt->second[i], ts.getValue(secondFeature, i));
            }

            // create the line_reg based on the newly built array.
            Line lineReg = linear_reg(points, size);

            // find the max anomaly allowed, aka the threshold.
            float maxDev = 0;
            for (int i = 0; i < size; i++) {
                float curDev = dev(*points[i], lineReg);
                if (curDev > maxDev) {
                    maxDev = curDev;
                }
                delete points[i];
            }
            float threshold = maxDev * (float) 1.1;

            // create the pair of correlated features, with the data we found.
            correlatedFeatures correlatedFeatures = {firstIt->first,
                                                     secondFeature,
                                                     maxPearson,
                                                     lineReg,
                                                     threshold};

            // insert the pair into this.
            cf.push_back(correlatedFeatures);
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    std::vector<AnomalyReport> reports;

    // traverse on the each line in the new given timeseries.
    int lines = ts.linesSize();
    for (int i = 0; i < lines; i++) {

        // traverse each correlated pair in this.
        for (auto &it : cf) {

            // create a point from the value of current correlated pair.
            float x = ts.getValue(it.feature1, i);
            float y = ts.getValue(it.feature2, i);
            Point p = Point(x, y);

            // check if the point is in the allowed threshold.
            if (dev(p, it.lin_reg) > it.threshold) {

                // if not - its anomaly, so create AnomalyReport and add it to the list.
                AnomalyReport r = AnomalyReport(it.feature1 + "-" + it.feature2, i + 1);
                reports.push_back(r);
            }
        }
    }
    return reports;
}

