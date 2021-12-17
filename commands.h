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
        cout << text << endl;
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

public:
    void learnNormal() {
        this->hd.learnNormal(TimeSeries(this->CSVFileNameLearn));
    }

    void detect() {
        this->reportVector = this->hd.detect(TimeSeries(this->CSVFileNameLearn));
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

    string getCSVfileNameLearn() const {
        return this->CSVFileNameLearn;
    }

    void setCSVfileNameLearn(string fileNameLearn) {
        this->CSVFileNameLearn = fileNameLearn;
    }

    string getCSVfileNameDetect() const {
        return this->CSVFileNameDetect;
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
    UploadTimeSeriesCommand(DefaultIO* dio):Command(dio, "upload a time series csv file"){}
    void execute(Info* info) override {
        this->dio->write("Please upload your local train CSV file.");

        // read csv data from client
        string csvData = this->dio->readFileData();

        string fileNameLearn = "anomalyTrain.csv";

        // create csv file
        this->dio->CreateFile(fileNameLearn, csvData);
        info->setCSVfileNameLearn(fileNameLearn);

        this->dio->write("Upload complete.");

        this->dio->write("Please upload your local test CSV file.");

        // read csv data from client
        csvData = this->dio->readFileData();

        string fileNameDetect = "anomalyTest.csv";

        // create csv file
        this->dio->CreateFile(fileNameDetect, csvData);
        info->setCSVfileNameDetect(fileNameDetect);

        this->dio->write("Upload complete.");
    }
};

class ThresholdCommand: public Command {
public:
    ThresholdCommand(DefaultIO* dio):Command(dio, "algorithm settings"){}
    void execute(Info* info) override {
        string currentThreshold = std::to_string(info->getThreshold());
        this->dio->write("The current correlation threshold is " + currentThreshold);
        float input = -1;

        // get new threshold from client until it is a valid one
        while (input < 0 || input > 1) {
            this->dio->read(&input);
            if (input < 0 || input > 1)
                this->dio->write("please choose a value between 0 and 1.");
        }

        // set new threshold
        info->setThreshold(input);
    }
};

class DetectAnomaliesCommand: public Command {
public:
    DetectAnomaliesCommand(DefaultIO* dio):Command(dio, "detect anomalies"){}
    void execute(Info* info) override {
        info->learnNormal();
        info->detect();
        this->dio->write("anomaly detection complete.");
    }
};

class DisplayAnomaliesCommand: public Command {
public:
    DisplayAnomaliesCommand(DefaultIO* dio):Command(dio, "display results"){}
    void execute(Info* info) override {
        vector<AnomalyReport> reportVector = info->getReportVector();
        for (AnomalyReport& ar : reportVector) {
            this->dio->write(to_string(ar.timeStep) + "\t" + ar.description);
        }
        this->dio->write("Done.");
    }
};

class UploadAnomaliesAndAnalyzeCommand: public Command {
public:
    UploadAnomaliesAndAnalyzeCommand(DefaultIO* dio):Command(dio, "upload anomalies and analyze results"){}
    void execute(Info* info) override {
        this->dio->write("Please upload your local train CSV file.");

        // read csv data from client
        string csvData = this->dio->readFileData();
        this->dio->write("Upload complete.");

        map<string, vector<string>> cfReported = map<string, vector<string>>();
        vector<AnomalyReport> reportVector = info->getReportVector();
        for (AnomalyReport& ar : reportVector) {
            if (cfReported.find(ar.description) != cfReported.end()) {
                cfReported[ar.description] = vector<string>();
                cfReported[ar.description].push_back(to_string(ar.timeStep));
            } else {
                if (cfReported[ar.description].back().find('*') < 0) {
                    long start = stol(cfReported[ar.description].back());
                    if (start == ar.timeStep - 1) {
                        cfReported[ar.description].back() = cfReported[ar.description].back() + "*" + to_string(start);
                    } else {
                        cfReported[ar.description].push_back(to_string(ar.timeStep));
                    }
                } else {
                    long endNum = stol(strSplit(cfReported[ar.description].back(), '*')[1]);
                    if (endNum == ar.timeStep - 1) {
                        cfReported[ar.description].back() = strSplit(cfReported[ar.description].back(), '*')[0]
                                + "*" + to_string(endNum);
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
        long N = P;
        for (string& interval : intervals) {
            vector<string> startEnd = strSplit(interval, ',');
            starts.push_back(stol(startEnd[0]));
            ends.push_back(stol(startEnd[1]));
            N -= (stol(startEnd[1]) - stol(startEnd[0]) + 1);
        }
        long FP, TP;
        for (AnomalyReport& ar : reportVector) {
            for (string& interval : cfReported[ar.description]) {
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
                if (!flag) {
                    FP++;
                }
            }
        }
        std::ostringstream strStream;
        strStream << fixed << setprecision(3)  << ((float) TP / (float) P);
        this->dio->write("True Positive Rate: " + strStream.str());
        strStream << fixed << setprecision(3)  << ((float) FP / (float) N);
        this->dio->write("False Positive Rate: " + strStream.str());
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
    ExitCommand(DefaultIO* dio):Command(dio, "exit"){}
    void execute(Info* info) override {}
};

#endif /* COMMANDS_H_ */