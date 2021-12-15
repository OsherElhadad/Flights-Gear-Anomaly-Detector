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
    this->commands.push_back(new UploadTimeSeriesCommand(dio));
    this->commands.push_back(new ThresholdCommand(dio));
    this->commands.push_back(new DetectAnomaliesCommand(dio));
    this->commands.push_back(new DisplayAnomaliesCommand(dio));
    this->commands.push_back(new UploadAnomaliesAndAnalyzeCommand(dio));
    this->commands.push_back(new ExitCommand(dio));
}

// starts the interaction with the user
void CLI::start() {
    this->dio->write("Welcome to the Anomaly Detection Server.\n");
    this->dio->write("Please choose an option:\n");
    int size = this->commands.size();

    // write every command description
    for (int i = 0; i < size; ++i) {
        this->dio->write(i + 1);
        this->dio->write(". ");
        this->dio->write(commands.at(i)->getDescription());
        this->dio->write("\n");
    }
    int userInput = stoi(this->dio->read());

    // runs the chosen command
    this->commands.at(userInput - 1)->execute();
}

// destructor
CLI::~CLI() {
    for (auto & command : this->commands) {
        delete command;
    }
}
