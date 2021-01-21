#ifndef ANOMALY_DETECTOR_ANOMALY_DETECTION_UTIL_H
#define ANOMALY_DETECTOR_ANOMALY_DETECTION_UTIL_H

/**
 * returns the variance of X
 *
 * @param x an array of floats
 * @param size size of the array
 * @return the variance of the array
 */

float var(float *x, int size);

/**
 * returns the covariance of X and Y
 *
 * @param x first array of floats
 * @param y second array of floats
 * @param size the size of the two arrays.
 * @return the covariance of both arrays
 */

float cov(float *x, float *y, int size);

/**
 * returns the Pearson correlation coefficient of X and Y
 *
 * @param x first array of floats
 * @param y second array of floats
 * @param size the size of the two arrays
 * @return the Pearson correlation coefficient of both arrays
 */

float pearson(float *x, float *y, int size);

class Line {
public:
    float a, b;

    Line() : a(0), b(0) {}
    Line(float a, float b) : a(a), b(b) {}

    float f(float x) {
        return a * x + b;
    }
};

class Point {
public:
    float x, y;

    Point(float x, float y) : x(x), y(y) {}
};

/**
 *  performs a linear regression and returns the line equation
 *
 * @param points an array of points
 * @param size the size of the array
 * @return linear regression and returns the line equation
 */

Line linear_reg(Point **points, int size);

/**
 * returns the deviation between point p and the line equation of the points
 *
 * @param p given point to check
 * @param points an array of points
 * @param size the size of the array
 * @return the deviation between point p and the line equation of the points
 */

float dev(Point p, Point **points, int size);

/**
 * returns the deviation between point p and the line
 *
 * @param p given point to check
 * @param l linear regression
 * @return the deviation between point p and the line
 */

float dev(Point p, Line l);


/**
 * returns the average of an array X with length SIZE
 *
 * @param x an array of floats
 * @param size the size of the array
 * @return the average of the array
 */

float avg(float *x, int size);

#endif //ANOMALY_DETECTOR_ANOMALY_DETECTION_UTIL_H
