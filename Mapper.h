
#define OBSTACLE '#'
#define UNKNOWN '?'
#define OPENSPACE '_'

class Mapper
{
	public:
	Mapper();
	Mapper(int mapWidth, int mapHeight,
              int newCellWidth,
              int newCellHeight);
	
 
	char topography[20][20];
	long lastUpdate;
	bool endOfMap;
  int cellWidth;
  int cellHeight;
	
	double posX,posY;
	int mapX,mapY;
	int width,height;
	
	void update(long currTime, 
						double velX,
						double velY, 
						bool	collision);
						
	void printMap();
	
	bool offMap();
	
	void initMap(int mapWidth, int mapHeight,
              int newCellWidth,
              int newCellHeight);
	
	double getPosX();
	double getPosY();
	
	int getMapX();
	int getMapY();

	double getMapPosX(int x);
	double getMapPosY(int y);
	
	char getSpaceValue(int iX,int iY);
	
	bool getMapIxAtPosition(float xPos, float yPos,int &x,int &y);
	

	private:

	

};
