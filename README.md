# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* First, you will focus on loading images, setting up data structures and putting everything into a ring buffer to optimize memory load. 
* Then, you will integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed. 
* In the next part, you will then focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson. 
* In the last part, once the code framework is complete, you will test the various algorithms in different combinations and compare them with regard to some performance measures. 

See the classroom instruction and code comments for more details on each of these parts. Once you are finished with this project, the keypoint matching part will be set up and you can proceed to the next lesson, where the focus is on integrating Lidar points and on object detection using deep-learning. 

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.

##Mid Term Report
## MP.1 Data Buffer optimization.
1. I have implemented ring buffer in RingBuffer.h and tested using ring_buffer_test.cpp
2. Please define CUSTOM_BUFFER on the top to toggle between deque and custom ring buffer
3. #define CUSTOM_BUFFER uncomment in line number 22 in  MidTermProject_Camera_Student.cpp 

## MP.2 Keypoint Detection
1. Implemented key point detector in function calcKeyPointsHelper()
2. This function detects the keypoints 
3. Line number 130-138 in  MidTermProject_Camera_Student.cpp can be used to select the detector type. 

##MP.3 Keypoint Removal
1. cropFeatures(vector<cv::KeyPoint> &keypoints) function does this in line number 61 in  MidTermProject_Camera_Student.cpp

## MP.4 Keypoint Descriptors
1. Line number 140 to 146 in  MidTermProject_Camera_Student.cpp does implemnet this
## MP.5 Descriptor Matching
1. matchDescriptors() in matching2D_student.cpp implements this

## MP.6 Descriptor Distance Ration
1. Line number 66 to 73 in matching2D_student.cpp implements this. 

## MP.7 Performance Evaluation 1
1. FAST
2. BRISK
3. AKAZE
The top three are above. More details on SF_2D_tracking_performance_evaluation.xls attached with the project. 
## MP.8 Perf Evalaution 2
Please find the deatils on SF_2D_tracking_performance_evaluation.xls  attached with this project.
FAST  with ORB gave best results. 

## MP.9 Perf Evaluation 3
FAST with the following descriptors is fast as per the attached excel sheet SF_2D_tracking_performance_evaluation.xls
so this is best for our project. 
1. BRISK
2. BRIEF
3. ORB




