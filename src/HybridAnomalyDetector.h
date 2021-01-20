

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

    void addNormal(string &f1, string &f2, float pearson, Point **points, const TimeSeries &ts) override;
    bool isAnomalous(Point p, const correlatedFeatures &c) override;

    };

#endif /* HYBRIDANOMALYDETECTOR_H_ */
