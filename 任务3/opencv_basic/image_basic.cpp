
#include"image_basic.h"
void showimage(Mat &src) {
	cout << "图片宽度：" << src.cols << "\n"
		<< "图片高度：" << src.rows << "\n"
		<< "图片通道数：" << src.channels() << endl;
	imshow("原图片", src);
	waitKey(0);
	destroyAllWindows();
}
void convertcolor(Mat &src){
	Mat m1, m2;
cvtColor(src, m1, COLOR_BGR2GRAY);
cvtColor(src, m2, COLOR_BGR2HSV);
imshow("gray", m1);
imshow("hsv", m2);
imwrite("../../../output/gray.png", m1);
imwrite("../../../output/hsv.png", m2);
waitKey(0);
destroyAllWindows();

}
void cropimage(Mat &src){
	int x = 0;
	int y = 0;
	int w = 300;
	int	h = 300;
	//cin >> x >> y >> w >> h;
	Rect roi(x, y, w, h);
	
	if (x >=0 && y >= 0 && x + w <= src.cols && y + h <= src.rows)
	{
		imshow("裁剪图", src(roi));
		imwrite("../../../output/roi.png",src(roi));
		waitKey(0);
		destroyAllWindows(); 

	}
		else
			cout << "超越边界" << endl;
}
void resizeimage(Mat &src) {
	Mat dst;
	resize(src, dst, Size(), 0.5, 0.5);
	imshow("缩放图", dst);
	imwrite("../../../output/resized.png", dst);
	waitKey(0);
	destroyAllWindows();

}
