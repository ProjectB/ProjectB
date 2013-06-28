/*
 * defs.hpp
 *
 *  Created on: Sep 4, 2012
 *      Author: ricardo
 */

#ifndef DEFS_HPP_
#define DEFS_HPP_

#define RCVBUFSIZE 256
#define _SERVER_DEBUG 1
#define _SERVER_ERR_DEBUG 1

#define _GAMESERVER_DEBUG 1
#define _GAMESERVER_ERR_DEBUG 1

#define _CLIENT_DEBUG 1
#define _CLIENT_MSG_DEBUG 0
#define _CLIENT_ERR_DEBUG 1


/************ GAME DEFINES ************/

enum objType {
    none, square, bomber
};

#define FPS 30
#define BLOCK_SIZE 38
#define BOMBER_WIDTH 32
#define BOMBER_HEIGHT 42
#define SEPARATOR "|"
#define SCREEN_WIDTH 440
#define SCREEN_HEIGHT 440

#endif /* DEFS_HPP_ */
