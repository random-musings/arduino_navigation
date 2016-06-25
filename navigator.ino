/*void setup(){}
void loop(){}

*/
#include <Wire.h>
#include "ADXL345lib.h"
#include "MPU3050lib.h"
#include "Motor.h"
#include "Odometer.h"
#include "RoboAccelerometer.h"
#include "RoboGyro.h"
#include "Mapper.h"

RoboGyro gyro;
RoboAccelerometer accel;
Odometer od;
Motor motor;

Mapper mapper;

double currAngle;
double velX,velY;
double goalX,goalY;
double runtX,runtY;
int uX,uY;


long delta=0;
long lastTime=0;
bool front,left,rightTurn;
long mapTime;
float prevDistance;
bool settingGoal;
long chkTime; //heck every so often to ensure we are getting closer to goal - fi not set new goal

int counter=1;
void setGoal(long currTime){
      runtX =mapper.posX;
      runtY = mapper.posY;
     int stage = counter%4;
     switch(stage)
     {
        case 0: uX = 0;  uY= 0; break;
      case 1: uX = 5; uY =5; break;
      case 2: uX = 0; uY = 0; break;
      case 3: uX = 5; uY = 5; break;
     }
        left = isLeft(runtX,runtY
              ,runtX+velX,runtY+velY
              ,uX*mapper.cellWidth,uY*mapper.cellHeight);
         front = isLeft(runtX,runtY
              ,-runtX*velY*mapper.cellWidth,-runtY*velX*mapper.cellHeight
              ,uX*mapper.cellWidth,uY*mapper.cellHeight);
       rightTurn =( front ^ left);
       goalX =  uX*mapper.cellWidth + 0.5* mapper.cellWidth;
       goalY  = uY*mapper.cellHeight+ 0.5* mapper.cellHeight;
       prevDistance = distance (runtX,runtY,goalX,goalY); //get our distance from target
       prevDistance = prevDistance*2;
       motor.setState(rightTurn?STATE_TURN_LEFT:STATE_TURN_RIGHT);
       od.currSpeed=10;
}


void setup() {
  Serial.begin(9600);
  
  gyro.init(-120,-50,17,//int offsetX, int offsetY, int offsetZ,
            0.04);         //int newGyroTolerance);
  
  accel.init(20,        // int sampleSize,
             -7,-7,8,   // int offsetX, int offsetY, int offsetZ,
             0.1,0.03,  // double newCollisionTolerance, double newMovingTolerance,
             2000);     // long newAccelShakeTimeout);
             
  od.setup(50,//int pSampleSize,  
          0,//int pPin,
          10,//int pTolerance);
          21 //tire circumference
   );
   mapper.initMap(5,    //int mapWidth, 
                  5,    //int mapHeight,  
                  100,   //int newCellWidth,   
                  100);   //int newCellHeight);
   runtX = mapper.getMapX() * mapper.cellWidth + 0.5* mapper.cellWidth;
   runtY = mapper.getMapY() * mapper.cellHeight + 0.5* mapper.cellHeight;
   goalX = runtX;
   goalY = runtY;
   currAngle = 0;                 
   velX =cos(currAngle);
   velY = sin(currAngle);
  settingGoal=false;
   motor.setState(STATE_IDLE);
   setGoal(millis());
}



 bool isLeft(float sX,float sY,float  eX,float eY,float  goalX,float goalY){
     return ((eX - sX)*(goalY - sY) - (eY - sY)*(goalX - sX)) > 0;
}

 float distance(float sX,float sY,float  eX,float eY)
{
    return sqrt( (eX-sX)*(eX-sX) +(eY-sY)*(eY-sY));
}


bool isDirectionCorrect(long currTime)
{
  bool correctDirection = false;
   if((currTime -lastTime)>50)
     {
      lastTime = currTime;
      gyro.update();
      accel.update(currTime);
      od.update(currTime);
      mapper.update(currTime,6*velX,6*velY,false);
      motor.update(millis());
      float gyroAngle = gyro.getAngle();
      float accelAngle = accel.angle_z;
      float newCurrAngle = currAngle +gyroAngle*0.85+ accelAngle*0.15;
      
      if( newCurrAngle!=currAngle && gyroAngle !=0 && accelAngle !=0)
      {
        velX = cos(newCurrAngle);
        velY = sin(newCurrAngle);
       
        //account for moving backwards
         double coverage = distance (runtX,runtY,goalX,goalY);
          //figure out right/left turns
        int destX = runtX + velX* coverage; 
        int destY = runtY + velY* coverage;
        double resultDistance = distance(destX,destY,goalX,goalY);
        if( resultDistance<10)
        {
          correctDirection = true;
          settingGoal=false;
        }
        currAngle = newCurrAngle;
         mapTime = currTime;
      }
     }
      return correctDirection;
}

void loop() {
   long currTime = millis();
    motor.update(currTime);
    float actDistance = distance( mapper.posX,mapper.posY,goalX,goalY);

     if(isDirectionCorrect(currTime))
     {
        motor.setState(front?STATE_FORWARD:STATE_BACKWARD);
     }else{
        motor.setState(left?STATE_TURN_LEFT:STATE_TURN_RIGHT);
        od.currSpeed=6;
     }
        Serial.println(actDistance);
      //if we start get farther away or off the map stop and start turnin
      if(actDistance< 15  )
      {
         Serial.print(F("  STOP"));
         Serial.println(actDistance);
         motor.setState(STATE_IDLE);
           settingGoal=true;
          setGoal(currTime);
      }else if(!settingGoal &&  prevDistance<actDistance)
      {
        chkTime = currTime;
         Serial.println(F(" missed target set new goal"));
        od.currSpeed=0;
        motor.setState(STATE_IDLE);
         settingGoal=true;
         setGoal(currTime);
      }  
      
      if((currTime - mapTime)>5000){
        mapper.printMap();
        mapTime = currTime;
        prevDistance = actDistance;
      }
     
  }
  
  

