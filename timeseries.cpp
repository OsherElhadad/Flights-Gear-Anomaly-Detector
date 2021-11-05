/*
 * timeseries.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#include "timeseries.h"
#include<map>

// getter for data structure map
const std::map<string, vector<float>>& TimeSeries::getMap() const{
    return this->map;
}

// getter for the headers names
const vector<string>& TimeSeries::getHeaders() const{
    return this->headers;
}

// getter for the numbers in the received line number
// in case of incorrect row number, throws an exception
const vector<float>* TimeSeries::getRow(int rowNum) const{
    if (rowNum < 0 || rowNum > this->rowsNum) {
        throw "Invalid row number!";
    }

    auto* row = new vector<float>();

    // for every header in the map, add the data which is in the received row number to the vector
    for (const auto & heads : this->map) {
        row->push_back(heads.second.at(rowNum));
    }
    return row;
}

// getter for the received header name's data
// in case of incorrect row number, throws an exception
const vector<float> &TimeSeries::getHeaderColumn(const string& head) const{
    return this->map.at(head);
}


