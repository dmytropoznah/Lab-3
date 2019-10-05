#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat erosion_dst;
Mat dilation_dst;
Mat src;

int main(int argc, char** argv)
{

	src = imread("D:/image.jpg"); 
	imshow("original", src);

	Mat dst;
	Mat map_x, map_y;
	dst.create(src.size(), src.type());
	map_x.create(src.size(), CV_32F);
	map_y.create(src.size(), CV_32F);

	int w = src.cols;
	int h = src.rows;

	const int K = 4;
	cv::Point2f srcCoords[K];
	cv::Point2f dstCoords[K]; 

	srcCoords[0] = cv::Point2f(292, 216);
	srcCoords[1] = cv::Point2f(647, 383);
	srcCoords[2] = cv::Point2f(396, 884);
	srcCoords[3] = cv::Point2f(45, 707);

	dstCoords[0] = cv::Point2f(0, 0);
	dstCoords[1] = cv::Point2f(w, 0);
	dstCoords[2] = cv::Point2f(w, h);
	dstCoords[3] = cv::Point2f(0, h);

	Mat imageResult;

	Mat transform = getPerspectiveTransform(srcCoords, dstCoords);
	warpPerspective(src, imageResult, transform,
		cv::Size(w, h), INTER_LINEAR); 

	imshow("Result", imageResult);

	vector<Point2f> sv(srcCoords, srcCoords + sizeof srcCoords / sizeof srcCoords[0]);
	vector<Point2f> dv(dstCoords, dstCoords + sizeof dstCoords / sizeof dstCoords[0]);
	Mat homographyMatrix = findHomography(dv, sv, RANSAC); 
	warpPerspective(imageResult, dst, homographyMatrix, dst.size()); 
	imshow("remap", dst);


	Mat kernel = getStructuringElement(MORPH_CROSS, Size(17, 17));
	erode(src, erosion_dst, kernel);
	imshow("Erosion", erosion_dst);
	dilate(src, dilation_dst, kernel);
	imshow("Dilation", dilation_dst);

	Mat ex, cl;
	morphologyEx(src, ex, MORPH_OPEN, kernel);
	morphologyEx(src, cl, MORPH_CLOSE, kernel);
	imshow("Opening", ex);
	imshow("Closure", cl);


	Mat outputBlur;
	String filePrefix = "img";
	String fileSuffix = ".bmp";
	int count = 0;
	for (int i = 1; i < 51; i += 2) {
		medianBlur(src, outputBlur, i);
		String filename = filePrefix + fileSuffix;
		imwrite(filePrefix + to_string(count) + fileSuffix, outputBlur);
		count++;
	}
	waitKey(0);
	return 0;
}
