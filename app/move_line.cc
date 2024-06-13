#include <chrono>
#include <opencv2/opencv.hpp>
#include <thread>

#define ROWS 300
#define COLS 1920

int main() {
  // 定义绿色和黄色的线条颜色
  cv::Scalar green(0, 255, 0);
  cv::Scalar yellow(0, 255, 255);

  // 定义线条的起始位置和移动步长
  int green_y = 0;
  int yellow_y = 100;
  int step = 1;
  cv::namedWindow("0");
  cv::namedWindow("1");
  cv::namedWindow("2");
  cv::namedWindow("3");

  // 显示图像并动态移动线条
  while (true) {
    // 创建一个黑色背景的图像
    cv::Mat image(ROWS, COLS, CV_8UC3, cv::Scalar(0, 0, 0));
    // 绘制新的线条
    cv::line(image, cv::Point(0, green_y), cv::Point(image.cols - 1, green_y), green, 2);
    cv::line(image, cv::Point(0, yellow_y), cv::Point(image.cols - 1, yellow_y), yellow, 2);

    std::vector<cv::Mat> croppedImages;
    int width = image.cols / 4;
    for (int i = 0; i < 4; i++) {
      cv::Rect roi(i * width, 0, width, image.rows);
      croppedImages.push_back(image(roi).clone());
      cv::imshow(std::to_string(i), croppedImages[i]);
    }

    // 显示图像
    cv::imshow("Image", image);
    cv::waitKey(1);

    // 移动线条
    green_y += step;
    yellow_y += step;
    if (green_y > ROWS) green_y -= ROWS;
    if (yellow_y > ROWS) yellow_y -= ROWS;

    // 暂停0.1秒
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  cv::destroyAllWindows();

  return 0;
}
