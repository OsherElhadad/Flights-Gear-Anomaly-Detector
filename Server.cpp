
#include "Server.h"

Server::Server(int port)throw (const char*) {
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

    if (listen(this->fd, 10) < 0) {
        throw "error while listen";
    }
}

void Server::start(ClientHandler& ch)throw(const char*){
}

void Server::stop(){
    t->join(); // do not delete this!
}

Server::~Server() {
}
