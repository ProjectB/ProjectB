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

#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

class GameState {
public:
    int width, height, xPoint, yPoint, xMove, yMove, n;
    std::vector<GenObject> fixedObjects;
    std::map<std::string, GenObject> players;

    GameState() {
        n = 0;
        height = width = 11 * BLOCK_SIZE;
        xPoint = BLOCK_SIZE;
        yPoint = BLOCK_SIZE;
        xMove = BLOCK_SIZE / 10;
        yMove = BLOCK_SIZE / 10;


        newObject(2 * xPoint, 2 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(2 * xPoint, 4 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(2 * xPoint, 6 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(2 * xPoint, 8 * yPoint, xPoint - 2, yPoint - 2,square);

        newObject(4 * xPoint, 2 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(4 * xPoint, 4 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(4 * xPoint, 6 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(4 * xPoint, 8 * yPoint, xPoint - 2, yPoint - 2,square);

        newObject(6 * xPoint, 2 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(6 * xPoint, 4 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(6 * xPoint, 6 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(6 * xPoint, 8 * yPoint, xPoint - 2, yPoint - 2,square);

        newObject(8 * xPoint, 2 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(8 * xPoint, 4 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(8 * xPoint, 6 * yPoint, xPoint - 2, yPoint - 2,square);
        newObject(8 * xPoint, 8 * yPoint, xPoint - 2, yPoint - 2,square);

        /* 5 x 5
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
        */
    }

    void newObject(int x, int y, int w, int h, objType type) {
        std::stringstream ss;
        ss << (n++);
        GenObject square(ss.str(), type, x, y, w, h);
        fixedObjects.push_back(square);
        if (_GAMESTATE_DEBUG == 1) {
        	std::cout << "OBJECT CREATED: number" << n-1 << ": (" << x << "," << y << ")";
        	std::cout << " (" << x+w << "," << y << ")";
        	std::cout << " (" << x+w << "," << y+h << ")";
        	std::cout << " (" << x << "," << y+h << ")" << std::endl;
        	//printf("OBJECT CREATED: number %d: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", n-1, x, y, x+w, y, x+w, y+h, x, y+h);
        }
    }

    std::string generateDifStateMessage(bool onlyDiff = true) {
        std::string str;
        str += SEPARATOR;

        str += generateFixedObjectsDifStateMessage(onlyDiff);
        str += generatePlayersDifStateMessage(onlyDiff);

        for (std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++)
            (*it).second.hasChanges = false;

        return str;
    }

    std::string generateFixedObjectsDifStateMessage(bool onlyDiff) {
        std::stringstream ss;

        for (unsigned int i = 0; i < fixedObjects.size(); i++)
            if (fixedObjects[i].hasChanges || !onlyDiff)
                ss << fixedObjects[i].getMsg();

        return ss.str();
    }

    std::string generatePlayersDifStateMessage(bool onlyDiff) {
        std::stringstream ss;

        for (std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++)
            if ((*it).second.hasChanges || !onlyDiff)
                ss << (*it).second.getMsg();

        return ss.str();
    }

    void update(std::string guid, std::string msg) {
        int xdif = 0, ydif = 0;

        if (msg.compare("leftKey") == 0)
            xdif = -xMove;
        else if (msg.compare("rightKey") == 0)
            xdif = xMove;
        else if (msg.compare("upKey") == 0)
            ydif = -yMove;
        else if (msg.compare("downKey") == 0)
            ydif = yMove;

        if (_GAMESTATE_DEBUG == 1) {
			std::cout << "bomber: topleft(" << players[guid].x << "," << players[guid].y << ") topright(" << players[guid].x + players[guid].width;
			std::cout << "," << players[guid].y << ") bottomright(" << players[guid].x + players[guid].width << "," << players[guid].y + players[guid].height;
			std::cout << ") bottomleft(" << players[guid].x << "," << players[guid].y + players[guid].height << ")" << std::endl;
			//printf("bomber: topleft(%d,%d) topright(%d,%d) bottomright(%d,%d) bottomleft(%d,%d)\n", players[guid].x, players[guid].y, players[guid].x + players[guid].width, players[guid].y, players[guid].x + players[guid].width, players[guid].y + players[guid].height, players[guid].x, players[guid].y + players[guid].height);
        }

        players[guid].Move(xdif, ydif);
        /*
        if (collisionTest(players[guid].x + xdif, players[guid].y + ydif, players[guid].width, players[guid].height))
            players[guid].Move(xdif, ydif);
        else
            printf("DEBUG: COLLISION\n");
        */
    }
  
  //collision test for fixed objects. Possible problems with 2 moving objects (crossing each other)
  //false = collision happens, true otherwise
  bool collisionTest(int x, int y, int w, int h) {
    /* outside screen */
    if(x < 0 || y < 0 || x > SCREEN_WIDTH - w || y > SCREEN_HEIGHT - h) return false;

    int px,py;
    int n;
    int leastDist=-1;
    int centerx, centery;
    int a;

    /* find nearest object */
    for (unsigned int i = 0; i < fixedObjects.size(); i++) {
      if (fixedObjects[i].type == square) {
	if(leastDist<0) {
	  leastDist = distance(x,y,fixedObjects[i].x + fixedObjects[i].width, fixedObjects[i].y + fixedObjects[i].height);
	  n = i;
	} 
	else if((a = distance(x,y,fixedObjects[i].x + fixedObjects[i].width, fixedObjects[i].y + fixedObjects[i].height)) < leastDist)
	  {
	    n = i;
	    leastDist = a;
	  }
      }
    }

    centerx = fixedObjects[n].x + fixedObjects[n].width/2;
    centery = fixedObjects[n].y + fixedObjects[n].height/2;
    
    /* find nearest point to the center of the object */
    leastDist = distance(x,y,centerx,centery);
    px=x;
    py=y;
    
    if((a = distance(x+w,y, centerx,centery))< leastDist){
      px = x+w;
      leastDist = a;
    }
    if((a = distance(x+w, y+h, centerx,centery)) < leastDist) {
      px = x+w;
      py = y+h;
      leastDist = a;
    }
    if((a = distance(x,y+h, centerx,centery)) < leastDist) {
      px = x;
      py = y+h;
    }
    
    /* if the nearest point is farther than the radius of the object */
    if(distance(px,py,centerx, centery) > distance(fixedObjects[n].x, fixedObjects[n].y, centerx, centery)) {
    	std::cout << "DEBUG: FARTHER THAN POSSIBLE" << std::endl;
      //printf("DEBUG: FARTHER THAN POSSIBLE\n");
      return true;
    }
    
    /* 8 points of interest of the object */
    /* top */
    int p1x = centerx;
    int p1y = fixedObjects[n].y;
    
    /* right */
    int p2x = fixedObjects[n].x + fixedObjects[n].width;
    int p2y = centery;
    
    /* bottom */
    int p3x = centerx;
    int p3y = fixedObjects[n].y + fixedObjects[n].height;
    
    /* left */
    int p4x = fixedObjects[n].x;
    int p4y = centery;

    /* top left */
    int p5x = fixedObjects[n].x;
    int p5y = fixedObjects[n].y;
    
    /* top right */
    int p6x = fixedObjects[n].x + fixedObjects[n].width;
    int p6y = fixedObjects[n].y;
    
    /* bottom left */
    int p7x = fixedObjects[n].x;
    int p7y = fixedObjects[n].y + fixedObjects[n].height;

    /* bottom right */
    int p8x = fixedObjects[n].x + fixedObjects[n].width;
    int p8y = fixedObjects[n].y + fixedObjects[n].height;
    
    
    /* find the nearest PoI */
    int obx,oby;
    
    obx=p1x;
    oby=p1y;
    
    if(distance(px,py,p2x,p2y) < distance(px,py,obx,oby)) {
      obx = p2x;
      oby = p2y;
    }
    
    if(distance(px,py,p3x,p3y) < distance(px,py,obx,oby)) {
      obx = p3x;
      oby = p3y;
    }
    
    if(distance(px,py,p4x,p4y) < distance(px,py,obx,oby)) {
      obx = p4x;
      oby = p4y;
    }

    if(distance(px,py,p5x,p5y) < distance(px,py,obx,oby)) {
      obx = p5x;
      oby = p5y;
    }
    
    if(distance(px,py,p6x,p6y) < distance(px,py,obx,oby)) {
      obx = p6x;
      oby = p6y;
    }
    
    if(distance(px,py,p7x,p7y) < distance(px,py,obx,oby)) {
      obx = p7x;
      oby = p7y;
    }
    
    if(distance(px,py,p8x,p8y) < distance(px,py,obx,oby)) {
      obx = p8x;
      oby = p8y;
    }


    std::cout << "bomber(" << x << "," << y << "," << w << "," << h << ") -- p(" << px << "," << py << ") -- center[" << n << "](" << centerx << "," << centery;
    std::cout << ") -- PoI(" << obx << "," << oby << ") -- bottom(" << x << "," << y+h << ") -- top(" << centerx << "," << centery + fixedObjects[n].height/2 << ")." << std::endl;
    std::cout << "both distances: " << distance(px, py, centerx, centery) << " ---- " << distance(obx, oby, centerx, centery) << "." << std::endl;
    //printf("bomber(%d,%d,%d,%d) -- p(%d,%d) -- center[%d](%d,%d) -- PoI(%d,%d) -- bottom(%d,%d) -- top(%d,%d).\n", x,y,w,h, px,py, n,centerx,centery, obx,oby, x,y+h, centerx, centery + fixedObjects[n].height/2);
    //printf("both distances: %d ---- %d.\n", distance(px,py,centerx,centery), distance(obx,oby,centerx,centery));
    
    /* if the distance point-center is less than or equal to the distance to the nearest PoI, then we have a collision */
    if(distance(px,py,centerx, centery) <= distance(obx,oby,centerx,centery)) {
    	std::cout << "DEBUG: COLLISION ENDED!" << std::endl;
      //printf("DEBUG: COLLISION ENDED!\n");
      return false;
    }
    
    std::cout << "end" << std::endl;
    //printf("end\n");
    
    
    return true;
  }

  
private:

/* squared distance from point x to point y. */
int distance(int x1, int x2, int y1, int y2)
{
  int a = (x1 - y1)*(x1 - y1) + (x2 - y2)*(x2 - y2);
  if (a < 0) return -a;
  return a;
}
};
#endif /* GAMESTATE_HPP_ */
