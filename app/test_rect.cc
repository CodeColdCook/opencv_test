#include <iostream>

#include "for_test.h"

using namespace std;
using namespace cv;

bool if_test_rect = false;
bool if_test_bboxPadding = false;
bool if_test_fillter_box = true;

cv::Rect2d BboxPadding(cv::Rect2d bbox_to_pad, int padding_pixel);
std::vector<cv::Rect> fillterBoxs(std::vector<cv::Rect> rects);
bool expand_and_find(const vector<cv::Rect> rects_, Rect &max_box_);
Rect find_max_box(const vector<cv::Rect> rects_);
vector<cv::Rect> find_contours(const Mat &image);
bool fillter_bbox_expand(const vector<cv::Rect> &rects_src,
                         vector<cv::Rect> &rects_fillterd, Rect &max_box_);
bool fillter_bbox_expand_with_bbox(const vector<cv::Rect> &rects_src,
                                   vector<cv::Rect> &rects_fillterd,
                                   Rect &max_box_);
void fillter_back_bbox(vector<cv::Rect> &rects_);
bool fillter_empty_area(const Mat &img, Rect &rect_);
int count_area_nums(const Mat &img, Rect rect);
vector<cv::Rect> detect_rect(Mat image_);
void process(Mat image_);

string path_image_buoy = "/home/mchris/test/opencv_test/buoy/frame0060.jpg";
string name_window = "test";
string result_window = "result";
string process_window = "process";
vector<Rect> rect_process;
vector<Rect> rect_process_before;
vector<pair<Rect, int>> obs_process;

void Crop();

void ReadYaml();

int main() {
  // Crop();
  ReadYaml();
  return 0;
  // test the expanding box
  /*
  for(int i=10; i<70; i++)
  {
      string path = "/home/mchris/test/opencv_test/image3/frame00" +
  to_string(i) + ".jpg"; Mat image_src = imread(path,CV_LOAD_IMAGE_GRAYSCALE);
      process(image_src);
  }

  return 0;
  */
}

void ReadYaml() {
  cv::FileStorage fs;
  std::string config_path =
      "/home/mchris/workspace_test/opencv_test/data/cfg.yaml";
  fs.open(config_path, cv::FileStorage::READ);
  if (!fs.isOpened()) {
    fs.release();
  }
  cv::FileNode cfg_node = fs["pmtd_usv_fusion"];
  std::vector<int> device_indexs;
  cfg_node["device_indexs"] >> device_indexs;
  for (int i : device_indexs) {
    cout << i << endl;
  }
  fs.release();
}

void Crop() {
  cv::Mat img_src = imread(
      "/home/mchris/workspace_docker_all/pmtd4/pmtd_color_cloud/asset/"
      "image_left.jpg");
  cv::Mat crop = img_src(Range(0, 1079), Range(5, 1914));
  cv::resize(img_src, img_src, cv::Size(640, 640));
  cv::imshow("crop", crop);
  cv::waitKey(0);
}

void process(Mat image_) {
  vector<cv::Rect> rects_detect = detect_rect(image_);
  if (rect_process.size() < 1)
    rect_process = rects_detect;
  else {
    // track
    for (auto &box : rect_process) {
      box = BboxPadding(box, 10);
      if (!fillter_empty_area(image_, box)) continue;
      vector<cv::Rect> rects_detect_filltered;
      fillter_bbox_expand_with_bbox(rects_detect, rects_detect_filltered, box);
      rects_detect = rects_detect_filltered;
      rects_detect.push_back(box);
    }
    rect_process = rects_detect;
  }
  for (auto &box : rect_process) fillter_empty_area(image_, box);
  for (const auto &box : rect_process) rectangle(image_, box, Scalar(150), 1.0);
  imshow(result_window, image_);
  waitKey(0);
}

vector<cv::Rect> detect_rect(Mat image_) {
  Mat image_dilate = image_.clone();
  Mat cur = image_.clone();
  Mat Kernel_test_3_3 = (cv::Mat_<double>(3, 3) << 1, 1, 1, 1, 9, 1, 1, 1, 1);
  filter2D(image_dilate, image_dilate, image_dilate.depth(), Kernel_test_3_3);

  Mat structureElement_dilate =
      cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));
  dilate(image_dilate, cur, structureElement_dilate);
  vector<cv::Rect> rects = find_contours(image_dilate);
  vector<cv::Rect> results, rects_fillterd, rects_cur;
  rects_cur = rects;
  while (true) {
    Rect box_max;
    if (fillter_bbox_expand(rects_cur, rects_fillterd, box_max)) {
      results.push_back(box_max);
      rects_cur = rects_fillterd;
      rects_fillterd.clear();
    } else
      break;
  }
  return results;
}

void fillter_back_bbox(vector<cv::Rect> &rects_) {
  //    if(rects_.size() < 2)
  //        return;
  //    for(size_t i=0; i<rects_.size()-1;i++)
  //    {
  //        Rect rect_1 = rects_[i];
  //        int row_min = rect_1.y;
  //        int row_max = rect_1.y+rect_1.height;
  //        int col_min = rect_1.x;
  //        int col_max = rect_1.x+rect_1.width;
  //        for (size_t j=1; i<rects_.size();j++)
  //        {
  //            if()
  //            {

  //            }
  //        }
  //    }
}
bool fillter_bbox_expand_with_bbox(const vector<cv::Rect> &rects_src,
                                   vector<cv::Rect> &rects_fillterd,
                                   Rect &max_box_) {
  while (true)
    if (expand_and_find(rects_src, max_box_) == false) {
      max_box_ = BboxPadding(max_box_, -10);
      break;
    }
  for (const auto &box : rects_src) {
    int area = box.width * box.height;
    Rect r_and = max_box_ & box;
    int area_and = r_and.width * r_and.height;
    if (area == area_and)
      continue;
    else
      rects_fillterd.push_back(box);
  }
  if (rects_fillterd.size() == rects_src.size()) return false;
  return true;
}

bool fillter_bbox_expand(const vector<cv::Rect> &rects_src,
                         vector<cv::Rect> &rects_fillterd, Rect &max_box_) {
  Rect max_box = find_max_box(rects_src);
  while (true)
    if (expand_and_find(rects_src, max_box) == false) {
      max_box = BboxPadding(max_box, -10);
      break;
    }
  for (const auto &box : rects_src) {
    int area = box.width * box.height;
    Rect r_and = max_box & box;
    int area_and = r_and.width * r_and.height;
    if (area == area_and)
      continue;
    else
      rects_fillterd.push_back(box);
  }
  max_box_ = max_box;
  if (rects_fillterd.size() == rects_src.size()) return false;
  return true;
}

cv::Rect2d BboxPadding(cv::Rect2d bbox_to_pad, int padding_pixel) {
  return (cv::Rect2d(bbox_to_pad.x - padding_pixel,
                     bbox_to_pad.y - padding_pixel,
                     bbox_to_pad.width + 2 * padding_pixel,
                     bbox_to_pad.height + 2 * padding_pixel));
}

vector<cv::Rect> find_contours(const Mat &image) {
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hireachy;
  std::vector<cv::Rect> rects_;
  cv::findContours(image, contours, hireachy, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
  if (contours.size() > 0) {
    for (size_t i = 0; i < contours.size(); i++) {
      if (contours[i].size() > 10) {
        cv::Rect rect = boundingRect(contours[i]);
        rect = BboxPadding(rect, 10);
        rects_.push_back(rect);
      }
    }
  }
  return rects_;
}

Rect find_max_box(const vector<cv::Rect> rects_) {
  Rect max_box_;
  if (rects_.size() > 0) {
    max_box_ = rects_[0];
    for (const auto &box : rects_) {
      if (box.width * box.height > max_box_.width * max_box_.height)
        max_box_ = box;
    }
  }
  return max_box_;
}

bool expand_and_find(const vector<cv::Rect> rects_, Rect &max_box_) {
  max_box_ = BboxPadding(max_box_, 10);
  int area = max_box_.width * max_box_.height;
  for (const auto &box : rects_) {
    Rect r_and = box & max_box_;
    if (r_and.width * r_and.height != 0) {
      max_box_ = box | max_box_;
    }
  }
  if (area == max_box_.width * max_box_.height) return false;
  return true;
}

bool fillter_empty_area(const Mat &img, Rect &rect_) {
  Rect max_box = rect_;
  int x_min, y_min, x_max, y_max, nums_count = 0;
  x_min = y_min = 100000;
  x_max = y_max = 0;

  for (int j = max_box.y; j <= (max_box.y + max_box.height); j += 1) {
    for (int i = max_box.x; i <= (max_box.x + max_box.width); i++) {
      if (int(img.at<uchar>(j, i)) < 10) continue;
      nums_count++;
      x_min = std::min(i, x_min);
      x_max = std::max(i, x_max);
      y_min = std::min(j, y_min);
      y_max = std::max(j, y_max);
    }
  }
  if (x_min > 9999) return false;
  if (rect_.width < 30 || rect_.height < 30) {
    cout << "nums_count: " << nums_count << endl;
    if (nums_count > 5) return true;
    return false;
  }
  max_box.x = x_min;
  max_box.y = y_min;
  max_box.width = x_max - x_min;
  max_box.height = y_max - y_min;
  rect_ = max_box;
  return true;
}

int count_area_nums(const Mat &img, Rect rect) {}

std::vector<cv::Rect> fillterBoxs(std::vector<cv::Rect> rects) {
  // include, cross
  for (size_t i = 0; i < rects.size() - 1; i++) {
    cv::Rect r1 = rects[i];
    if (r1.width == 0 || r1.width == 0) continue;
    for (size_t j = i + 1; j < rects.size(); j++) {
      cv::Rect r2 = rects[j];
      // have common
      cv::Rect r = r1 & r2;
      std::cout << "rect common: " << r << std::endl;
      if (r.width < 3 || r.height < 3)  // no common
        continue;

      size_t index_big, index_small;
      if (r1.width * r1.height > r2.width * r2.height)  // find biger one
      {
        index_big = i;
        index_small = j;
      } else {
        index_big = j;
        index_small = i;
      }

      cv::Rect r_and = r1 & r2;
      std::cout << "rect small: " << rects[index_small] << std::endl;
      std::cout << "r.width * r.height: " << r.width * r.height << std::endl;
      std::cout << "rects[index_small].width * rects[index_small].height: "
                << rects[index_small].width * rects[index_small].height
                << std::endl;
      float a = static_cast<float>(r.width * r.height) /
                static_cast<float>(rects[index_small].width *
                                   rects[index_small].height);
      std::cout << "common rate: " << a << std::endl;
      if (a > 0) {
        r1 = r1 | r2;
        rects[index_big] = r1;
        rects[index_small].width = rects[index_small].height = 0;
      }
    }
  }
  std::vector<cv::Rect> results;
  for (size_t i = 0; i < rects.size(); i++) {
    cv::Rect r = rects[i];
    if (r.width == 0 || r.height == 0) continue;
    results.push_back(r);
  }
  std::cout << "fillter down size: " << rects.size() - results.size()
            << std::endl;
  return results;
}
