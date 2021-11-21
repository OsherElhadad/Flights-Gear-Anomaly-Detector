/*
 * timeseries.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

class TimeSeries {
private:
    std::map<std::string, vector<float>> map;
    vector<string> headers;
    long rowsNum;
public:

    // constructor - get a csv file and copy it's headers and data to a map
    explicit TimeSeries(const string& CSVfileName);

    // getter for data structure map
    const std::map<string, vector<float>>& getMap() const;

    // getter for the headers names
    const vector<string>& getHeaders() const;

    /*
    * getter for the numbers in the received line number, using vector
    * in case of incorrect row number, throws an exception
    */
    const vector<float>* getVectorRow(long rowNum) const;

    /*
    * getter for the numbers in the received line number, using map
    * in case of incorrect row number, throws an exception
    */
    const std::map<string,float>* getMapRow(long rowNum) const;

    // getter for the number of rows in the table
    long getRowNumber() const {
        return this->rowsNum;
    }

    // getter for header's data by the header name
    vector<float> getHeaderDataByName(const string& header) const;

    // getter for header's data by the header column number
    vector<float> getHeaderDataByNum(int num) const;

    // getter for the number of columns
    int getNumOfColumns() const;
};

#endif /* TIMESERIES_H_ */
