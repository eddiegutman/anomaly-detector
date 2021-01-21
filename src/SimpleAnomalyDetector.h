

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
	float x;
	float y;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
	float threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();


	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

    void setCorrelationThreshold(float threshold){
        this->threshold=threshold;
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

    static float findThreshold(Point **points, Line lineReg, size_t size);

    /**
     * creates an array of points from two given vectors.
     *
     * @param x given x vector
     * @param y given y vector
     * @return an array of points
     */
    static Point **createArray(vector<float> x, vector<float> y);

    /**
     * adds the given pair of correlated features, to the correlated features vector.
     *
     * @param f1 the name of the first feature
     * @param f2 the name of the second feature
     * @param pearson the pearson of the two features
     * @param points an array of points
     * @param ts this TimeSeries
     */

    virtual void addNormal(string &f1, string &f2, float pearson, Point **points, const TimeSeries &ts);

    /**
     * checks if given point is an anomaly to the corresponding correlated features.
     *
     * @param p given point to check
     * @param c given correlated features
     * @return ture if the point is anomaly and false otherwise
     */

    virtual bool isAnomalous(Point p, const correlatedFeatures &c);

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
