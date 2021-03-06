#include <cstdlib>
#include <iostream>
#include "lib/GameServer/GameServer.hpp"
#include "lib/ConnectionManager/ConnServer.hpp"

using namespace std;

void showHelp() {
    cout << " - help/?:    show this help" << endl;
    cout << " - exit/quit: stops server and exits application" << endl;
}

int main(int argc, char *argv[]) {

    unsigned short port = 8080;
    if (argc >= 2)
        port = atoi(argv[1]);

    std::cout << "starting connection server " << std::endl;
    ConnServer * cs = new ConnServer(port);
    cs->start();

    std::cout << "starting game server " << std::endl;

    GameServer::getInstance().initGameServer();

    cout << "Type 'help' to list available commands" << endl;

    string line;
    while (!getline(cin, line).eof()) {
        if (line.compare("exit") == 0 || line.compare("quit") == 0)
            break;
        else if (line.compare("help") == 0 || line.compare("?") == 0)
            showHelp();
        else
            cout << "Command not found." << endl;
    }

    std::cout << "Closing GameServer..." << endl;
    GameServer::getInstance().stop();
    std::cout << "Closing ConnServer..." << endl;
    cs->stop();

    std::cout << "Cleaning..." << endl;
    delete cs;
    std::cout << "Clean!" << endl;

    return 0;
}
