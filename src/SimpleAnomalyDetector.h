

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	vector<correlatedFeatures> cf;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();


	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

protected:
    /**
     * finds the max threshold of given array of points, from given linear regression.
     *
     * @param points an array of points
     * @param lineReg linear regression
     * @param size the size of the points array
     * @return max threshold
     */

    float findThreshold(Point **points, Line lineReg, size_t size);

    /**
     * creates an array of points from two given vectors.
     *
     * @param x given x vector
     * @param y given y vector
     * @return an array of points
     */
    Point **createArray(vector<float> x, vector<float> y);

    /**
     * adds the given pair of correlated features, to the correlated features vector.
     *
     * @param f1 the name of the first feature
     * @param f2 the name of the second feature
     * @param pearson the pearson of the two features
     * @param lineReg linear regression of the two features
     * @param threshold the threshold of the two features
     */

    void addNormal(string &f1, string &f2, float pearson, Line lineReg, float threshold);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
