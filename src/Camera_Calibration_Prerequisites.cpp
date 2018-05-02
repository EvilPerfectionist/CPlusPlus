#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;
using namespace std;

int main()
{
    string filename = "/home/leon/Instrument_Pose_Estimation/Test_C/Images.xml";
    {     //write
        FileStorage fs(filename, FileStorage::WRITE);
        fs << "images" << "[";
        fs << "/home/leon/Instrument_Pose_Estimation/Test_C/photos/ChessBoard_1.jpg"
           << "/home/leon/Instrument_Pose_Estimation/Test_C/photos/ChessBoard_2.jpg"
           << "/home/leon/Instrument_Pose_Estimation/Test_C/photos/ChessBoard_3.jpg"
           << "/home/leon/Instrument_Pose_Estimation/Test_C/photos/ChessBoard_4.jpg"
           << "/home/leon/Instrument_Pose_Estimation/Test_C/photos/ChessBoard_5.jpg"
           << "/home/leon/Instrument_Pose_Estimation/Test_C/photos/ChessBoard_6.jpg";
        fs << "]";

        cout << endl << "Reading: " << endl;
    }
    {
        FileStorage fs;
        fs.open(filename, FileStorage::READ);
        FileNode n = fs["images"];                         // Read string sequence - Get node
        if (n.type() != FileNode::SEQ)
        {
            cerr << "strings is not a sequence! FAIL" << endl;
            return 1;
        }
        FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
        for (; it != it_end; ++it)
            cout << (string)*it << endl;
    }
    return 0;
}