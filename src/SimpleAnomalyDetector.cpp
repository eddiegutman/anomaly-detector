
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() { threshold = 0.9; }

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;


// finds the max anomaly allowed, aka the threshold.
float SimpleAnomalyDetector::findThreshold(Point **points, Line lineReg, size_t size) {
    float maxDev = 0;
    for (int i = 0; i < size; i++) {
        float curDev = dev(*points[i], lineReg);
        if (curDev > maxDev) {
            maxDev = curDev;
        }
    }
    return maxDev;
}

Point** SimpleAnomalyDetector::createArray(vector<float> x, vector<float> y) {
    int size = x.size();
    auto **points = new Point*[size];
    for (int i = 0; i < size; i++)
        points[i] = new Point(x[i],y[i]);
    return points;
}

// create the pair of correlated features, with the data we found.
// insert the pair into this.
void SimpleAnomalyDetector::addNormal(std::string &f1, std::string &f2, float pearson, Point **points,
                                      const TimeSeries &ts) {
    if (pearson >= threshold) {
        int size = ts.attributesSize();
        correlatedFeatures c;
        c.feature1 = f1;
        c.feature2 = f2;
        c.corrlation = pearson;
        c.lin_reg = linear_reg(points, size);
        c.threshold = findThreshold(points, c.lin_reg, size) * (float)1.1;
        cf.push_back(c);
    }
}

bool SimpleAnomalyDetector::isAnomalous(Point p, const correlatedFeatures &c){
    return (dev(p, c.lin_reg) > c.threshold);
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    int size = ts.attributesSize();
    std::string firstFeature;
    std::string secondFeature;


    // iterate on each feature excluding the last one.
    for (auto firstIt = ts.Data.begin(); std::next(firstIt, 1) != ts.Data.end(); firstIt++) {
        float maxPearson = 0;
        firstFeature = firstIt->first;

        // iterate on each feature from firstIt to the last one excluding it.
        for (auto secondIt = std::next(firstIt, 1); secondIt != ts.Data.end(); secondIt++) {

            // find firstIt's correlated feature.
            float p = pearson((float *) firstIt->second.data(), (float *) secondIt->second.data(),
                              ts.attributesSize());
            p = std::abs(p);
            if (p > 0.5 && p > maxPearson) {
                maxPearson = p;
                secondFeature = secondIt->first;
            }
        }

        // if a correlation is found.
        if (maxPearson != 0) {

            // create an array of points, consisting of the values that firstIt and secondIt are pointing at.
            Point **points = createArray(firstIt->second, ts.getAttributes(secondFeature));

            // add the correlated pair
            addNormal(firstFeature, secondFeature, maxPearson, points, ts);

            for (int i = 0; i < size; i++)
                delete points[i];
            delete[] points;
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    std::vector<AnomalyReport> reports;

    // traverse on the each line in the new given timeseries.
    int lines = ts.attributesSize();
    for (int i = 0; i < lines; i++) {

        // traverse each correlated pair in this.
        for (auto &it : cf) {

            // create a point from the value of current correlated pair.
            float x = ts.getValue(it.feature1, i);
            float y = ts.getValue(it.feature2, i);
            Point p = Point(x, y);

            // check if the point is anomaly
            if (isAnomalous(p, it)) {
                // if so create AnomalyReport and add it to the list.
                AnomalyReport r = AnomalyReport(it.feature1 + "-" + it.feature2, i + 1);
                reports.push_back(r);
            }
        }
    }
    return reports;
}
