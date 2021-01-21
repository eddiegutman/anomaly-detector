#include "minCircle.h"


// calculates the distance between two points.
float pDistance(const Point &A, const Point &B) {
    return (float) std::sqrt(std::pow(A.x - B.x, 2) + std::pow(A.y - B.y, 2));
}

// return the middle point between two points.
Point getMid(const Point &A, const Point &B) {
    return {(A.x + B.x) / (float) 2.0, (A.y + B.y) / (float) 2.0};
}

// checks if a given point is in the given circle.
bool isInCircle(const Point &p, const Circle &c) {
    return (pDistance(p, c.center) <= c.radius);
}

// calculate center of circle, which 3 given points are on its boundaries.
// copied from stackoverflow.com, created by Russell Strauss.
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

// create a circle from 2 given points.
Circle createCircle(const Point &A, const Point &B) {
    Point center = getMid(A, B);
    float radius = pDistance(center, A);
    return {center, radius};
}

// create a circle from 3 given points.
Circle createCircle(const Point &A, const Point &B, const Point &C) {
    Point center = circleCenter(A, B, C);
    float radius = pDistance(center, A);
    return {center, radius};
}

// the trivial case of the welzel algorithm.
Circle trivial(const std::vector<Point> &R) {
    if (R.empty())
        return {{0, 0}, 0};
    else if (R.size() == 1)
        return {R[0], 0};
    else if (R.size() == 2)
        return createCircle(R[0], R[1]);

    return createCircle(R[0], R[1], R[2]);
}

// recursively finds the minimum enclosing circle
Circle welzel(std::vector<Point> &P, std::vector<Point> R, size_t size) {
    if (size == 0 || R.size() == 3)
        return trivial(R);

    int index = rand() % size;

    Point p = P[index];
    std::swap(P[index], P[size - 1]);
    Circle D = welzel(P, R, size - 1);
    if (isInCircle(p, D))
        return D;

    R.push_back(p);
    return welzel(P, R, size - 1);
}

// finds the minimum enclosing circle
Circle findMinCircle(Point **points, size_t size) {
    std::vector<Point> v;
    v.reserve(size);
    for (int i = 0; i < size; i++)
        v.push_back(*points[i]);

    return welzel(v, {}, size);
}