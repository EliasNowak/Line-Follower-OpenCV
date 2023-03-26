#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;







int main()
{
	Mat frame, grayFrame, binarizedFrame, draw, erodeFrame, dilateFrame, canny_output;
	vector<vector<Point>> contours;
	int thresh = 100;
	Mat erodeKernel = Mat::ones(Size(3, 3), CV_8U);
	frame = imread("./road.png");

	cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
	threshold(grayFrame, binarizedFrame, 125, 255, THRESH_BINARY_INV);
	erode(binarizedFrame, erodeFrame, erodeKernel);
	dilate(erodeFrame, dilateFrame, erodeKernel);
	Canny(frame, canny_output, thresh, thresh * 2, 3);
	findContours(binarizedFrame, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	if (contours.empty())
		return 0; // send 0x00 to stop 
	//----------------------------------------------------------------------------------
	vector<Moments> mu(contours.size());
	for (size_t i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i]);
		cout << "---> " << mu[i].m00 << endl;
	}
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		//add 1e-5 to avoid division by zero
		mc[i] = Point2f(static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
			static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)));

		cout << "mc[" << i << "]=" << mc[i] << endl;
	}
	for (size_t i = 0; i < contours.size(); i++)
	{
		string values = "";
		drawContours(frame, contours, (int)i, Scalar(255, 0, 0), 2);
		line(frame, Point(mc[i].x, mc[i].y), Point(frame.size().width / 2, frame.size().height / 2), Scalar(125,125,125));
		circle(frame, mc[i], 4, Scalar(255, 255, 255), -1);
		circle(frame, Point(frame.size().width / 2, frame.size().height / 2), 4, Scalar(255, 0, 0), -1); // mitte
		if (mc[i].x > (frame.size().width / 2))
			values = to_string((int)(mc[i].x - (frame.size().width / 2)));
		if (mc[i].x < (frame.size().width / 2))
			values = to_string((int)((frame.size().width / 2) - mc[i].x));
		putText(frame, values, Point(frame.size().width / 2, (frame.size().height / 2) -20), 2, 1, Scalar(255, 0, 0));
	}

	imshow("original", frame);

	waitKey(0);

	return 0;
}