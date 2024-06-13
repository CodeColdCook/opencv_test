#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

int main() {
    // 定义图像路径和文件夹路径
    std::string folderPath = "your_folder_path"; // 替换为实际文件夹路径
    std::vector<cv::String> imagePaths;

    // 使用 glob 函数获取文件夹下所有图像文件的路径
    cv::glob(folderPath, imagePaths);

    // 对图像路径按名称进行排序
    std::sort(imagePaths.begin(), imagePaths.end());

    // 读取每个图像并垂直裁切为四个图像
    for (const auto& imagePath : imagePaths) {
        cv::Mat image = cv::imread(imagePath);

        if (image.empty()) {
            std::cerr << "Error: Unable to read image " << imagePath << std::endl;
            continue;
        }

        // 垂直裁切为四个图像
        std::vector<cv::Mat> croppedImages;
        int height = image.rows / 4;
        for (int i = 0; i < 4; ++i) {
            cv::Rect roi(0, i * height, image.cols, height);
            croppedImages.push_back(image(roi).clone());
        }

        // 保存裁切后的图像
        for (int i = 0; i < 4; ++i) {
            std::string croppedImagePath = "cropped_image_" + std::to_string(i) + ".jpg";
            cv::imwrite(croppedImagePath, croppedImages[i]);
            std::cout << "Saved cropped image: " << croppedImagePath << std::endl;
        }
    }

    return 0;
}
