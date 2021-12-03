/*
 * minCircle.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */
#include "minCircle.h"
#define MIDDLE_DEST 2

// returns center of circle based on three points
Point get_circle_center(float bx, float by, float cx, float cy) {
    float b = bx * bx + by * by;
    float c = cx * cx + cy * cy;
    float d = bx * cy - by * cx;
    return {(cy * b - by * c) / (2 * d),(bx * c - cx * b) / (2 * d) };
}

// return circle that intersect with the received points
Circle createCircle(const Point& a, const Point& b, const Point& c) {
    Point center = get_circle_center(b.getX() - a.getX(), b.getY() - a.getY(),
                                     c.getX() - a.getX(),c.getY() - a.getY());

    // set final center's x axis
    center.setX(center.getX() + a.getX());

    // set final center's y axis
    center.setY(center.getY() + a.getY());

    // return the wanted circle
    return { center, center.distance(a)};
}

// return the smallest circle that intersects 2 points
Circle createCircle(const Point& a, const Point& b) {

    // set the center to be the midpoint of a and b
    Point C = a.middlePoint(b);                             // !!!!!

    // set the radius to be half the distance from a to b and return circle
    return { C, a.distance(b) / MIDDLE_DEST };
}

// return true if received circle encloses the received points and false otherwise
bool is_valid_circle(const Circle& c, const vector<Point>& P) {

    // in case one of the points is outside the circle, return false
    for (const Point& p : P)
        if (c.is_point_inside(p))
            return false;
    return true;
}

// return the minimum enclosing circle for 3 or fewer points
Circle min_circle_trivial(vector<Point>& P) {
    assert(P.size() <= 3);

    // in case there are no points, create a "zero" circle
    if (P.empty())
        return { { 0, 0 }, 0 };

    // in case there is a single point, create a "dot circle"
    else if (P.size() == 1)
        return { P[0], 0 };

    // in case there are two points, create a minimal circle
    else if (P.size() == 2)
        return createCircle(P[0], P[1]);

    /*
     * in case of 3 points, check if it is possible to create minimal circle using two points
     * if yes, create and return it
     */
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = createCircle(P[i], P[j]);
            if (is_valid_circle(c, P))
                return c;
        }
    }
    return createCircle(P[0], P[1], P[2]);
}

// returns the minimal enclosing circle using welzl algorithm
Circle MEC_welzl(vector<Point>& points, vector<Point> leftOut, int n) {

    // base case when all points processed or |leftOut| = 3
    if (n == 0 || leftOut.size() == 3)
        return min_circle_trivial(leftOut);

    // Pick a random point
    int idx = rand() % n;
    Point p = points[idx];

    // put the picked point at the end of points
    swap(points[idx], points[n - 1]);

    // get the minimal enclosing circle without picked point
    Circle current = MEC_welzl(points, leftOut, n - 1);

    // in case current contains p, return current
    if (current.is_point_inside(p))
        return current;

    // else, must be on the boundary of the minimal enclosing circle
    leftOut.push_back(p);
    // return the minimal enclosing circle for points - {p} and leftOut U {p}
    return MEC_welzl(points, leftOut, n - 1);
}

// returns the minimal enclosing circle for the received points
Circle findMinCircle(Point** points, size_t size) {
    std::vector<Point> points_vector(points, points + size);
    random_shuffle(points_vector.begin(), points_vector.end());
    return MEC_welzl(points_vector, {}, points_vector.size());
}