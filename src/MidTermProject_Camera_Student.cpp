/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"
#include "ring_buffer.h"

using namespace std;
//#define CUSTOM_BUFFER //Uncomment it to use custom ring buffer built for this project.
/**
 * @brief Task 1
 * The following function calculates the key points taking input
 * @param keypoints keypoints calculated 
 * @param detectorType Detector type selected for keypoint detection. 
 * @param imgGray Grayscale image on which the key points are claculated. 
 * */
void calcKeyPointsHelper(vector<cv::KeyPoint> &keypoints, string detectorType, cv::Mat &imgGray)
{

    if (detectorType.compare("SHITOMASI") == 0)
    {
        detKeypointsShiTomasi(keypoints, imgGray, false);
    }
    else if (detectorType.compare("HARRIS") == 0)
    {
        detKeypointsHarris(keypoints, imgGray, false);
        //...
    }
    else if (detectorType.compare("FAST") == 0 ||
             detectorType.compare("BRISK") == 0 ||
             detectorType.compare("ORB") == 0 ||
             detectorType.compare("AKAZE") == 0 ||
             detectorType.compare("SIFT") == 0)
    {
        detKeypointsModern(keypoints, imgGray, detectorType, false);
    }
    else
    {
        std::cout << "Invalid Arguments for detector type" << std::endl;
        /* code */
    }
}
/**
 * @brief Task 2 The following function filters the keypoints outside the focus.
 * @param keypoints 
 * 
 * **/
void cropFeatures(vector<cv::KeyPoint> &keypoints)
{
    bool bFocusOnVehicle = true;
    cv::Rect vehicleRect(535, 180, 180, 150);
    if (bFocusOnVehicle)
    {
        vector<cv::KeyPoint> filteredKeyPoints;
        for (auto keypoint : keypoints)
        {
            if (vehicleRect.contains(keypoint.pt))
            {
                filteredKeyPoints.push_back(keypoint);
            }
        }
        keypoints = filteredKeyPoints;

        // ...
    }
}

/**
 * @brief Task 3 The following function limits the keypoints to 50.
 * @param keypoints 
 * 
 * **/

void limitNoOfKeyPoints(vector<cv::KeyPoint> &keypoints, string detectorType)
{
    bool bLimitKpts = false;
    if (bLimitKpts)
    {
        int maxKeypoints = 50;

        if (detectorType.compare("SHITOMASI") == 0)
        { // there is no response info, so keep the first 50 as they are sorted in descending quality order
            keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
        }
        cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
        cout << " NOTE: Keypoints have been limited!" << endl;
    }
}

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{

    /* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    const int dataBufferSize = 2; // no. of images which are held in memory (ring buffer) at the same time

#ifdef CUSTOM_BUFFER
    RingBuffer<DataFrame, dataBufferSize> dataBuffer;
#else
    deque<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
#endif
    bool bVis = false; // visualize results

    /*set up all the design parameters*/
    /*Select detector type below*/
    //string detectorType = "BRISK";
    string detectorType = "FAST";
    // string detectorType = "ORB";
    // string detectorType = "AKAZE";
    // string detectorType = "SIFT";
    // string detectorType = "SHITOMASI";
    //string detectorType = "HARRIS";

    /*Select descriptor type below*/
    //string descriptorType = "BRISK"; // BRIEF, ORB, FREAK, AKAZE, SIFT
     string descriptorType = "BRIEF";
    //string descriptorType = "ORB";
    // string descriptorType = "FREAK";
    // string descriptorType = "AKAZE";
    // string descriptorType = "SIFT";

    /*Select Matcher Type below*/
    string matcherType = "MAT_BF"; // MAT_BF, MAT_FLANN

    /*Select selector type below*/
    string selectorType = "SEL_NN"; // SEL_NN, SEL_KNN

    /* MAIN LOOP OVER ALL IMAGES */

    for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        /* LOAD IMAGE INTO BUFFER */

        // assemble filenames for current index
        ostringstream imgNumber;
        imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
        string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(imgFullFilename);
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        //// STUDENT ASSIGNMENT
        //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize

        // push image into data frame buffer
        DataFrame frame;
        frame.cameraImg = imgGray;

#ifdef CUSTOM_BUFFER
        dataBuffer.Push(frame);
#else
        dataBuffer.push_back(frame);
#endif
        if (dataBuffer.size() > dataBufferSize)
        {

#ifdef CUSTOM_BUFFER
            dataBuffer.PopFront();
#else
            dataBuffer.pop_front();
#endif
        }
        // assert(dataBuffer.size() <= dataBufferSize); //This will report error if the data buffe ris greater than

        //// EOF STUDENT ASSIGNMENT
        cout << "#1 : LOAD IMAGE INTO BUFFER done" << endl;

        /* DETECT IMAGE KEYPOINTS */

        // extract 2D keypoints from current image
        vector<cv::KeyPoint> keypoints; // create empty feature list for current image

        /**
         * TASK MP.2 -> Detecting key points 
         * When i checked with multiple 
         * 
         * */
        double detected_time = (double)cv::getTickCount();
        calcKeyPointsHelper(keypoints, detectorType, imgGray);
        detected_time = ((double)cv::getTickCount() - detected_time) / cv::getTickFrequency();
        // std::cout << "Detecting keypoints using " << detectorType << " took " << detected_time << " seconds" << std::endl;
        
        //// TASK MP.3 -> only keep keypoints on the preceding vehicle

        cropFeatures(keypoints);

        // optional : limit number of keypoints (helpful for debugging and learning)
        limitNoOfKeyPoints(keypoints, detectorType);
        int key_size = keypoints.size();
        std::cout << "Number of keypoints detected using " << detectorType << " is " << keypoints.size() << std::endl;
        // push keypoints and descriptor for current frame to end of data buffer
#ifdef CUSTOM_BUFFER
        dataBuffer.GetEnd(0)->keypoints = keypoints;
#else
        (dataBuffer.end() - 1)->keypoints = keypoints;
#endif
        cout << "#2 : DETECT KEYPOINTS done" << endl;

        /* EXTRACT KEYPOINT DESCRIPTORS */

        //// STUDENT ASSIGNMENT
        //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
        //// -> BRIEF, ORB, FREAK, AKAZE, SIFT

        cv::Mat descriptors;

#ifdef CUSTOM_BUFFER
        descKeypoints(dataBuffer.GetEnd(0)->keypoints, dataBuffer.GetEnd(0)->cameraImg, descriptors, descriptorType);
#else
        descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType);
#endif
        //// EOF STUDENT ASSIGNMENT
#ifdef CUSTOM_BUFFER
        dataBuffer.GetEnd(0)->descriptors = descriptors;
#else
        // push descriptors for current frame to end of data buffer
        (dataBuffer.end() - 1)->descriptors = descriptors;
#endif
        cout << "#3 : EXTRACT DESCRIPTORS done" << endl;

        if (dataBuffer.size() > 1) // wait until at least two images have been processed
        {

            /* MATCH KEYPOINT DESCRIPTORS */

            vector<cv::DMatch> matches;

            //string descriptorType = "DES_BINARY"; // DES_BINARY, DES_HOG

            string descriptorCat;
            if (descriptorType.compare("SIFT") == 0)
            {
                descriptorCat = "DES_HOG";
            }
            else
            {
                descriptorCat = "DES_BINARY";
                /* code */
            }

            //// STUDENT ASSIGNMENT
            //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
            //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp
            double desc_t = (double)cv::getTickCount();
#ifdef CUSTOM_BUFFER
            matchDescriptors(dataBuffer.GetEnd(1)->keypoints, dataBuffer.GetEnd(0)->keypoints,
                             dataBuffer.GetEnd(1)->descriptors, dataBuffer.GetEnd(0)->descriptors,
                             matches, descriptorCat, matcherType, selectorType);
#else
            matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                             (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                             matches, descriptorCat, matcherType, selectorType);
#endif

            desc_t = ((double)cv::getTickCount() - desc_t) / cv::getTickFrequency();

            cout<<detectorType<<",";
            cout<<descriptorType<<",";
            cout<<key_size<<",";
            cout<<matches.size()<<", ";
            cout<<1000*detected_time/1.0<<",";
            cout<<1000*desc_t/1.0<<endl;
            //// EOF STUDENT ASSIGNMENT

            // store matches in current data frame
#ifdef CUSTOM_BUFFER
            dataBuffer.GetEnd(0)->kptMatches = matches;
#else
            (dataBuffer.end() - 1)->kptMatches = matches;
#endif
            cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << endl;

            // visualize matches between current and previous image
            bVis = true;
            if (bVis)
            {
#ifdef CUSTOM_BUFFER
                cv::Mat matchImg = (dataBuffer.GetEnd(0)->cameraImg).clone();
                cv::drawMatches(dataBuffer.GetEnd(1)->cameraImg, dataBuffer.GetEnd(1)->keypoints,
                                dataBuffer.GetEnd(0)->cameraImg, dataBuffer.GetEnd(0)->keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
#else
                cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
#endif
                string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                cout << "Press key to continue to next image" << endl;
                cv::waitKey(0); // wait for key to be pressed
            }
            bVis = false;
        }

    } // eof loop over all images

    return 0;
}
