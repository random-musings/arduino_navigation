
#include <stdint.h>

class Odometer
{
	int flexPin;
	long checkInTime;
	long lastCheck;
	long min,max;
	long movingTimeout;
	long movingElapsed;
	bool odometerClicked; 
	bool moving; 
	int rotateCount;
	float tireCirc;

	public:
	Odometer();
	void update(long currTime);
	void setup(int pSampleSize,int pPin,	int pTolerance,int tireCircumference);
	void resetValues(long currTime);
  float currSpeed;
	
	protected:
	void establishBase(int pSampleSize,int pPin,int pTolerance);
};
