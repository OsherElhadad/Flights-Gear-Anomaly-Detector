/*
 * Server.h
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <pthread.h>
#include <netinet/in.h>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include "commands.h"
#include "CLI.h"

using namespace std;

// SocketIO - interaction with client using socket
class SocketIO: public DefaultIO {
    int clientID;
public:

    // constructor
    explicit SocketIO(int id):clientID(id){};

    // read a string from client using socket

    string read() override {
        char temp = 0;
        string data;

        // read all chars from client and append them to one string
        while (temp != '\n') {
            recv(this->clientID, &temp, sizeof(char), 0);

            // in case of no more chars, break
            if (temp == '\n')
                break;

            // append new char to string
            data += temp;
        }
        return data;
    }

    // write a string to client using socket
    void write(string text) override {
        send(this->clientID, text.c_str(), text.length(), 0);
    }

    // write a float to client using socket
    void write(float f) override {
        std::stringstream sstream;
        sstream << f;
        write(sstream.str());
    }

    // read a float from client using socket
    void read(float* f) override{
        float temp;
        recv(this->clientID, &temp, sizeof(float), 0);
        *f = temp;
    }
};


// interface which handles with client
class ClientHandler {
public:

    // handles with the received client
    virtual void handle(int clientID)=0;
};

// handles anomaly detection request from client
class AnomalyDetectionHandler:public ClientHandler {
public:

    // handles with the received client and active anomaly detection program
    void handle(int clientID) override{
        auto* io = new SocketIO(clientID);
        CLI* cli = new CLI(io);
        (*cli).start();

        delete io;
        delete cli;
    }
};

// the server has a socket and can start listening up to 5 clients at the same time
class Server {

    // the server start listening on that thread
    thread* t;

    // the file descriptor of the socket
    int fd;

    // the client and server socket address
    sockaddr_in client;
    sockaddr_in server;

    // a boolean variable that stops the thread t
    volatile bool stopping;

public:

    // constructor - create the server socket
    explicit Server(int port) noexcept(false);

    // destructor
    virtual ~Server() = default;

    // start a new thread that the server listen on, and listen up to 5 clients at the same time
    void start(ClientHandler& ch) noexcept(false);

    // stop the thread t (in start)
    void stop();
};

#endif /* SERVER_H_ */
