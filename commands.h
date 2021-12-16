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
    float Threshold;

public:
    float getThreshold() const {
        return this->Threshold;
    }

    void setThreshold(float t) {
        this->Threshold = t;
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
        this->dio->write("Please upload your local train CSV file.\n");

        // read csv data from client
        string csvData = this->dio->readFileData();

        // create csv file
        this->dio->CreateFile("anomalyTrain.csv", csvData);

        this->dio->write("Please upload your local test CSV file.\n");

        // read csv data from client
        csvData = this->dio->readFileData();

        // create csv file
        this->dio->CreateFile("anomalyTest.csv", csvData);
    }
};

class ThresholdCommand: public Command {
public:
    ThresholdCommand(DefaultIO* dio):Command(dio, "algorithm settings"){}
    void execute(Info* info) override {
        string currentThreshold = std::to_string(info->getThreshold());
        cout << "The current correlation threshold is " + currentThreshold + "\n";
        float input = -1;

        // get new threshold from client until it is a valid one
        while (input < 0 || input > 1) {
            this->dio->read(&input);
            if (input < 0 || input > 1)
                cout << "please choose a value between 0 and 1.\n";
        }

        // set new threshold
        info->setThreshold(input);
    }
};

class DetectAnomaliesCommand: public Command {
public:
    DetectAnomaliesCommand(DefaultIO* dio):Command(dio, "detect anomalies"){}
    void execute(Info* info) override {
        // TODO
    }
};

class DisplayAnomaliesCommand: public Command {
public:
    DisplayAnomaliesCommand(DefaultIO* dio):Command(dio, "display results"){}
    void execute(Info* info) override {
        // TODO
    }
};

class UploadAnomaliesAndAnalyzeCommand: public Command {
public:
    UploadAnomaliesAndAnalyzeCommand(DefaultIO* dio):Command(dio, "upload anomalies and analyze results"){}
    void execute(Info* info) override {
        // TODO
    }
};

class ExitCommand: public Command {
public:
    ExitCommand(DefaultIO* dio):Command(dio, "exit"){}
    void execute(Info* info) override {
        // TODO
    }
};

#endif /* COMMANDS_H_ */