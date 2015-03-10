/*
 * Game.hpp
 *
 *  Created on: Feb 22, 2015
 *      Author: kodo
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include <thread>
#include <sys/time.h>
#include "GenObj.hpp"
#include "GameState.hpp"

class Game {
public:
	unsigned int nextGuid;
	std::vector<GenObject*> recentlyCreatedObjects;
	std::vector<GenObject*> objectsToBeDeleted; //objects that will be deleted in this frame
	std::vector<std::string> npObjects; //guids of Non-player Objects
	std::vector<std::string> players; //guids of the players
	std::map<std::string, GenObject*> objects; //dictionary to any object with a guid in the game
    std::thread stepThread;

	Game();
	~Game();
	GenObject* newObject(int,int,ObjType);
	void deleteObject(unsigned int);
	void deleteObject(std::string);
	GenObject* createPlayer(int, int);
	std::string deletePlayer(std::string);
	void removeConnectedPlayerGuid(std::string);
	void update(std::string, std::string);
	std::vector<std::string> getAttributes(unsigned int, std::string) const;
	std::string getDifStateMessage();

    virtual void step();

	//EVENTS
	void bomberDropsABomb(std::string);
	virtual void onBombExplosion(void*);

	//GETTERS && SETTERS
	unsigned int getNumPlayersOnGame() const;
	std::vector<std::string> getAllGuids() const;
private:
	std::vector<std::string> connectedPlayersGuid;
	GameState* gs;
	bool isRunning;
};


#endif /* GAME_HPP_ */
