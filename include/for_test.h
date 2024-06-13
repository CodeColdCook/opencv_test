#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

void bbox_rect(cv::Rect2d &bbox ,const cv::Rect2d &bbox_max);