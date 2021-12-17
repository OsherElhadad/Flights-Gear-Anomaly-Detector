/*
 * CLI.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */
#include "CLI.h"

// constructor
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->createCommands();
}

// virtual method- create by default 1. upload a time series csv file. 2. algorithm settings.
//3. detect anomalies. 4. display results. 5. upload anomalies and analyze results. 6. exit.
void CLI::createCommands() {
    this->commands.push_back(new UploadTimeSeriesCommand(this->dio));
    this->commands.push_back(new ThresholdCommand(this->dio));
    this->commands.push_back(new DetectAnomaliesCommand(this->dio));
    this->commands.push_back(new DisplayAnomaliesCommand(this->dio));
    this->commands.push_back(new UploadAnomaliesAndAnalyzeCommand(this->dio));
    this->commands.push_back(new ExitCommand(this->dio));
}

// starts the interaction with the user
void CLI::start() {
    Info* sharedInfo = new Info();
    int userInput;
    do {
        this->dio->write("Welcome to the Anomaly Detection Server.\n");
        this->dio->write("Please choose an option:\n");
        int size = this->commands.size();

        // write every command description
        for (int i = 0; i < size; ++i) {
            this->dio->write(i + 1);
            this->dio->write(". " + commands.at(i)->getDescription());
        }
        userInput = stoi(this->dio->read());
        while (userInput > 6 || userInput < 1) {
            userInput = stoi(this->dio->read());
        }

        // runs the chosen command
        this->commands.at(userInput - 1)->execute(sharedInfo);
    } while (userInput != 6);
}

// destructor
CLI::~CLI() {
    for (auto & command : this->commands) {
        delete command;
    }
}
