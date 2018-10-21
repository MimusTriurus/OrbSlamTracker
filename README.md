# OrbSlamTracker

Project is based on ORB_SLAM2 (https://github.com/raulmur/ORB_SLAM2)

[![SLAM + OpenGL](http://img.youtube.com/vi/oA76ZAMNR90/0.jpg)](http://www.youtube.com/watch?v=oA76ZAMNR90 "")

[![SLAM + Unity 5](http://img.youtube.com/vi/SGa-UU-WdEA/0.jpg)](http://www.youtube.com/watch?v=SGa-UU-WdEA "")


System requirements: 
-------
1. ОС: Windows x64; 
2. Qt 5.5.1 or higher;
3. OpenCV version: 3.1;
4. Compiler: : MSVC2013 x64;
4. Two Web-cameras or one stereocamera. 

Assembly sequence:
----------------------------------------------
1. Open the project in QtCreator;
2. Set up assembly pattern "Release";
3. Build the project;

Adjustment:
----------------------------------------------
1. Copy libraries OpenCV (https://cloud.mail.ru/public/KMiB/znjPSo2CZ) to bin folder;
2. Copy libraries Qt to bin folder;
3. Copy files from directory /assets (files: ORBvoc.txt, settings.yaml, webcam.yaml) to bin folder;
4. Calibrate stereopair:
 	* binary files calibration program - https://cloud.mail.ru/public/5iHb/CbBNiNDin);
 	* source codes calibration program - https://github.com/MimusTriurus/QtStereoCamCalibration;
5. Copy generated calibration file - extrinsics.yaml to bin folder;
6. Enter parameters of camera matrix in file webcam.yaml: Camera.fx, Camera.fy, Camera.cx, Camera.cy (parameters K1[0,0][1,1][0,2][1,2] in file extrinsics.yaml respectively)
7. Set up in file settings.yaml:
	* leftCamIndex, rightCamIndex - index of left camera and index of right camera respectively;
	* frameWidth, frameHeight - camera resolution
	* calibrationFilePath - path to calibration file extrinsics.yaml
8. Copy library OrbSlamTracker.dll to bin folder

Remark:
----------------------------------------------
Colibration results: smaller error value (less than 0.5) indicates better tracker work.

