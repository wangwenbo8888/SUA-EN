#pragma once

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <QImage>

using namespace cv;

void canny(const std::string& file);

void getEdge(const std::string& file, Mat& edge);

void getEdgeFromIp1Image(IplImage* image, CvSeq* seq);

void getImageEdge(const IplImage* image, Mat& mat);

void DrawBox(const CvBox2D& box, IplImage* img);

void imageRotate(const std::string& file);

void match(const std::string& src);

void imagePreprocess(const std::string& file,Mat& mat);

void imagePreprocess(const QImage& image,Mat& mat);

cv::Mat QImageToMat(const QImage& image);
