/*
 * timeseries.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;
#include<map>
#include <vector>
#include <fstream>
#include <sstream>

class TimeSeries{
private:
    std::map<string, vector<float>> map;
    vector<string> headers;
    int rowsNum;
public:

    // constructor - get a csv file and copy it's headers and data to a map
    explicit TimeSeries(const char* CSVfileName){
        this->headers = vector<string>();
        std::ifstream file(CSVfileName);
        int lineNum = 0;
        int columnNum = 0;

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
        this->rowsNum = lineNum;
        file.close();
	}

    // getter for data structure map
    const std::map<string, vector<float>>& getMap() const;

    // getter for the headers names
    const vector<string>& getHeaders() const;

    // getter for the numbers in the received line number, using vector
    // in case of incorrect row number, throws an exception
    const vector<float>* getVectorRow(int rowNum) const;

    // getter for the received header name's data
    // in case of incorrect row number, throws an exception
    const vector<float>& getHeaderColumn(const string& head) const;

    // getter for the numbers in the received line number, using map
    // in case of incorrect row number, throws an exception
    const std::map<string,float>* getMapRow(int rowNum) const;
};

#endif /* TIMESERIES_H_ */