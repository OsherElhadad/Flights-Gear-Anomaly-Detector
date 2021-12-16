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

    string readFileData(string path) {
        string line;
        while (line != "done") {
            // TODO
        }
    }
};

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
        return;
    }
};

class Command{
    const string description;
protected:
    DefaultIO* dio;
public:
    Command(DefaultIO* dio, const string& d):dio(dio), description{d}{}
    virtual void execute()=0;
    virtual ~Command(){}

    string getDescription() const {
        return this->description;
    }
};

class UploadTimeSeriesCommand: public Command {
public:
    UploadTimeSeriesCommand(DefaultIO* dio):Command(dio, "upload a time series csv file"){}
    void execute() override {
        this->dio->write("Please upload your local train CSV file.");
        string pathInput;
        cin >> pathInput;
        string csvData = this->dio->readFileData(pathInput);
        // TODO
    }
};

class ThresholdCommand: public Command {
public:
    ThresholdCommand(DefaultIO* dio):Command(dio, "algorithm settings"){}
    void execute() override {
        // TODO
    }
};

class DetectAnomaliesCommand: public Command {
public:
    DetectAnomaliesCommand(DefaultIO* dio):Command(dio, "detect anomalies"){}
    void execute() override {
        // TODO
    }
};

class DisplayAnomaliesCommand: public Command {
public:
    DisplayAnomaliesCommand(DefaultIO* dio):Command(dio, "display results"){}
    void execute() override {
        // TODO
    }
};

class UploadAnomaliesAndAnalyzeCommand: public Command {
public:
    UploadAnomaliesAndAnalyzeCommand(DefaultIO* dio):Command(dio, "upload anomalies and analyze results"){}
    void execute() override {
        // TODO
    }
};

class ExitCommand: public Command {
public:
    ExitCommand(DefaultIO* dio):Command(dio, "exit"){}
    void execute() override {
        // TODO
    }
};

#endif /* COMMANDS_H_ */