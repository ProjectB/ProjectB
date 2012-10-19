/*
 * BomberServer.hpp
 *
 *  Created on: Sep 15, 2012
 *      Author: ricardo
 */

#ifndef BOMBERSERVER_HPP_
#define BOMBERSERVER_HPP_

#include "GameServer.hpp"
#include <sstream>

#define BLOCK_SIZE 40
#define SEPARATOR "|"

enum objType {
    none, square, bomber
};

class BomberServer: public virtual GameServer {

    class GenObject {
    public:
        objType type;
        int x, y, height, width;

        GenObject() {
            type = none;
            x = y = height = width = -1;
        }

        GenObject(objType type, int x, int y, int height, int width) {
            this->type = type;
            this->x = x;
            this->y = y;
            this->height = height;
            this->width = width;
        }
    };

    class GameState {
    public:
        int width, height, xPoint, yPoint, xMove, yMove;
        std::vector<GenObject> fixedObjects;
        std::map<std::string, GenObject> players;

        GameState() {
            height = width = 11*BLOCK_SIZE;
            xPoint = BLOCK_SIZE;
            yPoint = BLOCK_SIZE;
            xMove = BLOCK_SIZE/5;
            yMove = BLOCK_SIZE/5;

            newObject(xPoint, yPoint, xPoint-2, yPoint-2, square);
            newObject(xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
            newObject(xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
            newObject(xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
            newObject(xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

            newObject(3*xPoint, yPoint, xPoint-2, yPoint-2, square);
            newObject(3*xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
            newObject(3*xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
            newObject(3*xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
            newObject(3*xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

            newObject(5*xPoint, yPoint, xPoint-2, yPoint-2, square);
            newObject(5*xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
            newObject(5*xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
            newObject(5*xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
            newObject(5*xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

            newObject(7*xPoint, yPoint, xPoint-2, yPoint-2, square);
            newObject(7*xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
            newObject(7*xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
            newObject(7*xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
            newObject(7*xPoint, 9*yPoint, xPoint-2, yPoint-2, square);

            newObject(9*xPoint, yPoint, xPoint-2, yPoint-2, square);
            newObject(9*xPoint, 3*yPoint, xPoint-2, yPoint-2, square);
            newObject(9*xPoint, 5*yPoint, xPoint-2, yPoint-2, square);
            newObject(9*xPoint, 7*yPoint, xPoint-2, yPoint-2, square);
            newObject(9*xPoint, 9*yPoint, xPoint-2, yPoint-2, square);
        }

        void newObject(int x, int y, int w, int h, objType type) {
            GenObject square(type, x, y, w, h);
            fixedObjects.push_back(square);
        }


        std::string getMsg() {
            std::stringstream ss;
            ss << SEPARATOR;
            for(std::vector<GenObject>::iterator it = fixedObjects.begin(); it != fixedObjects.end(); it++)
                ss << (*it).type << SEPARATOR << (*it).x << SEPARATOR << (*it).y << SEPARATOR << (*it).width << SEPARATOR << (*it).height << SEPARATOR;
            for(std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++)
                ss << (*it).second.type << SEPARATOR << (*it).second.x << SEPARATOR << (*it).second.y;
            return ss.str();
        }
    };

public:
    BomberServer();
    virtual ~BomberServer();
private:
    void step();
    void onClientConnect(ClientConnection * client);
    void onClientDisconnect(ClientConnection * client);
    void onNewMessage(std::string guid, std::string msg);


    void saveDiffMsg(GenObject obj);

    GameState gs;
};

#endif /* BOMBERSERVER_HPP_ */
