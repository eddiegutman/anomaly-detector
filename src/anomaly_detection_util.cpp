#include "anomaly_detection_util.h"
#include <cmath>

// returns the average of an array X with length SIZE
float avg(float *x, int size) {

    float sum = 0;
    for (int i = 0; i < size; i++)
        sum += *(x + i);
    return (sum / size);
}

// returns the variance of X
float var(float *x, int size) {
    float sum = 0, average;

    // calculate the total sum power two
    for (int i = 0; i < size; i++)
        sum += x[i] * x[i];

    // calculate the average value
    average = avg(x, size);

    // calculate the variance and return it
    return sum / size - average * average;
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
    float cov, avgSum = 0, uX, uY;

    // calculate the average value of X and Y
    uX = avg(x, size);
    uY = avg(y, size);

    // calculate multiplication of X difference and Y difference
    for (int i = 0; i < size; i++) {
        avgSum += ((x[i] - uX) * (y[i] - uY));
    }

    // calculate the covariance of X and Y
    cov = avgSum / size;
    return cov;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
    float sigmaX, sigmaY, pearson;
    sigmaX = std::sqrt(var(x, size));
    sigmaY = std::sqrt(var(y, size));
    pearson = cov(x, y, size) / (sigmaX * sigmaY);
    return pearson;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size) {
    float a, b;
    float x[size];
    float y[size];
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    a = cov(x, y, size) / var(x, size);
    b = avg(y, size) - (a * avg(x, size));
    return Line(a, b);
}

//returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
    return dev(p, linear_reg(points, size));
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    return std::abs(p.y - l.f(p.x));
}