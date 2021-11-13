/*
 * timeseries.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#include "timeseries.h"

TimeSeries::TimeSeries(const string& CSVfileName) {
    this->headers = vector<string>();
    std::ifstream file(CSVfileName);
    int lineNum = 0, columnNum = 0;

    // maps each header to it's column number. will be used to go through the file more easily
    std::map<int,string> heads = std::map<int,string>();

    // for every line in the file, save the relevant data at the relevant place
    for(std::string line; getline(file,line);)
    {
        std::istringstream ss(line);
        std::string currentStr;

        // break current line by commas and save relevant data
        while(std::getline(ss, currentStr, ',')) {

            // in case it's the first line, save headers names
            if (lineNum == 0) {
                this->map[currentStr] = vector<float>();
                heads[columnNum] = currentStr;
                this->headers.push_back(currentStr);
                columnNum++;
            } else {

                // insert the number to the relevant header's data
                this->map[heads[columnNum]].push_back(std::stof(currentStr));
                columnNum++;
            }
        }
        columnNum = 0;
        lineNum++;
    }
    this->rowsNum = lineNum - 1;
    file.close();
}

// getter for data structure map
const std::map<string, vector<float>>& TimeSeries::getMap() const {
    return this->map;
}

// getter for the headers names
const vector<string>& TimeSeries::getHeaders() const {
    return this->headers;
}

/*
 * getter for the numbers in the received line number, using vector
 * in case of incorrect row number, throws an exception
 */
const vector<float>* TimeSeries::getVectorRow(long rowNum) const {
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

/*
 * getter for the numbers in the received line number, using map
 * in case of incorrect row number, throws an exception
 */
const std::map<string, float>* TimeSeries::getMapRow(long rowNum) const {
    auto* rowMap = new std::map<string, float>();
    const vector<float>* rowVector = this->getVectorRow(rowNum);

    // put every number of the received line under it's header
    for (int i = 0; i < rowVector->size(); ++i) {
        (*rowMap)[this->headers.at(i)] = rowVector->at(i);
    }
    return rowMap;
}

// getter for header's data by the header name
vector<float> TimeSeries::getHeaderDataByName(const string& header) const {
    return this->map.at(header);
}

// getter for header's data by the header column number
vector<float> TimeSeries::getHeaderDataByNum(int num) const {
    return this->map.at(this->headers.at(num));
}

// getter for the number of columns
int TimeSeries::getNumOfColumns() const{
    return this->getHeaders().size();
}




