/*
 * animaly_detection_util.cpp
 *
 * Author: write your ID and name here
 */

#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

// returns the average of the array's members
float avg(float* x, int size);

// returns the variance of X
float var(float* x, int size);

// returns the covariance of X and Y
float cov(float* x, float* y, int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size);

// line has a - slope and b - constant
class Line {
public:
	float a,b;

    // default constructor
	Line(): a(0), b(0) {}

    // constructor
    Line(float a, float b): a(a), b(b) {}

    // evaluate the y value at the given x
	float f(float x) {
		return a * x + b;
	}
};

// point has x and y
class Point {
public:
	float x, y;

    // constructor
	Point(float x, float y): x(x), y(y) {}

    // x axis getter
    float getX() {
        return x;
    }

    // y axis getter
    float getY() {
        return y;
    }
};

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size);

// returns the deviation between point p and the line
float dev(Point p, Line l);

#endif
