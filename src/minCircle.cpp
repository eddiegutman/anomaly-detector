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

/**
float dist(Point a, Point b){
    float x2=(a.x-b.x)*(a.x-b.x);
    float y2=(a.y-b.y)*(a.y-b.y);
    return sqrt(x2+y2);
}

Circle from2points(Point a,Point b){
    float x=(a.x+b.x)/2;
    float y=(a.y+b.y)/2;
    float r=dist(a,b)/2;
    return Circle(Point(x,y),r);
}



Circle from3Points(Point a, Point b, Point c){
    // find the circumcenter of the triangle a,b,c
    // find the circumcenter of the triangle a,b,c

    Point mAB((a.x+b.x)/2 , (a.y+b.y)/2); // mid point of line AB
    float slopAB = (b.y - a.y) / (b.x - a.x); // the slop of AB
    float pSlopAB = - 1/slopAB; // the perpendicular slop of AB
    // pSlop equation is:
    // y - mAB.y = pSlopAB * (x - mAB.x) ==> y = pSlopAB * (x - mAB.x) + mAB.y

    Point mBC((b.x+c.x)/2 , (b.y+c.y)/2); // mid point of line BC
    float slopBC = (c.y - b.y) / (c.x - b.x); // the slop of BC
    float pSlopBC = - 1/slopBC; // the perpendicular slop of BC
    // pSlop equation is:
    // y - mBC.y = pSlopBC * (x - mBC.x) ==> y = pSlopBC * (x - mBC.x) + mBC.y

    float x = (- pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y) / (pSlopAB - pSlopBC);
    float y = pSlopAB * (x - mAB.x) + mAB.y;
    Point center(x,y);
    float R=dist(center,a);

    return Circle(center,R);
}

Circle trivial(vector<Point>& P){
    if(P.size()==0)
        return Circle(Point(0,0),0);
    else if(P.size()==1)
        return Circle(P[0],0);
    else if (P.size()==2)
        return from2points(P[0],P[1]);

    // maybe 2 of the points define a small circle that contains the 3ed point
    Circle c=from2points(P[0],P[1]);
    if(dist(P[2],c.center)<=c.radius)
        return c;
    c=from2points(P[0],P[2]);
    if(dist(P[1],c.center)<=c.radius)
        return c;
    c=from2points(P[1],P[2]);
    if(dist(P[0],c.center)<=c.radius)
        return c;
    // else find the unique circle from 3 points
    return from3Points(P[0],P[1],P[2]);
}


Circle welzl(Point** P,vector<Point> R, size_t n){
    if(n==0 || R.size()==3){
        return trivial(R);
    }

    // remove random point p
    // swap is more efficient than remove
    //srand (time(NULL));
    int i=rand()%n;
    Point p(P[i]->x,P[i]->y);
    swap(P[i],P[n-1]);

    Circle c=welzl(P,R,n-1);

    if(dist(p,c.center)<=c.radius)
        return c;

    R.push_back(p);

    return welzl(P,R,n-1);
}

Circle findMinCircle(Point** points,size_t size){
    return welzl(points,{},size);
}
*/