/*
 * animaly_detection_util.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#include <math.h>
#include "anomaly_detection_util.h"
#define EMPTY_ARRAY 0
#define EPSILON_POW -10
#define EPSILON_BASE 10

// returns the average of the array's members
float avg(float* x, int size) {
    // checks if the array size is a positive number
    if(size <= EMPTY_ARRAY) {
        throw "Size must be positive!";
    }

    float sum = 0;

    // add the array's members to the sum
    for (int i = 0; i < size; ++i) {
        sum += x[i];
    }
    return sum/size;
}

// returns the variance of X
float var(float* x, int size) {
    // checks if the array size is a positive number
    if(size <= EMPTY_ARRAY) {
        throw "Size must be positive!";
    }

    float sumPow = 0;

    // sums the array's member's pows
    for (int i = 0; i < size; ++i) {
        sumPow += x[i] * x[i];
    }
	float mui = avg(x, size);

    // variance formula
    float result = sumPow / (float)size - mui * mui;
    return result;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    // checks if the array size is a positive number
    if(size <= EMPTY_ARRAY) {
        throw "Size must be positive!";
    }

    float c[size];

    // multiply the vectors
    for (int i = 0; i < size; i++) {
        c[i] = x[i] * y[i];
    }

    // covariance formula
    return avg(c, size) - avg(x, size) * avg(y, size);
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {

    // pearson formula
	return cov(x, y, size) / (sqrtf(var(x, size)) * sqrt(var(y, size)));
}

// fills the x and y arrays with points array
void fill_x_and_y_arrays(Point** points, float* x, float* y, int size) {
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    float epsilon = powf(EPSILON_BASE, EPSILON_POW);
    // checks if the array size is a positive number
    if(size <= EMPTY_ARRAY) {
        throw "Size must be positive!";
    }

    // fills x and y arrays from point
    float x[size];
    float y[size];
    fill_x_and_y_arrays(points, x, y, size);

    float xVar = var(x, size), a;

    // in case of vertical line
    if(xVar <= epsilon) {
        a = INFINITY;
    } else {
        a = cov(x, y, size) / xVar;
    }

    // b =  y - ax
    float b = avg(y, size) - a * avg(x, size);
	return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
    // checks if the array size is a positive number
    if(size <= EMPTY_ARRAY) {
        throw "Size must be positive!";
    }
	return dev(p, linear_reg(points, size));
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    float yOfLine = l.f(p.x);
    float yOfPoint = p.y;

    // it is the |f(x) - y| (f is the line, and y is the y of point)
    if(yOfLine > yOfPoint) {
        return yOfLine - yOfPoint;
    } else {
        return yOfPoint - yOfLine;
    }
}
