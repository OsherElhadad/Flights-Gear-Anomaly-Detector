/*
 * CLI.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

// Command line interface - used to start interaction with user
class CLI {
    DefaultIO* dio;
    vector<Command*> commands;
public:

    // constructor
    CLI(DefaultIO* dio);

    // starts the interaction with the user
    void start();

    // destructor
    virtual ~CLI();

    // virtual method- create by default 1. upload a time series csv file. 2. algorithm settings.
    //3. detect anomalies. 4. display results. 5. upload anomalies and analyze results. 6. exit.
    virtual void createCommands();
};

#endif /* CLI_H_ */