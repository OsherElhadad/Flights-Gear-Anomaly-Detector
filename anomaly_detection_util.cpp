/*
 * animaly_detection_util.cpp
 *
 * Author: Name, ID
 */

#include <math.h>
#include "anomaly_detection_util.h"

// returns the average of the array's members
float avg(float* x, int size) {
    float sum = 0;

    // add the array's members to the sum
    for (int i = 0; i < size; ++i) {
        sum += x[i];
    }
    return sum/size;
}

// returns the variance of X
float var(float* x, int size) {
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
	return cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size)));
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

    // fills x and y arrays from point
    float x[size];
    float y[size];
    fill_x_and_y_arrays(points, x, y, size);

    // a = cov(x,y) / var(x)
    float a = cov(x, y, size) / var(x, size);

    // b =  y - ax
    float b = avg(y, size) - a * avg(x, size);
	return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
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




