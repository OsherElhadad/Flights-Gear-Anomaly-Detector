/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include <pthread.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "commands.h"
#include "CLI.h"

// SocketIO - interaction with client using socket
class SocketIO: public DefaultIO {
    int clientID;
public:

    // constructor
    SocketIO(int id):clientID(id){};
    string read() override {
        char temp = 0;
        string data;

        while (temp != '\n') {
            recv(this->clientID, &temp, sizeof(char), 0);
            if (temp == '\n')
                break;
            data += temp;
        }
        return data;
    }

    void write(string text) override {
        send(this->clientID, text.c_str(), text.length(), 0);
    }

    void write(float f) override {
        std::stringstream sstream;
        sstream << f;
        write(sstream.str());
    }

    void read(float* f) {
        float temp;
        recv(this->clientID, &temp, sizeof(float), 0);
        *f = temp;
    }
};


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler {
public:
    virtual void handle(int clientID){
        auto* io = new SocketIO(clientID);
        CLI* cli = new CLI(io);
        (*cli).start();

        delete io;
        delete cli;
    }
};

// implement on Server.cpp
class Server {
    thread* t; // the thread to run the start() method in
    int fd;
    sockaddr_in client;
    sockaddr_in server;
    volatile bool stopping;

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};

#endif /* SERVER_H_ */
