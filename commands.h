/*
 * commands.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <iomanip>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}

    string readFileData() {
        string data;
        string line = this->read();
        while (line != "done") {
            line.append("\n");
            data.append(line);
            line = this->read();
        }
        return data;
    }
    void CreateFile(const string& fileName, const string& data) {
        std:ofstream outfile {fileName};
        outfile << data;
    }
};

// Standard IO - interaction with user using console printing and scanning
class StandardIO: public DefaultIO {
    string read() override {
        string input;
        cin >> input;
        return input;
    }

    void write(string text) override {
        cout << text;
    }

    void write(float f) override {
        cout << f;
    }

    void read(float* f) {
        cin >> *f;
    }
};

class Info {
    HybridAnomalyDetector hd;
    string CSVFileNameLearn;
    string CSVFileNameDetect;
    vector<AnomalyReport> reportVector;
    long rowNumber;

public:
    void learnNormal() {
        this->hd.learnNormal(TimeSeries(this->CSVFileNameLearn));
    }

    void detect() {
        TimeSeries ts = TimeSeries(this->CSVFileNameDetect);
        this->rowNumber = ts.getRowNumber();
        this->reportVector = this->hd.detect(ts);
    }

    long getRowNumber() const {
        return this->rowNumber;
    }

    const vector<AnomalyReport> getReportVector() const {
        return this->reportVector;
    }

    float getThreshold() const {
        return this->hd.getThreshold();
    }

    void setThreshold(float t) {
        this->hd.setThreshold(t);
    }

    void setCSVfileNameLearn(string fileNameLearn) {
        this->CSVFileNameLearn = fileNameLearn;
    }

    void setCSVfileNameDetect(string fileNameDetect) {
        this->CSVFileNameDetect = fileNameDetect;
    }
};

class Command {
    const string description;
protected:
    DefaultIO* dio;
public:
    Command(DefaultIO* dio, const string& d):dio(dio), description{d}{}
    virtual void execute(Info* info)=0;
    virtual ~Command(){}

    string getDescription() const {
        return this->description;
    }
};

class UploadTimeSeriesCommand: public Command {
public:
    UploadTimeSeriesCommand(DefaultIO* dio):Command(dio, "upload a time series csv file\n"){}
    void execute(Info* info) override {
        this->dio->write("Please upload your local train CSV file.\n");

        // read csv data from client
        string csvData = this->dio->readFileData();

        string fileNameLearn = "anomalyTrain.csv";

        // create csv file
        this->dio->CreateFile(fileNameLearn, csvData);
        info->setCSVfileNameLearn(fileNameLearn);

        this->dio->write("Upload complete.\n");

        this->dio->write("Please upload your local test CSV file.\n");

        // read csv data from client
        csvData = this->dio->readFileData();

        string fileNameDetect = "anomalyTest.csv";

        // create csv file
        this->dio->CreateFile(fileNameDetect, csvData);
        info->setCSVfileNameDetect(fileNameDetect);

        this->dio->write("Upload complete.\n");
    }
};

class ThresholdCommand: public Command {
public:
    ThresholdCommand(DefaultIO* dio):Command(dio, "algorithm settings\n"){}
    void execute(Info* info) override {
        string currentThreshold = std::to_string(info->getThreshold());
        this->dio->write("The current correlation threshold is " + currentThreshold + "\n");
        float input = -1;

        // get new threshold from client until it is a valid one
        while (input < 0 || input > 1) {
            this->dio->write("Type a new threshold\n");
            this->dio->read(&input);
            if (input < 0 || input > 1)
                this->dio->write("please choose a value between 0 and 1.\n");
        }

        // set new threshold
        info->setThreshold(input);
    }
};

class DetectAnomaliesCommand: public Command {
public:
    DetectAnomaliesCommand(DefaultIO* dio):Command(dio, "detect anomalies\n"){}
    void execute(Info* info) override {
        info->learnNormal();
        info->detect();
        this->dio->write("anomaly detection complete.\n");
    }
};

class DisplayAnomaliesCommand: public Command {
public:
    DisplayAnomaliesCommand(DefaultIO* dio):Command(dio, "display results\n"){}
    void execute(Info* info) override {
        vector<AnomalyReport> reportVector = info->getReportVector();
        for (AnomalyReport& ar : reportVector) {
            this->dio->write(to_string(ar.timeStep) + "\t" + ar.description + "\n");
        }
        this->dio->write("Done.\n");
    }
};

class UploadAnomaliesAndAnalyzeCommand: public Command {
public:
    UploadAnomaliesAndAnalyzeCommand(DefaultIO* dio):Command(dio, "upload anomalies and analyze results\n"){}
    void execute(Info* info) override {
        this->dio->write("Please upload your local train CSV file.\n");

        // read csv data from client
        string csvData = this->dio->readFileData();
        this->dio->write("Upload complete.\n");
        string s = "8";
        int x = s.find("*");
        if (x < 0) {
            int y = 0;

        }

        map<string, vector<string>> cfReported = map<string, vector<string>>();
        vector<AnomalyReport> reportVector = info->getReportVector();
        for (AnomalyReport& ar : reportVector) {
            if (cfReported.find(ar.description) == cfReported.end()) {
                cfReported[ar.description] = vector<string>();
                cfReported[ar.description].push_back(to_string(ar.timeStep));
            } else {
                int a = cfReported.find(ar.description)->second.back().find("*");
                if (a < 0) {
                    long start = stol(cfReported[ar.description].back());
                    if (start == ar.timeStep - 1) {
                        cfReported[ar.description].back() = cfReported[ar.description].back() + "*" + to_string(ar.timeStep);
                    } else {
                        cfReported[ar.description].push_back(to_string(ar.timeStep));
                    }
                } else {
                    long endNum = stol(strSplit(cfReported[ar.description].back(), '*')[1]);
                    if (endNum == ar.timeStep - 1) {
                        cfReported[ar.description].back() = strSplit(cfReported[ar.description].back(), '*')[0]
                                + "*" + to_string(ar.timeStep);
                    } else {
                        cfReported[ar.description].push_back(to_string(ar.timeStep));
                    }
                }
            }
        }
        vector<long> starts = vector<long>();
        vector<long> ends = vector<long>();
        vector<string> intervals = strSplit(csvData, '\n');
        long P = intervals.size();
        long N = info->getRowNumber();
        for (string& interval : intervals) {
            vector<string> startEnd = strSplit(interval, ',');
            starts.push_back(stol(startEnd[0]));
            ends.push_back(stol(startEnd[1]));
            N -= (stol(startEnd[1]) - stol(startEnd[0]) + 1);
        }
        long FP = 0, TP = 0;
        for (auto& ar : cfReported) {
            for (string& interval : cfReported.find(ar.first)->second) {
                long start1, end1;
                if (interval.find('*') < 0) {
                    start1 = stol(interval);
                    end1 = start1;
                } else {
                    start1 = stol(strSplit(interval, '*')[0]);
                    end1 = stol(strSplit(interval, '*')[1]);
                }
                bool flag = true;
                for (long i = 0; i < starts.size(); i++) {
                    if (start1 <= ends[i] && end1 >= starts[i]) {
                        TP++;
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    FP++;
                }
            }
        }
        this->dio->write("True Positive Rate: ");
        this->dio->write((int)(1000.0*TP/P)/1000.0f);
        this->dio->write("\n");
        this->dio->write("False Positive Rate: ");
        this->dio->write((int)(1000.0*FP/N)/1000.0f);
        this->dio->write("\n");
    }

    static vector<string> strSplit(string string1, char delim) {
        vector<string> result = vector<string>();
        auto ss = stringstream{string1};

        for (string line; getline(ss, line, delim);)
            result.push_back(line);

        return result;
    }
};

class ExitCommand: public Command {
public:
    ExitCommand(DefaultIO* dio):Command(dio, "exit\n"){}
    void execute(Info* info) override {}
};

#endif /* COMMANDS_H_ */