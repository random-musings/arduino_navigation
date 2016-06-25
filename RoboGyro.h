class RoboGyro
{
		public:
			RoboGyro();
			void  init (int offsetX, int offsetY, int offsetZ,int newGyroTolerance);
			void update();
     double readings[10];
     int readingIx;
     int readingSz;
			bool gyroAvail;
      double gyroTolerance;
      Gyroscope gyro;
      double getAngle();
};
