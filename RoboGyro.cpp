#include <Arduino.h>
#include <Wire.h>
#include "MPU3050lib.h"
#include "RoboGyro.h"
RoboGyro::RoboGyro(){}
void RoboGyro::init (int offsetX, int offsetY, int offsetZ,int newGyroTolerance)
{
  readingSz =10;
	gyroAvail= false;
  if (gyro.begin(OSEPP_GYRO_SW_ON) != 0){ return;}
  gyro.setRange(MPU3050_RANGE_PM500);
  gyro.setLowPassFilterBw(MPU3050_LOWPASS_5HZ);
	gyro.setOffsets(offsetX,offsetY,offsetZ);
  gyroAvail = true;
  gyroTolerance = newGyroTolerance;
}

void RoboGyro::update (){
	if(gyroAvail){
      double gX,gY,gZ;
			gyro.readDegPerSecond(&gX,&gY,&gZ); 
      readings[readingIx] = (abs(gY)<gyroTolerance)?0:gY;
      readingIx = (readingIx+1) % readingSz;
	}
}

double RoboGyro::getAngle(){
  double total=0;
  for(int x=0;x<readingSz;x++){ total +=readings[x];}
  return 0.01745332*total /readingSz;   //in rads
}


