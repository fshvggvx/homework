#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int H_min=0;
int H_max=33;
int S_min=102;
int S_max=255;
int V_min=134;
int V_max=255;
Mat hsv, hsv_mask;
void trackbar(int, void*);
int main() {
	Mat src = imread("../../../../images/outpost_frame.png");
	if (src.empty())
		return -1;
	imshow("src", src);
	waitKey();
	cvtColor(src, hsv, COLOR_BGR2HSV);
	medianBlur(hsv, hsv, 3);
	namedWindow("inrange");
     createTrackbar("H_min", "inrange", &H_min, 179, trackbar);
     createTrackbar("H_max", "inrange", &H_max, 179, trackbar);
     createTrackbar("S_min", "inrange", &S_min, 255, trackbar);
     createTrackbar("S_max", "inrange", &S_max, 255, trackbar);
     createTrackbar("V_min", "inrange", &V_min, 255, trackbar);
     createTrackbar("V_max", "inrange", &V_max, 255, trackbar);
     trackbar(0, 0);
     waitKey();
	 imwrite("../../../../output/hsv_mask.jpg", hsv_mask);
	vector<vector<Point>>contours;
	vector<Point2f>points;
	findContours(hsv_mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0;i < contours.size();i++) {
		Rect box = boundingRect(contours[i]);
		float a = box.height;
		float b = box.width;

		if ((a / b) > 3&&a*b>200)
		{
			Point2f pts[4];
			pts[0] =Point(box.x           ,box.y);
			pts[1] =Point(box.x+box.width ,box.y);
			pts[2] =Point(box.x+box.width ,box.y+ box.height);
			pts[3] = Point(box.x, box.y + box.height);
			
			for (int k = 0;k < 4;k++) {
				line(src, pts[k], pts[(k + 1) % 4], Scalar(0, 255, 0));
				points.push_back(pts[k]);
			}
		}
	}

	if (points.size() == 8) {
		cout << "结果合法"<<endl;
		RotatedRect box = minAreaRect(points);
		Point2f pts[4];
		box.points(pts);
		cout << "四个点坐标：" << endl;
		for (int k = 0;k < 4;k++) {
			line(src, pts[k], pts[(k + 1) % 4], Scalar(255, 0, 0));
			cout << pts[k] << endl;
		}
		cout << "装甲中心：\n" << box.center << endl;

		circle(src, box.center, 5, Scalar(0, 0, 255),-1);
		imshow("dst", src);
		imwrite("../../../../output/dst.jpg", src);
		waitKey();
	}

}
void trackbar(int, void*) {
	inRange(hsv, Scalar(H_min, S_min, V_min), Scalar(H_max, S_max, V_max), hsv_mask);
	imshow("inrange", hsv_mask);

}
