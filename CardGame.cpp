//
// Created by edems on 2018.10.23..
//

#include "CardGame.h"

CardGame::CardGame(unsigned int nPairs, vector<Mat> &images) {
    this->markerLength = 0.05f;
    this->dictionaryId = 0;

    this->nPairs = nPairs;
    this->cardImages = images;

    this->cardImageWidth = (unsigned int)images[0].cols;
    this->cardImageHeight = (unsigned int)images[0].rows;

    this->detectorParams = DetectorParameters::create();
    this->dictionary = getPredefinedDictionary(PREDEFINED_DICTIONARY_NAME(this->dictionaryId));

    this->setObjPoints();
    this->setCardCorners();
}

void CardGame::initCamera(Mat &camMatrix, Mat &distCoeffs) {
    this->camMatrix = camMatrix;
    this->distCoeffs = distCoeffs;
}

void CardGame::createMapping() {
    srand (static_cast<unsigned int>(time(nullptr)));

    vector<unsigned int> pool;

    for(unsigned int i = 0; i < nPairs; i++) {
        pool.push_back(i);
        pool.push_back(i);
    }

    cout << "Mapping:" << endl;

    for(auto i = 0; i < this->nPairs * 2; i++) {
        auto n = rand() % pool.size();

        cout << "\t" << i << " --> " << pool[n] << endl;

        this->idMapToImage.push_back(pool[n]);
        pool.erase(pool.begin() + n);
    }


}

void CardGame::setMarkerLength(float length) {
    this->markerLength = length;
    this->setObjPoints();
}

void CardGame::setDictionary(unsigned int id) {
    this->dictionaryId = id;
    this->dictionary = getPredefinedDictionary(PREDEFINED_DICTIONARY_NAME(this->dictionaryId));
}

void CardGame::handleFrame(Mat &frame) {
    vector<int> ids;
    vector<vector<Point2f>> corners, rejected;
    vector<Vec3d> rvecs, tvecs;

    detectMarkers(frame, dictionary, corners, ids, detectorParams, rejected);

    if( !ids.empty() ) {
        estimatePoseSingleMarkers(
                corners,
                markerLength,
                camMatrix,
                distCoeffs,
                rvecs,
                tvecs
        );

        //drawDetectedMarkers(frame, corners, ids);

        for(unsigned int i = 0; i < ids.size(); i++) {
//            drawAxis(
//                    frame,
//                    camMatrix,
//                    distCoeffs,
//                    rvecs[i],
//                    tvecs[i],
//                    markerLength * 0.5f
//            );

            if( isValidId(ids[i]) ) {
                this->drawCard(frame, ids[i], rvecs[i], tvecs[i]);
            }
        }
    }
}

void CardGame::reset() {
    found.clear();

    this->createMapping();
}

void CardGame::drawCard(Mat &frame, unsigned int id, Vec3d &rvec, Vec3d &tvec) {
    vector<Point2f> imagePoints;
    vector<Point2i> pts;

    Mat warpedImg;

    projectPoints(objPoints, rvec, tvec, camMatrix, distCoeffs, imagePoints);

    cv::Mat T = getPerspectiveTransform(cardCorners, imagePoints);

    warpPerspective(getCardImageById(id), warpedImg, T, frame.size());

    for(auto i : imagePoints) {
        pts.emplace_back((int)i.x, (int)i.y);
    }

    fillConvexPoly(frame, pts, Scalar::all(0), LINE_AA);

    bitwise_or(warpedImg, frame, frame);
}

void CardGame::setObjPoints() {
    float l = markerLength * 0.5f;

    objPoints.clear();

    objPoints.emplace_back(-l, l, 0);
    objPoints.emplace_back(l, l, 0);
    objPoints.emplace_back(l, -l, 0);
    objPoints.emplace_back(-l, -l, 0);
}

void CardGame::setCardCorners() {
    cardCorners.clear();

    cardCorners.emplace_back(0, 0);
    cardCorners.emplace_back(cardImageWidth, 0);
    cardCorners.emplace_back(cardImageWidth, cardImageHeight);
    cardCorners.emplace_back(0, cardImageHeight);
}

Mat &CardGame::getCardImageById(unsigned int id) {
    return cardImages[idMapToImage[id]];
}

bool CardGame::isFound(unsigned int id) {
    for(auto i = 0; i < found.size(); i++) {
        if( found[i] == id )
            return true;
    }

    return false;
}

bool CardGame::isValidId(unsigned int id) {
    return id < nPairs * 2;
}