#include <cstdlib>
#include <iostream>
#include "BomberServer.hpp"

using namespace std;

void showHelp() {
    cout << " - help/?:    show this help" << endl;
    cout << " - exit/quit: stops server and exits application" << endl;
}

int main(int argc, char *argv[]) {

    unsigned short port = 8080;
    if (argc >= 2)
        port = atoi(argv[1]);

    BomberServer * bs = new BomberServer(port);
    bs->start();

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

    cout << "Closing..." << endl;
    bs->stop();
    delete bs;

    return 0;
}
