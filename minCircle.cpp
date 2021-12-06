/*
 * minCircle.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */
#include "minCircle.h"
#define MIDDLE_DEST 2
#define ZERO_CIRCLE 0
#define MAX_POINTS 3
#define EMPTY 0
#define DOUBLE 2
#define FIRST_POINT 0
#define SECOND_POINT 1
#define THIRD_POINT 2

// return the smallest circle that intersects 2 points
Circle createCircle(const Point& a, const Point& b) {

    // set the center to be the midpoint of a and b
    Point C = a.middlePoint(b);

    // set the radius to be half the distance from a to b and return circle
    return { C, a.distance(b) / MIDDLE_DEST };
}

// return the minimum enclosing circle for 3 or fewer points
Circle min_circle_trivial(vector<Point>& P) {

    // in case there are no points, create a "zero" circle
    if (P.empty())
        return { { ZERO_CIRCLE, ZERO_CIRCLE }, ZERO_CIRCLE };

        // in case there is a single point, create a "dot circle"
    else if (P.size() == 1)
        return { P[FIRST_POINT], ZERO_CIRCLE };

        // in case there are two points, create a minimal circle
    else if (P.size() == 2)
        return createCircle(P[FIRST_POINT], P[SECOND_POINT]);

    Circle c = createCircle(P[FIRST_POINT], P[SECOND_POINT]);
    if (c.is_point_inside(P[THIRD_POINT]))
        return c;

    c = createCircle(P[FIRST_POINT], P[THIRD_POINT]);
    if (c.is_point_inside(P[SECOND_POINT]))
        return c;

    c = createCircle(P[SECOND_POINT], P[THIRD_POINT]);
    if (c.is_point_inside(P[FIRST_POINT]))
        return c;

    float ax = P[FIRST_POINT].getX();
    float ay = P[FIRST_POINT].getY();
    float bx = P[SECOND_POINT].getX();
    float by = P[SECOND_POINT].getY();
    float cx = P[THIRD_POINT].getX();
    float cy = P[THIRD_POINT].getY();

    float k = bx - ax;
    float t = by - ay;
    float s = cx - ax;
    float v = cy - ay;

    float x = k * k + t * t;
    float y = s * s + v * v;
    float z = k * v - t * s;
    float div = DOUBLE * z;
    auto* center = new Point((v * x - t * y) / div, (k * y - s * x) / div);

    // set final center's x axis
    center->setX(center->getX() + P[FIRST_POINT].getX());

    // set final center's y axis
    center->setY(center->getY() + P[FIRST_POINT].getY());

    // return the wanted circle
    return { *center, center->distance(P[FIRST_POINT])};
}

// returns the minimal enclosing circle using welzl algorithm
Circle mecWelzl(Point**& points, vector<Point> leftOut, int n) {

    // base case when all points processed or |leftOut| = 3
    if (n == EMPTY || leftOut.size() == MAX_POINTS)
        return min_circle_trivial(leftOut);

    // Pick a random point
    int idx = rand() % n;
    Point p = *points[idx];

    // put the picked point at the end of points
    swap(points[idx], points[n - 1]);

    // get the minimal enclosing circle without picked point
    Circle current = mecWelzl(points, leftOut, n - 1);

    // in case current contains p, return current
    if (current.is_point_inside(p))
        return current;

    // else, must be on the boundary of the minimal enclosing circle
    leftOut.push_back(p);
    // return the minimal enclosing circle for points - {p} and leftOut U {p}
    return mecWelzl(points, leftOut, n - 1);
}

// returns the minimal enclosing circle for the received points
Circle findMinCircle(Point** points, size_t size) {
    return mecWelzl(points, {}, size);
}