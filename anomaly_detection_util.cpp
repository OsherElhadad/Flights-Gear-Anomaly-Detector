/*
 * animaly_detection_util.cpp
 *
 * Author: write your ID and name here
 */

#include <math.h>
#include "anomaly_detection_util.h"

//Returns the average of the array's members
float avg(float* x, int size){
    float sum = 0;
    //Add the array's members to the sum
    for (int i = 0; i < size; ++i) {
        sum += x[i];
    }
    return sum/size;
}

// returns the variance of X and Y
float var(float* x, int size){
    float sum = 0, sumPow = 0;
    for (int i = 0; i < size; ++i) {
        sum += x[i];
        sumPow += x[i] * x[i];
    }
	float mui = (1.0/size) * sum;
    float result = (1.0/size) * sumPow - mui * mui;
    return result;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float c[size];
    //Multiply the vectors
    for (int i = 0; i < size;i++) {
        c[i] = x[i] * y[i];
    }
    return avg(c,size) - avg(x,size) * avg(y,size);
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	return cov(x,y,size)/(sqrt(var(x,size)) * sqrt(var(y,size)));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

	return Line(0,0);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	return 0;
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	return 0;
}




