#include "detect_app.h"

using namespace std;
using namespace cv;

void DetectApp::ResultCallback(std::string &result)
{
  cout << "result: " << result << endl;
}


// void DetectApp::ResultCallback()
// {
//   cout << "result: " << "test" << endl;
// }