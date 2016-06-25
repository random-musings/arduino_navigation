#include "Mapper.h"
#include<arduino.h>

Mapper::Mapper()
{
	initMap(0,0,0,0);
}

 Mapper::Mapper(int mapWidth, int mapHeight,
              int newCellWidth,
              int newCellHeight)
{
	lastUpdate = 0;
	posX =0; posY =0;
	mapX =0; mapY=0;
	cellWidth = newCellWidth;
	cellHeight = newCellHeight;
	endOfMap = false;
}

void Mapper:: initMap(int mapWidth, int mapHeight,
              int newCellWidth,
              int newCellHeight){
	for(int x=0;x<mapWidth;x++){
		for(int y=0;y<mapHeight;y++){
			topography[x][y] = UNKNOWN;
		}		
	}
	width = mapWidth>100?100:mapWidth;
	height = mapHeight>100?100:mapHeight;
 //assume we are starting in the iddle of the map
  posX = mapWidth*0.5 * newCellWidth;
  posY = mapHeight*0.5 * newCellHeight;
  mapX = mapWidth*0.5; 
  mapY = mapHeight*0.5;
  topography[mapX][mapY] = OPENSPACE;
  cellWidth = newCellWidth;
  cellHeight  = newCellHeight;
  
}
	


	bool Mapper:: offMap()
	{
			return endOfMap;
	}
	


	void Mapper::update(long currTime, 
						double velX,
						double velY, 
						bool	collision)
	{
		if(!endOfMap && collision){
				topography[mapX][mapY] = OBSTACLE;
		}else{
     
      long offsetTime = currTime - lastUpdate;
			posX = posX + velX* offsetTime*0.001;
			posY = posY + velY* offsetTime*0.001;
		
			int newMapX = posX/cellWidth;
			int newMapY = posY/cellHeight;
			endOfMap = posX<0 || posY<0 || posX> width*cellWidth || posY> height*cellHeight;
			
      if(!collision
      && !endOfMap
      && (newMapX!= mapX || newMapY != mapY))
      {
        topography[newMapX][newMapY] = OPENSPACE;
      }
      //leave this here in case we need to do state checking of when collision was reported
      mapX = endOfMap?0:newMapX;
      mapY = endOfMap?0:newMapY;

     lastUpdate = currTime; 
		}
	
	}
						
	void Mapper::printMap()
	{
		  int x =0;
		  int y=0;
		for(x=0;x<width;x++){
			for(y=0;y<height;y++){
				char elt =  topography[x][y];
				Serial.print(elt);
			}		
			Serial.println(" ");
		}
	}

  

  	double Mapper::getPosX(){return posX;}
	double Mapper::getPosY(){return posY;}
	
	int Mapper::getMapX(){return mapX;}
	int Mapper::getMapY(){return mapY;}
	
	char Mapper::getSpaceValue(int iX,int iY)
	{
			if (iX>=0 && iX<width
			 && iY>=0 && iY<height){
				return  topography[iX][iY];
			}
		return UNKNOWN;
	}

	
	double Mapper::getMapPosX(int x)
	{
		return x * cellWidth;
	}
		
	double Mapper::getMapPosY(int y)
	{
		return y*cellHeight;
	}

	
	bool Mapper::getMapIxAtPosition(float xPos, float yPos,int &x,int &y)
	{
		
			x = xPos/cellWidth;
			y = yPos/cellHeight;
			
			return !(x<0 || y<0 || x> (width*cellWidth-1) || y> (height*cellHeight-1));
	}
