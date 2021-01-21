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

class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};

/**
 * calculates the distance between two points.
 *
 * @param A first point
 * @param B second point
 * @return the distance between the two points
 */

float pDistance(const Point &A, const Point &B);

/**
 * returns the middle point between two points.
 *
 * @param A first point
 * @param B second point
 * @return the middle point between two points
 */

Point getMid(const Point &A, const Point &B);

/**
 * checks if a given point is in the given circle.
 *
 * @param p given point
 * @param c given circle
 * @return true if the point is in the circle and false otherwise
 */

bool isInCircle(const Point &p, const Circle &c);

/**
 * calculate center of circle, which 3 given points are on its boundaries.
 * copied from stackoverflow.com, created by Russell Strauss.
 *
 * @param A first point
 * @param B second point
 * @param C third point
 * @return circle center point
 */

Point circleCenter(const Point &A, const Point &B, const Point &C);

/**
 * create a circle from 2 given points.
 *
 * @param A first point
 * @param B second point
 * @return circle
 */

Circle createCircle(const Point &A, const Point &B);

/**
 * create a circle from 3 given points.
 *
 * @param A first point
 * @param B second point
 * @param C third point
 * @return circle
 */

Circle createCircle(const Point &A, const Point &B, const Point &C);

/**
 * the trivial case of the welzel algorithm.
 *
 * @param R a vector on points
 * @return circle
 */

Circle trivial(const std::vector<Point> &R);

/**
 * recursively finds the minimum enclosing circle
 *
 * @param P vector of points
 * @param R helping vector
 * @param size the size of the first vector
 * @return minimum enclosing circle
 */

Circle welzel(std::vector<Point> &P, std::vector<Point> R, size_t size);

/**
 * finds the minimum enclosing circle of given array of points.
 *
 * @param points an array of points
 * @param size the size of the array
 * @return minimum enclosing circle
 */

Circle findMinCircle(Point **points, size_t size);


#endif /* MINCIRCLE_H_ */