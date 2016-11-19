#pragma once
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include <thread>
#include <string>

class VideoPlayback
{
public:
	int current_frame = 0;
	int max_frames = 0;

	VideoPlayback(cv::String filename, cv::String& win_name);
	bool isReady();
	void skipToCurrent();

private:
	cv::VideoCapture capture;
	bool playback_state = false;
	bool ready = false;
	std::string window_name;
};

