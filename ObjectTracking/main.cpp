#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "VideoPlayback.h"

#include <iostream>
#include <stdio.h>

/** Function Headers */
void detectAndDisplay(cv::Mat);
void skipToFrame(int, void*);
void playVideo();
cv::Mat translateImg(cv::Mat &, int, int);
void onMouse(int, int, int, int, void*);

/** Global variables */
cv::String window_name = "Object Tracking";
cv::String edges_window = "Edges";
cv::String video_filename = "video.mp4";
VideoPlayback video_playback{ video_filename, window_name };

int thresh = 130;
int max_thresh = 255;
int current_frame = 0;
int max_frames = 0;
bool playback_state = false;

cv::RNG rng(12345);

cv::Point lastPos (0, 0);

/** @function main */
int main(void)
{
	cv::Mat frame;

	if (!video_playback.isReady()) { printf("--(!)Error opening video capture\n"); return -1; }

	cv::namedWindow(window_name);
	cv::createTrackbar("Threshold:", window_name, &thresh, max_thresh);
	cv::createTrackbar("Frame:", window_name, &video_playback.current_frame, video_playback.max_frames, VideoPlayback::skipToFrame, &video_playback);

	//cv::setMouseCallback(window_name, onMouse);

	for (;;)
	{
		int key = cv::waitKey(20);
		if ((char)key == 32) printf("yes");
		else if ((char)key == 27) break;

	}
	printf("fuckface");
	cv::waitKey(0);
	return 0;
}

//void skipToFrame(int frame, void* data) 
//{
//	cv::Mat image;
//	capture.set(cv::CAP_PROP_POS_FRAMES, frame);
//	capture.read(image);
//	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
//	cv::imshow(window_name, image);
//}
//
//void playVideo()
//{
//	cv::Mat frame;
//
//	while(true)
//	{
//		while (capture.read(frame) && !playback_state)
//		{
//			int key = cv::waitKey(20);
//			if ((char)key == 32) return;
//
//			if (frame.empty())
//			{
//				printf(" --(!) No captured frame -- Break!");
//				break;
//			}
//
//			cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
//			detectAndDisplay(frame);
//			cv::imshow(window_name, frame);
//
//			int c = cv::waitKey(20);
//			if ((char)c == 27) { break; } // escape
//		}
//	}
//}
//
//void detectAndDisplay(cv::Mat frame)
//{
//	cv::Mat threshold_output;
//	std::vector<std::vector<cv::Point> > contours;
//	std::vector<cv::Vec4i> hierarchy;
//
//	/// Detect edges using Threshold
//	threshold(frame, threshold_output, thresh, 255, cv::THRESH_BINARY);
//	/// Find contours
//	findContours(threshold_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
//
//	/// Approximate contours to polygons + get bounding rects and circles
//	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
//	std::vector<cv::Point2f>center(contours.size());
//	std::vector<float>radius(contours.size());
//
//	for (int i = 0; i < contours.size(); i++)
//	{
//		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
//		minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i]);
//	}
//
//	/// Draw polygonal contour + bonding rects + circles
//	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
//
//
//	int min = INT_MAX;
//
//	// center[k] will be center of tracking object
//	int k = 0;
//
//	for (int i = 0; i< contours.size(); i++)
//	{
//		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
//		circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);
//		
//		// finding object wchich has minimal distance to previously tracked object
//		auto dist = cv::sqrt((center[i].x - lastPos.x)*(center[i].x - lastPos.x) + (center[i].y - lastPos.y)*(center[i].y - lastPos.y));
//		if(dist < min)
//		{
//			min = dist;
//			k = i;
//		}
//	}
//
//	/// Show in a window
//	namedWindow("Contours", cv::WINDOW_AUTOSIZE);
//	imshow("Contours", drawing);
//
//
//	if (contours.size() > 0) {
//		circle(frame, center[k], (int)radius[k], cv::Scalar(255, 255, 255), 2, 8, 0);
//		frame = translateImg(frame, frame.cols / 2 - center[k].x, frame.rows / 2 - center[k].y);
//		lastPos = center[k];
//	}
//		
//}
//
//cv::Mat translateImg(cv::Mat &img, int offsetx, int offsety) 
//{
//	cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
//	warpAffine(img, img, trans_mat, img.size());
//	return trans_mat;
//}
//
//void onMouse(int event, int x, int y, int flags, void* data)
//{
//	if(event == cv::EVENT_LBUTTONDOWN)
//	{
//		
//	}
//}