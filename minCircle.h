/*
 * minCircle.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "anomaly_detection_util.h"
#include <cassert>
#include <algorithm>

using namespace std;

// Circle has center point and radius
class Circle{
public:
	Point center;
	float radius;

    // constructor
	Circle(Point c,float r):center(c),radius(r){}

    // getter for center of circle
    const Point& getCenter() const{
        return this->center;
    }

    // getter for center of circle
    float getRadius() const{
        return this->radius;
    }

    // returns true if the received point is inside the circle and false otherwise
    bool is_point_inside(const Point& p) const {
        return p.distance(this->center) <= this->radius;
    }
};

// returns the minimal enclosing circle for the received points
Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */
