#include"image_basic.h"
#include<filesystem>
int main() {
	Mat src = imread("C:/Users/qweasd/Desktop/opencv_basic/lena.jpg");
	if (src.empty()) {
		cout << "读取失败" << endl;
		return -1;
	}
	cout<<filesystem::current_path()<<endl;
	showimage(src);
    convertcolor(src);
	cropimage(src);
    resizeimage(src);
	Mat dst = src.clone();
	line(dst, Point(0, 0), Point(500, 500), Scalar(0, 0, 255), 1, LINE_8, 0);
	rectangle(dst, Point(200, 200), Point(300, 300), Scalar(0, 255, 0), 1, LINE_8, 0);
	circle(dst, Point(50, 100), 35, Scalar(255, 0, 0), 1, LINE_8, 0);
	putText(dst, "hello world", Point(100, 100), FONT_HERSHEY_SIMPLEX, 2.0,Scalar(255,255,255), 1, LINE_8, false);
	imshow("最终图", dst);
	imwrite("../../../output/result.png", dst);
	waitKey(0);
	destroyAllWindows();
	return 0;
   

}