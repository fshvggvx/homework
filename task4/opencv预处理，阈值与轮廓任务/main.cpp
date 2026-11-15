#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main() {
	Mat src = imread("../../../../CMakeProject2/lena.jpg");
	if (src.empty())
		return -1;
	imwrite("../../../../output/src.png", src);
	imshow("src", src);
	waitKey();
	Mat gray, hsv,gray_,hsv_,gray_mask,hsv_mask;
	
	cvtColor(src, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray_, Size(5, 5), 0);
	imwrite("../../../../output/fixed_blur_gray.png",gray_ );
	threshold(gray_, gray_mask, 120,255, THRESH_BINARY_INV);
	imwrite("../../../../output/fixed_mask_gray.png", gray_mask);
	imshow(" gray_mask", gray_mask);

	cvtColor(src, hsv, COLOR_BGR2HSV);
	medianBlur(hsv, hsv_, 5);
	imwrite("../../../../output/fixed_blur_color.png", hsv_);
	inRange(hsv, Scalar(35, 80, 80), Scalar(85, 255, 255), hsv_mask);
	imwrite("../../../../output/fixed_mask_hsv.png", hsv_mask);
	imshow("hsv_mask", hsv_mask);
	waitKey();

	Mat dst1,dst2;
	Mat element3 = getStructuringElement(0, Size(3, 3));
	Mat element7 = getStructuringElement(0, Size(7, 7));
	erode (hsv_mask, dst1, element7);
	dilate(hsv_mask, dst2, element7);
	imwrite("../../../../output/fixed_eroded.png",dst1 );
	imwrite("../../../../output/fixed_dilated.png",dst2 );
	imshow("erosion" , dst1);
	imshow("dilation", dst2);
	waitKey();

	Mat close_dst,open_dst;
	morphologyEx(hsv_mask, open_dst, MORPH_OPEN, element7);
	morphologyEx(hsv_mask, close_dst,MORPH_CLOSE, element7);
	imwrite("../../../../output/fixed_opened.png", open_dst);
	imwrite("../../../../output/fixed_closed.png", close_dst);
	imshow("open" ,open_dst );
	imshow("close",close_dst);
	waitKey();

	vector<vector<Point>>contours;
	findContours(open_dst, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Mat image = src.clone();
	int count = 0;
	for (size_t i = 0;i < contours.size();i++) {
		double area = contourArea(contours[i]);
		if (area < 100)
			continue;
		Rect rect = boundingRect(contours[i]);
		rectangle(image, rect, Scalar(0, 0, 255));
		RotatedRect rect1 = minAreaRect(contours[i]);
		Point2f pts[4];

		rect1.points(pts);
		for (int k = 0;k < 4;k++)
			line(image, pts[k], pts[(k + 1) % 4], Scalar(255, 0, 0));
		
		Moments m = moments(contours[i]);
		double cx = m.m10 / m.m00;
		double cy = m.m01 / m.m00;
		circle(image, Point(cvRound(cx), cvRound(cy)), 4, Scalar(0, 0, 0), -1);
		count++;
		cout << "\n第" << count << "个图形：\n" << "面积：" << area << "\n中心坐标：" << cx << "," << cy;
	}
	imwrite("../../../../output/fixed_result.png", image);
	cout << "\n共" << count << "个图形\n";
	imshow("1", image);
	waitKey();

}

