// ID1	ID2

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

// ------------ DO NOT CHANGE -----------
class Point {
public:
    float x, y;

    Point(float x, float y) : x(x), y(y) {}
};

class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------
// you may add helper functions here


float pDistance(const Point &A, const Point &B) {
    return std::sqrt(std::pow(A.x - B.x, 2) + std::pow(A.y - B.y, 2));
}

Point getMid(const Point &A, const Point &B) {
    return {(A.x + B.x) / (float) 2.0, (A.y + B.y) / (float) 2.0};
}

bool isInCircle(const Circle &c, const Point &p) {
    return (pDistance(p, c.center) <= c.radius);
}

Point circleCenter(const Point &A, const Point &B, const Point &C) {

    float yDelta_a = B.y - A.y;
    float xDelta_a = B.x - A.x;
    float yDelta_b = C.y - B.y;
    float xDelta_b = C.x - B.x;
    Point center = Point(0, 0);

    float aSlope = yDelta_a / xDelta_a;
    float bSlope = yDelta_b / xDelta_b;
    center.x = (aSlope * bSlope * (A.y - C.y) + bSlope * (A.x + B.x)
                - aSlope * (B.x + C.x)) / (2 * (bSlope - aSlope));
    center.y = -1 * (center.x - (A.x + B.x) / 2) / aSlope + (A.y + B.y) / 2;

    return center;
}

Circle createCircle(const Point &A, const Point &B) {
    Point center = getMid(A, B);
    float radius = pDistance(center, A);
    return {center, radius};
}

Circle createCircle(const Point &A, const Point &B, const Point &C) {
    Point center = circleCenter(A, B, C);
    float radius = pDistance(center, A);
    return {center, radius};
}

Circle trivial(const std::vector<Point> &R) {
    if (R.empty())
        return {{0, 0}, 0};
    else if (R.size() == 1)
        return {R[0], 0};
    else if (R.size() == 2)
        return createCircle(R[0], R[1]);

    return createCircle(R[0], R[1], R[2]);
}

Circle welzel(std::vector<Point> &P, std::vector<Point> R, size_t size) {
    if (size == 0 || R.size() == 3)
        return trivial(R);

    int index = rand() % size;

    Point p = P[index];
    std::swap(P[index], P[size - 1]);
    Circle D = welzel(P, R, size - 1);
    if (isInCircle(D, p))
        return D;

    R.push_back(p);
    return welzel(P, R, size - 1);
}

// implement
Circle findMinCircle(Point **points, size_t size) {
    std::vector<Point> v;
    v.reserve(size);
    for (int i = 0; i < size; i++)
        v.push_back(*points[i]);

    return welzel(v, {}, size);
}


#endif /* MINCIRCLE_H_ */