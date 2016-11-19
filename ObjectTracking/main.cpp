#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "VideoPlayback.h"

#include <iostream>
#include <stdio.h>

using contour_t = std::pair<cv::Point2f, float>;

/** Function Headers */
std::vector<contour_t> find_contours(cv::Mat);
void stabilize_frame(cv::Mat);
cv::Mat translate_img(cv::Mat, int, int);
void on_mouse(int, int, int, int, void*);

/** Global variables */
cv::String window_name = "Object Tracking";
cv::String edges_window = "Edges";
cv::String video_filename = "video.mp4";
VideoPlayback video_playback{ video_filename, window_name };

int thresh = 130;
int max_thresh = 255;

cv::RNG rng(12345);

cv::Point lastPos(0, 0);

/** @function main */
int main(void)
{
	cv::Mat frame;

	cv::namedWindow(window_name);
	cv::createTrackbar("Threshold:", window_name, &thresh, max_thresh);
	cv::createTrackbar("Frame:", window_name, &video_playback.current_frame, video_playback.max_frames,
		[](int, void*) {video_playback.skip_to_current_frame(); });

	namedWindow("Contours", cv::WINDOW_AUTOSIZE);
	cv::setMouseCallback("Contours", on_mouse, &video_playback);

	for (;;)
	{
		int key = cv::waitKey(20);
		if ((char)key == 32)		// space
		{
			if (video_playback.is_playing())
				video_playback.pause();
			else
				video_playback.play();
		}
		else if ((char)key == 27)	// esc
			video_playback.stop();
	}

	cv::waitKey(0);
	return 0;
}

std::vector<contour_t> find_contours(cv::Mat frame)
{
	cv::Mat image;
	frame.copyTo(image);
	cv::cvtColor(frame, image, CV_BGR2GRAY);

	cv::Mat threshold_output;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	/// Detect edges using Threshold
	threshold(image, threshold_output, thresh, 255, cv::THRESH_BINARY);
	/// Find contours
	findContours(threshold_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/// Approximate contours to polygons + get bounding rects and circles
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	std::vector<std::pair<cv::Point2f, float>> bounding_circles(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((cv::Mat)contours_poly[i], bounding_circles[i].first, bounding_circles[i].second);
	}

	return bounding_circles;
}

void stabilize_frame(cv::Mat frame)
{
	auto contours = find_contours(frame);

	/// Draw bonding circles
	cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);

	int min = INT_MAX;
	int k = 0;

	for (int i = 0; i< contours.size(); i++)
	{
		auto &center = contours[i].first;
		auto &radius = contours[i].second;

		cv::Scalar color = cv::Scalar(255, 255, 255);
		circle(drawing, center, (int)radius, color, 2, 8, 0);

		// finding object wchich has minimal distance to previously tracked object
		auto dist = cv::sqrt((center.x - lastPos.x)*(center.x - lastPos.x) + (center.y - lastPos.y)*(center.y - lastPos.y));
		if (dist < min)
		{
			min = dist;
			k = i;
		}
	}

	if (contours.size() > 0) 
	{
		auto &center = contours[k].first;
		auto &radius = contours[k].second;
		circle(frame, center, (int)radius, cv::Scalar(255, 255, 255), 2, 8, 0);
		circle(drawing, center, (int)radius, cv::Scalar(150, 255, 150), 2, 8, 0);

		frame = translate_img(frame, frame.cols / 2 - center.x, frame.rows / 2 - center.y);

		lastPos = center;
	}

	/// Show in a window
	imshow("Contours", drawing);
}

cv::Mat translate_img(cv::Mat img, int offsetx, int offsety)
{
	cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
	return trans_mat;
}

void on_mouse(int event, int x, int y, int flags, void* data)
{
	auto frame = static_cast<VideoPlayback*>(data)->get_current_frame();
	auto contours = find_contours(frame);

	if (event == cv::EVENT_LBUTTONDOWN)
	{
		for(auto& contour : contours)
		{
			auto &center = contour.first;
			auto &radius = contour.second;
			if((x - center.x) * (x - center.x) + (y - center.y) * (y - center.y) < radius * radius)
			{
				lastPos = center;
				stabilize_frame(frame);
				imshow(window_name, frame);
			}
		}
	}
}