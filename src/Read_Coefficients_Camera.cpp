# include <Eigen/Core>
#include <Eigen/Dense>
# include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;
using namespace std ;
using namespace Eigen ;

int main()
{
    char file[200];
    Mat cameraMatrix, distCoeffs;
    sprintf(file, "%s/intrinsics.xml", "/home/leon/Instrument_Pose_Estimation/Test_C");
    cv::FileStorage fs(file, cv::FileStorage::READ);
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    fs.release();

    cout << "cameraMatrix\n" << cameraMatrix << endl;
    //cout << "distCoeffs\n" << distCoeffs << endl;

    Matrix4f ProjectionMatrix;
    Matrix3f K, Kinv;
    float n = 0.01; // near field
    float f = 1000; // far field

    ProjectionMatrix << cameraMatrix.at<double>(0, 0) / cameraMatrix.at<double>(0, 2), 0.0, 0.0, 0.0,
            0.0, cameraMatrix.at<double>(1, 1) / cameraMatrix.at<double>(1, 2), 0.0, 0.0,
            0.0, 0.0, -(f + n) / (f - n), (-2.0f * f * n) / (f - n),
            0.0, 0.0, -1.0, 0.0;
    cout << "ProjectionMatrix\n" << ProjectionMatrix << endl;
    K << cameraMatrix.at<double>(0, 0), cameraMatrix.at<double>(0, 1), cameraMatrix.at<double>(0, 2),
            cameraMatrix.at<double>(1, 0), cameraMatrix.at<double>(1, 1), cameraMatrix.at<double>(1, 2),
            cameraMatrix.at<double>(2, 0), cameraMatrix.at<double>(2, 1), cameraMatrix.at<double>(2, 2);
    cout << "K\n" << K << endl;
    Kinv = K.inverse();
}