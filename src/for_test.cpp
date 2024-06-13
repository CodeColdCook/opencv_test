#include "for_test.h"
#include <iostream>

using namespace std;
using namespace cv;

void bbox_rect(cv::Rect2d &bbox, const cv::Rect2d &bbox_max)
{
  cout << bbox << endl;
  cout << bbox_max << endl;
  bbox = bbox & bbox_max;
  cout << "result: " << bbox << endl;
}