/*
 * Block.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: kodo
 */

#include "Block.hpp"
using namespace std;
#include <string>
#include <sstream>

Block::Block(bool isBreakable, std::string guid, int x, int y, int w, int h)
: GenObject(guid, ObjType::_Square, x, y, w, h)
{
	this->HP = 1;
	this->isBreakable = isBreakable;
}
Block::~Block()
{

}
std::string Block::generateObjectState() const
{
	std::stringstream ss;

	ss << this->HP;
	ss << ";" << this->isBreakable;

	return ss.str();
}

short int Block::getHP() const
{
	return this->HP;
}
void Block::setHP(short int HP)
{
	this->HP = HP;
}
void Block::decreaseHP(short int val)
{
	if(this->isBreakable == true)
	{
		this->HP -= val;
		if(this->HP <= 0)
			delete this;
	}
}
std::string Block::getObjectState() const
{
	return this->generateObjectState();
}
