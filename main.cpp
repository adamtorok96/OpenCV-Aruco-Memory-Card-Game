#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "CardGame.h"

#define WINDOW_NAME "CardGame"

using namespace std;
using namespace cv;

static bool readCameraParameters(const string &filename, Mat &camMatrix, Mat &distCoeffs);

int main() {
    VideoCapture vc;

    if( !vc.open(0) ) {
        cerr << "Failed to open camera (0)!";

        return -1;
    }

    Mat camMatrix, distCoeffs;

    bool readOk = readCameraParameters("cam2.calib", camMatrix, distCoeffs);

    if( !readOk ) {
        cerr << "Invalid camera file" << endl;
        return -1;
    }

    std::vector<Mat> cardImages;

    Mat i = imread("mem1.jpg");
    resize(i, i, Size(100, 100));

    cardImages.push_back(i);

    CardGame cg(1, cardImages);
    cg.initCamera(camMatrix, distCoeffs);
    cg.createMapping();

    Mat frame;

    while(vc.grab()) {
        vc >> frame;

        cg.handleFrame(frame);

        imshow(WINDOW_NAME, frame);

        char key = (char)waitKey(10);

        if( key == 27 )
            break;
    }

    vc.release();

    return 0;
}

static bool readCameraParameters(const string &filename, Mat &camMatrix, Mat &distCoeffs) {
    FileStorage fs(filename, FileStorage::READ);

    if(!fs.isOpened())
        return false;

    fs["camera_matrix"] >> camMatrix;
    fs["distortion_coefficients"] >> distCoeffs;

    return true;
}