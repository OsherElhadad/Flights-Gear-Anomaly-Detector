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

    string readFileData(string fileData) {
        string data, line;
        while (line != "done") {
            // TODO
        }
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
        // TODO
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
        string csvData = this->dio->read();
        // TODO
    }
};

class ThresholdCommand: public Command {
public:
    ThresholdCommand(DefaultIO* dio):Command(dio, "algorithm settings"){}
    void execute(Info* info) override {
        string current = std::to_string(info->getThreshold());
        cout << "The current correlation threshold is " + current + "\n";
        float input = -1;
        while (input < 0 || input > 1) {
            cin >> input;
            if (input < 0 || input > 1)
                cout << "please choose a value between 0 and 1.\n";
        }
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