#-*- coding:utf-8 –*-
# license removed for brevity


import math
import sys
import cv2
import numpy as np
import matplotlib.pyplot as plt
import yaml

# 欧拉角 (r, p, y) 转换为旋转矩阵
def eulerAnglesToRotationMatrix(theta) :
    
    R_x = np.array([[1,         0,                  0                   ],
                    [0,         math.cos(theta[0]), -math.sin(theta[0]) ],
                    [0,         math.sin(theta[0]), math.cos(theta[0])  ]
                    ])
                    
    R_y = np.array([[math.cos(theta[1]),    0,      math.sin(theta[1])  ],
                    [0,                     1,      0                   ],
                    [-math.sin(theta[1]),   0,      math.cos(theta[1])  ]
                    ])
                
    R_z = np.array([[math.cos(theta[2]),    -math.sin(theta[2]),    0],
                    [math.sin(theta[2]),    math.cos(theta[2]),     0],
                    [0,                     0,                      1]
                    ])        
                    
    R = np.dot(R_z, np.dot( R_y, R_x ))

    return R

# gazebo世界坐标系下的3D坐标投影为像素平面下的2D坐标
def project(bbx_3d):
    bbx_3d = np.array(bbx_3d)
    imagePoints, _ = cv2.projectPoints(bbx_3d, rvecs, tvecs, camera_Matrix)

    # 挑选2D坐标位于左上角和右下角的点，用于后续画图
    x_max = x_min = imagePoints[0][0][0]
    y_max = y_min = imagePoints[0][0][1]
    for num in imagePoints:
        n = num[0]        
        x_max = max(n[0], x_max)
        x_min = min(n[0], x_min)
        y_max = max(n[1], y_max)
        y_min = min(n[1], y_min)

    x_max = int(x_max)
    x_min = int(x_min)
    y_max = int(y_max)
    y_min = int(y_min)

    bbx_2d = [(x_min, y_min), (x_max, y_max)]

    return bbx_2d

# params init
# 从 rostopic：/camera_info 读取到的相机内参
cameraMatrix = np.array([[1144.0874006434067, 0.0, 960.5], [0.0, 1144.0874006434067, 540.5], [0.0, 0.0, 1.0]])

# 从 gazebo 发出的 tf 获得相机的 (x, y, z) pose
p_c_in_w = np.array([0.000, 0.000, 7.591])
# 从 gazebo 发出的 tf 获得相机的 (r, p, y) pose
rpy = np.array([0, 0.349, 0])

# 将 rpy 转化为 旋转矩阵
r_mat = eulerAnglesToRotationMatrix(rpy)
# 将 旋转矩阵 转化为 旋转向量
rvecs = cv2.Rodrigues(r_mat)[0]
# 计算平移向量
tvecs = -r_mat @ p_c_in_w

if __name__ == "__main__":
    # 读取车辆在gazebo中的 3D bounding-box 座标
    bbx_3d = np.loadtxt("/home/mchris/test/opencv_test/data/example.txt", dtype=float)

    # 使用 camera_matrix, rvecs, tvecs 对车辆3D-bbx进行投影
    bbx_2d = project(bbx_3d)

    # read the image and draw 2D bbox on it
    cv_img = cv2.imread("/home/mchris/test/opencv_test/data/lADPDiCpt8vhhy_NBDjNB4A_1920_1080.jpeg")
    img = cv2.rectangle(cv_img, bbx_2d[0], bbx_2d[1], (255, 0, 0))

    plt.imshow(img)
    plt.axis("off")
    plt.show()
