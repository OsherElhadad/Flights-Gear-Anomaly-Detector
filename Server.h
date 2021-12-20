/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){

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