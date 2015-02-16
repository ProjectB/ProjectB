/*
 * GameState.hpp
 *
 *  Created on: Nov 28, 2012
 *      Author: ricardo
 */

#include <vector>
#include <map>

#include "../defs.hpp"
#include "GenObj.hpp"
#include "Block.hpp"
#include "Bomber.hpp"
#include "Bomb.hpp"

#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

class GameState {
public:
	int width, height, xMove, yMove;
	unsigned int n;
	std::vector<GenObject*> recentlyCreatedObjects;
	std::vector<std::string> npObjects; //guids of Non-player Objects
	std::vector<std::string> players; //guids of the players
	std::map<std::string, GenObject*> objects; //dictionary to any object with a guid in the game

	GameState();
	~GameState();
	GenObject* newObject(int,int,ObjType);
	void deleteObject(unsigned int);
	std::string updateNPObjects();
	GenObject* createPlayer(int, int);
	std::string generateDifStateMessage();
	std::string generateObjectsDifStateMessage();
	void update(std::string, std::string);
	std::vector<std::string> getAttributes(unsigned int, std::string) const;

	//EVENTS
	void bomberDropsABomb(std::string);
	void onBombExplosion(Bomb* bomb);

private:
};
#endif /* GAMESTATE_HPP_ */
