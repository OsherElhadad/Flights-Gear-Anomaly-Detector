/*
 * animaly_detection_util.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

#include <memory>
#include <vector>
#define DOUBLE_POW 2
#define DEFAULT_VALUE 0
#define MIDDLE_DEST 2


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
	Line(): a(DEFAULT_VALUE), b(DEFAULT_VALUE) {}

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

    // getter for point's x
    float getX() const{
        return this->x;
    }

    // setter for point's x
    void setX(float newX){
        this->x = newX;
    }

    // getter for point's y
    float getY() const{
        return this->y;
    }

    // setter for point's y
    void setY(float newY){
        this->y = newY;
    }

    // returns euclidean distance between current point and the received point p
    float distance(const Point& p) const {
        return sqrtf(powf(this->x - p.getX(), DOUBLE_POW) + powf(this->y - p.getY(), DOUBLE_POW));
    }

    // returns the middle points between current points and the received point
    Point middlePoint(const Point& p) const{
        return {(this->x + p.getX()) / MIDDLE_DEST , (this->y + p.getY()) / MIDDLE_DEST};
    }
};

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size);

// returns the deviation between point p and the line
float dev(Point p, Line l);

#endif
