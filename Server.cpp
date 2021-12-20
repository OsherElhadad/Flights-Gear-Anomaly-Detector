
#include "Server.h"

Server::Server(int port) throw (const char*) {
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        throw "error while create socket";
    }
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(port);
    int bi = bind(this->fd, (sockaddr*)&this->server, sizeof(this->server));
    if (bi < 0) {
        throw "error while bind to the port";
    }

    if (listen(this->fd, 5) < 0) {
        throw "error while listen";
    }
    this->stopping = false;
}

void Server::start(ClientHandler& ch) throw(const char*){
    this->t = new thread([&ch, this]() {
        signal(SIGALRM, [](int sNum) { cout << "Timeout" << endl; });
        while(!this->stopping){
            alarm(5);
            socklen_t cSize = sizeof(this->client);
            int fdC = accept(this->fd,(struct sockaddr*)&this->client, &cSize);
            if(fdC < 0) {
                close(this->fd);
                throw "error while accepting";
            } else {
                new thread([&fdC, &ch](){
                    ch.handle(fdC);
                    close(fdC);
                });
            }
            alarm(0);
        }
        close(this->fd);
    });
}

void Server::stop(){
    this->stopping = true;
    this->t->join();
}

Server::~Server() {
}
