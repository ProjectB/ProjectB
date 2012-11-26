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
        std::string guid;
        objType type;
        int x, y, height, width;

        GenObject() {
            guid = "";
            type = none;
            x = y = height = width = -1;
        }

        GenObject(std::string id, objType type, int x, int y, int height, int width) {
            this->guid = id;
            this->type = type;
            this->x = x;
            this->y = y;
            this->height = height;
            this->width = width;
        }

        std::string getMsg() {
            std::stringstream ss;
            if (type == square)
                ss << "sq" << SEPARATOR << x << SEPARATOR << y << SEPARATOR << width << SEPARATOR << height;
            else if (type == bomber)
                ss << "bomber" << SEPARATOR << guid << SEPARATOR << x << SEPARATOR << y;
            else
                ss << "invalid";

            ss << SEPARATOR;

            return ss.str();
        }
    };

    class GameState {
    public:
        int width, height, xPoint, yPoint, xMove, yMove, n;
        std::vector<GenObject> oldFixedObjects, lastFixedObjects, fixedObjects;
        std::map<std::string, GenObject> oldPlayers, lastPlayers, players;

        GameState() {
            n = 0;
            height = width = 11 * BLOCK_SIZE;
            xPoint = BLOCK_SIZE;
            yPoint = BLOCK_SIZE;
            xMove = BLOCK_SIZE / 5;
            yMove = BLOCK_SIZE / 5;

            newObject(xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);

            newObject(3 * xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(3 * xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(3 * xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(3 * xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(3 * xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);

            newObject(5 * xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(5 * xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(5 * xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(5 * xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(5 * xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);

            newObject(7 * xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(7 * xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(7 * xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(7 * xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(7 * xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);

            newObject(9 * xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(9 * xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(9 * xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(9 * xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(9 * xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);
        }

        void newObject(int x, int y, int w, int h, objType type) {
            std::stringstream ss;
            ss << (n++);
            GenObject square(ss.str(), type, x, y, w, h);
            fixedObjects.push_back(square);
        }

        std::string getMsg(bool first) {
            std::string str;
            str += SEPARATOR;

            updateOld();

            str += updateLast(first);

            if (!first) {
                str += msgFixedObjects();
                str += msgPlayers();
            }

            return str;
        }

        void updateOld() {
            // atualiza ultimo snapshot
            oldFixedObjects.clear();
            oldPlayers.clear();

            if (!lastFixedObjects.empty())
                for (std::vector<GenObject>::iterator it = lastFixedObjects.begin(); it != lastFixedObjects.end();
                        it++) {
                    oldFixedObjects.push_back(*it);
                }

            if (!lastPlayers.empty())
                for (std::map<std::string, GenObject>::iterator it = lastPlayers.begin(); it != lastPlayers.end();
                        it++) {
                    oldPlayers = lastPlayers;
                }
        }

        std::string updateLast(bool first) {
            std::stringstream ss;
            //cria a mensagem, criando um snapshot

            lastFixedObjects.clear();
            lastPlayers.clear();

            for (std::vector<GenObject>::iterator it = fixedObjects.begin(); it != fixedObjects.end(); it++) {
                if (first)
                    ss << (*it).getMsg();

                lastFixedObjects.push_back(*it);
            }

            for (std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++) {
                if (first)
                    ss << (*it).second.getMsg();

                lastPlayers = players;
            }

            return ss.str();
        }

        std::string msgFixedObjects() {
            std::stringstream ss;

            for (unsigned int i = 0; i < fixedObjects.size(); i++)
                for (unsigned int j = 0; j < oldFixedObjects.size(); j++)
                    //compara os guids dos 2 objects
                    if (oldFixedObjects[j].guid.compare(fixedObjects[i].guid) == 0)
                        if (oldFixedObjects[j].type != fixedObjects[i].type || oldFixedObjects[j].x != fixedObjects[i].x
                                || oldFixedObjects[j].y != fixedObjects[i].y
                                || oldFixedObjects[j].height != fixedObjects[i].height
                                || oldFixedObjects[j].width != fixedObjects[i].width) {
                            ss << fixedObjects[i].getMsg();
                        }

            return ss.str();
        }

        std::string msgPlayers() {
            std::stringstream ss;

            for (std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++)
                for (std::map<std::string, GenObject>::iterator oit = oldPlayers.begin(); oit != oldPlayers.end();
                        oit++)
                    if ((*oit).first.compare((*it).first) == 0)
                        if ((*oit).second.type != (*it).second.type || (*oit).second.x != (*it).second.x
                                || (*oit).second.y != (*it).second.y) {
                            ss << (*it).second.getMsg();
                        }

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

    GameState gs;

};

#endif /* BOMBERSERVER_HPP_ */
