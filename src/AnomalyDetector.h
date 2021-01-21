

#ifndef ANOMALYDETECTOR_H_
#define ANOMALYDETECTOR_H_

#include <string>
#include <vector>
#include "timeseries.h"
#include "math.h"
using namespace std;


class AnomalyReport{
public:
	const string description;
	const long timeStep;
	AnomalyReport(string description, long timeStep):description(description),timeStep(timeStep){}
};

class TimeSeriesAnomalyDetector {
public:

    /**
     * learns the normal behaviour of given TimeSeries
     *
     * @param ts given TimeSeries
     */
	virtual void learnNormal(const TimeSeries& ts)=0;

	/**
	 * creates a vector of anomaly reports based on given TimeSeries
	 * compared to the normal behaviour.
	 *
	 * @param ts given TimeSeries
	 * @return vector of anomaly reports.
	 */

	virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
	virtual ~TimeSeriesAnomalyDetector(){}
};

#endif /* ANOMALYDETECTOR_H_ */
