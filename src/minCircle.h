// 311432082

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <random>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------

class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------
// you may add helper functions here

// calculates the distance between two points.
float pDistance(const Point &A, const Point &B);

// return the middle point between two points.
Point getMid(const Point &A, const Point &B);
// checks if a given point is in the given circle.
bool isInCircle(const Point &p, const Circle &c);

// calculate center of circle, which 3 given points are on its boundaries.
// copied from stackoverflow.com, created by Russell Strauss.
Point circleCenter(const Point &A, const Point &B, const Point &C);

// create a circle from 2 given points.
Circle createCircle(const Point &A, const Point &B);

// create a circle from 3 given points.
Circle createCircle(const Point &A, const Point &B, const Point &C);

// the trivial case of the welzel algorithm.
Circle trivial(const std::vector<Point> &R);

// recursively finds the minimum enclosing circle
Circle welzel(std::vector<Point> &P, std::vector<Point> R, size_t size);

// implement
Circle findMinCircle(Point **points, size_t size);


#endif /* MINCIRCLE_H_ */