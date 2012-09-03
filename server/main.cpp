#include <cstdlib>
#include "Server.hpp"


int main(int argc, char *argv[]) {
    unsigned short port = 8080;
    if (argc >= 2)
        port = atoi(argv[1]);

    Server server(port);
    server.start();
}
