#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;
using namespace cv;
int H_min=0;
int H_max=33;
int S_min=102;
int S_max=255;
int V_min=134;
int V_max=255;

int main() {
	VideoCapture cap("../../../../images/outpost.avi");
	if (!cap.isOpened()) {
		cout << "无法打开视频" << endl;
		return -1;
	}
	int fourcc = VideoWriter::fourcc('M', 'J', 'P', 'G');
	double fps = cap.get(CAP_PROP_FPS);
	cout << fps << endl;
	Size sz(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));
	VideoWriter writer("../../../../output/result.avi",fourcc,fps,sz);
	if (!writer.isOpened()) {
		cout << "无法写入视频" << endl;
		return -1;
	}
	namedWindow("1");
	createTrackbar("H_min", "1", &H_min, 179);
	createTrackbar("H_max", "1", &H_max, 179);
	createTrackbar("S_min", "1", &S_min, 255);
	createTrackbar("S_max", "1", &S_max, 255);
	createTrackbar("V_min", "1", &V_min, 255);
	createTrackbar("V_max", "1", &V_max, 255);
	vector<Point3f>objPoints = {
		Point3f(-0.0675,-0.028,  0),
		Point3f(0.0675,- 0.028, 0),
		Point3f(0.0675, 0.028, 0),
		Point3f(-0.0675, 0.028, 0)

	};
	Mat cameraMatrix = (Mat_<double>(3, 3)
		<< 1781.053454, 0, 510.5751,
		0, 1781.686873, 472.9735,
		0, 0, 1);
	Mat distCoeffs= (Mat_<double>(1, 5)
		<< -0.04420740074,-0.10815,-0.0006506219039,0.0002,0);
	
	ofstream ofs("../../../../output/result.txt");
	if (!ofs.is_open())
		cout << "无法打开" << endl;
	int frame_id = 0;
	int Z_rough_count = 0;
	while (1) {
		Mat src, hsv, hsv_mask, hsv_color;
		cap >> src;
		if (src.empty()) {
			cout << "图片为空" << endl;
			break;
		}

		frame_id++;
		double reprojection_error_px = 0;
		Mat rvec(Size(1,3),CV_64FC1), tvec(Size(1, 3), CV_64FC1);
		bool detect_valid = 0;
		bool pnp_valid = 0;
		double tx_m = 0,
			ty_m = 0,
			tz_m = 0,
			rx_m = 0,
			ry_m = 0,
			rz_m = 0,
			distance_m = 0;
		double time_s = frame_id / fps;
		float p;

		cvtColor(src, hsv, COLOR_BGR2HSV);
		medianBlur(hsv, hsv, 3);
		inRange(hsv, Scalar(H_min, S_min, V_min), Scalar(H_max, S_max, V_max), hsv_mask);
		vector<vector<Point>>contours;
		vector<Point2f>points;
		findContours(hsv_mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		for (int i = 0;i < contours.size();i++) {
			Rect box = boundingRect(contours[i]);
			float a = box.height;
			float b = box.width;

			if ((a / b) > 2 && a *b> 50)
			{
				Point2f pts[4];
				pts[0] = Point(box.x, box.y);
				pts[1] = Point(box.x + box.width, box.y);
				pts[2] = Point(box.x + box.width, box.y + box.height);
				pts[3] = Point(box.x, box.y + box.height);

				for (int k = 0;k < 4;k++) {
					line(src, pts[k], pts[(k + 1) % 4], Scalar(0, 0, 255));
					points.push_back(pts[k]);
				}
			}
		}
		if (points.size() > 8) {
			int min = 10000, max = 0;
			int min_n, max_n;
			for (int n = 0;n < points.size();n = n + 4)
				for (int k = 4;k < points.size();k = k + 4)
					if (abs(points[n].x - points[(n + k) % points.size()].x) < 90 && abs(points[n].y - points[(n + k) % points.size()].y) < 20) {
						min = abs(points[n].x - points[(n + k) % points.size()].x);
						min_n = n < (n + k) % points.size() ? n : (n + k) % points.size();
						max_n = n > (n + k) % points.size() ? n : (n + k) % points.size();
					}
			points.erase(points.begin() + max_n + 4, points.end());
			points.erase(points.begin(), points.begin() + min_n);
		}
		if (points.size() == 8)
		{
			vector<Point2f> pts(4);
			int detect_valid = 1;
			if (abs(points[0].x - points[4].x) > 4 * abs(points[0].y - points[2].y)) {
				detect_valid = 0;
				putText(src, "target not find", Point(100, 75), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
			}
			if (detect_valid) {
				putText(src, "target find", Point(100, 75), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
				RotatedRect box = minAreaRect(points);
				box.points(pts.data());
				if (pts[0].y > pts[1].y && pts[1].x - pts[0].x < 10) {
					Point p = pts[0];
					pts.erase(pts.begin());
					pts.push_back(p);
				}
				p = (pts[1].x - pts[0].x + pts[2].x - pts[3].x) / 2;
				for (int k = 0; k < 4;k++) {
					line(src, pts[k], pts[(k + 1) % 4], Scalar(255, 0, 0));
					string s = to_string(k);
					putText(src, s, pts[k], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0));
				}
				circle(src, box.center, 5, Scalar(0, 0, 255), -1);
				pnp_valid = solvePnP(objPoints, pts, cameraMatrix, distCoeffs, rvec, tvec);
			}
			
			if (pnp_valid) {
				vector<Point2f> pts_;
				tx_m = Vec3d(tvec)[0];
				ty_m = Vec3d(tvec)[1];
				tz_m = Vec3d(tvec)[2];
				rx_m = Vec3d(rvec)[0];
				ry_m = Vec3d(rvec)[1];
				rz_m = Vec3d(rvec)[2];
				distance_m = norm(tvec);
				if (distance_m < 2.6 && frame_id % 120 == 0 && Z_rough_count < 10) {
					float Z_rough = 1781.053454 * 0.135 / p;
					cout << p<< endl;
					cout << Z_rough_count <<"  "<<Z_rough << "  " << tz_m << " z误差 " << Z_rough - tz_m << endl;
					Z_rough_count++;
				}
				string distance = "distance:" + to_string(distance_m) + "m";
				putText(src, distance, Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
			    projectPoints(objPoints, rvec, tvec, cameraMatrix, distCoeffs, pts_);
			for (int i = 0;i < pts.size();i++) {
				double dx = pts[i].x - pts_[i].x;
				double dy = pts[i].y - pts_[i].y;
				reprojection_error_px += sqrt(dx * dx + dy * dy);
				line(src, pts_[i], pts_[(i + 1) % 4], Scalar(0, 255, 0));
				string s = to_string(i);
				putText(src, s, pts_[i], FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 255, 0));
			}
			reprojection_error_px = reprojection_error_px / 4;
			}
		} 
		ofs << "tx_m " << tx_m<<" "
			<< "ty_m " << ty_m<<" "
			<< "tz_m " << tz_m<<" "
			<< "rx_m " << rx_m<<" "
			<< "ry_m " << ry_m<<" "
			<< "rz_m " << rz_m<<" "
			<< "distance_m " << distance_m << endl
			<< "frame_id " << frame_id << " "
			<< "time_s " << time_s << "\t"
			<< "detect_valid " << detect_valid << " "
			<< "pnp_valid " << pnp_valid << " "
			<< "reprojection_error_px " << reprojection_error_px << endl;
		if (reprojection_error_px >1)
			ofs << "————unreliable————" << endl;//大多数在1下，部分跃升至数百
			imshow("dst", src);
			writer << src;
			waitKey(16);
		
	}
	writer.release();
	cap.release();
	ofs.close();
	}
