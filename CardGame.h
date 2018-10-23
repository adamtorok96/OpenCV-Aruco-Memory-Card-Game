//
// Created by edems on 2018.10.23..
//

#ifndef OPENCVMEMORYCARDGAME_CARDGAME_H
#define OPENCVMEMORYCARDGAME_CARDGAME_H

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
using namespace aruco;

class CardGame {
    Ptr<DetectorParameters> detectorParams;
    Ptr<Dictionary> dictionary;

    Mat camMatrix, distCoeffs;

    float markerLength;
    unsigned int dictionaryId;

    unsigned int nPairs;
    unsigned int cardImageWidth, cardImageHeight;

    vector<Mat> cardImages;
    vector<unsigned int> found;
    vector<unsigned int> idMapToImage;

    vector<Point3f> objPoints;
    vector<Point2f> cardCorners;

    void setObjPoints();
    void setCardCorners();
    void drawCard(Mat & frame, unsigned int id, Vec3d & rvec, Vec3d & tvec);

    Mat & getCardImageById(unsigned int id);

    bool isValidId(unsigned int id);
    bool isFound(unsigned int id);
public:
    CardGame(unsigned int nPairs, vector<Mat> & images);

    void initCamera(Mat & camMatrix, Mat & distCoeffs);
    void createMapping();

    void setMarkerLength(float length);
    void setDictionary(unsigned int id);

    void handleFrame(Mat & frame);
    void reset();
};


#endif //OPENCVMEMORYCARDGAME_CARDGAME_H
