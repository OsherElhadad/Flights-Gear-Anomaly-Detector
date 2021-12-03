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
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"
#include <assert.h>
#include <algorithm>

using namespace std;

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}

    bool is_point_inside(const Point& p) const {
        return p.distance(this->center) <= this->radius;
    }
};

Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */
