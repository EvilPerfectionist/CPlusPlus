// The source code is from: http://www.cplusplus.com/doc/tutorial/files/
// With topic: Input/output with files
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main () {
    ofstream WriteFile ("/home/leon/Instrument_Pose_Estimation/Test_C/example.txt");
    if (WriteFile.is_open())
    {
        WriteFile << "This is a line.\n";
        WriteFile << "This is another line.\n";
        WriteFile.close();
    }
    else cout << "Unable to open file";

    string line;
    ifstream ReadFile ("/home/leon/Instrument_Pose_Estimation/Test_C/example.txt");
    if (ReadFile.is_open())
    {
        while ( getline (ReadFile,line) )
        {
            cout << line << '\n';
        }
        ReadFile.close();
    }

    else cout << "Unable to open file";
    return 0;
}
