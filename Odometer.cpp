#include <Arduino.h>
#include "Odometer.h"

Odometer::Odometer(){
	 checkInTime =0;
	 lastCheck=0;
	 rotateCount =0;
	 flexPin = 0;
	 min = 0;
	 max = 0;
	 moving = false;
	 movingElapsed = 0;
	 odometerClicked = false;
}


void Odometer::setup(int pSampleSize,	int pPin,	int pTolerance,int tireCircumference)
{
  currSpeed =0;
	lastCheck = 0;
	flexPin = pPin;
	movingTimeout = 3000;
  tireCirc = tireCircumference;
	establishBase(pSampleSize,pPin,pTolerance);
}

void Odometer::establishBase(int pSampleSize,int pPin,int pTolerance)
{
	long sampleTotal=0;
	long flexAverage;
	if(pSampleSize>0)
	{
		for(int x=0;x<pSampleSize;x++)
		{
			sampleTotal +=analogRead(pPin);
			delay(10);
		}
		flexAverage = sampleTotal/pSampleSize;
		min = flexAverage - pTolerance;
		max = flexAverage +pTolerance;
	}
	  Serial.print("odometer base line = ");
		Serial.println(flexAverage);
	
}

void Odometer::update(long currTime)
{
		int currValue =analogRead(flexPin);
		bool clicked = (currValue<min || currValue >max);
		movingElapsed = clicked ?currTime:movingElapsed;
    if(movingElapsed>0){
      currSpeed = tireCirc/(movingElapsed*0.001);
    }
		moving = odometerClicked 
			&& !clicked 
			|| (currTime - movingElapsed)< movingTimeout; 
		odometerClicked = clicked;
		lastCheck = currTime;
		rotateCount += (int)moving;
}

void Odometer::resetValues(long pCurrTime)
{
	movingElapsed  =pCurrTime;
	lastCheck = pCurrTime;
}
