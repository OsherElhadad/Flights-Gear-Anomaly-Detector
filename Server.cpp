/*
 * Server.cpp
 *
 * Author: Yossi Maatook, 208641472
 * Author: Osher Elhadad, 318969748
 */

#include "Server.h"

// constructor - create the server socket
Server::Server(int port) throw (const char*) {

    // create a socket for the server
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        throw "error while creating socket";
    }
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(port);
    int bi = bind(this->fd, (sockaddr*)&this->server, sizeof(this->server));
    if (bi < 0) {
        throw "error while binding to the port";
    }
    if (listen(this->fd, 5) < 0) {
        throw "error while listening";
    }
    this->stopping = false;
}

// start a new thread that the server listen on, and listen up to 5 clients at the same time
void Server::start(ClientHandler& ch) throw(const char*) {
    this->t = new thread([&ch, this]() {
        vector<thread*> tVec;
        bool error = false;
        while(!this->stopping) {

            // set time out of 2 seconds for accept a client socket
            struct timeval tv{};
            fd_set fdSet;
            FD_ZERO(&fdSet);
            FD_SET(fd, &fdSet);
            tv.tv_sec = (long)2;
            tv.tv_usec = 0;
            int iResult = select(this->fd + 1, &fdSet, (fd_set *) nullptr,
                                 (fd_set *) nullptr, &tv);
            if(iResult > 0) {
                socklen_t cSize = sizeof(this->client);
                int fdC = accept(this->fd, (struct sockaddr *) &this->client, &cSize);
                if (fdC < 0) {
                    error = true;
                    break;
                } else {

                    // create thread for the new client
                    auto* cl = new thread([&fdC, &ch]() {
                        ch.handle(fdC);
                        close(fdC);
                    });
                    tVec.push_back(cl);
                }
            }
        }
        for (auto cl : tVec) {

            // wait until t is closed
            cl->join();
            delete cl;
        }
        close(this->fd);
        if (error) {
            throw "error while creating client socket";
        }
    });
}

// stop the thread t (in start)
void Server::stop(){
    this->stopping = true;

    // wait until t is closed
    if (this->t)
        this->t->join();
    delete this->t;
}
