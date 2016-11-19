#include "VideoPlayback.h"


VideoPlayback::VideoPlayback(cv::String filename, cv::String& win_name)
{
	capture.open(filename);
	if (!capture.isOpened()) {
		ready = false; 
		return;
	}

	while (capture.grab())
		max_frames++;

	capture.set(cv::CAP_PROP_POS_FRAMES, 0);
	window_name = win_name;
	ready = true;
}

bool VideoPlayback::isReady()
{
	return ready;
}

void VideoPlayback::skipToCurrent()
{
	cv::Mat image;
	capture.set(cv::CAP_PROP_POS_FRAMES, current_frame);
	capture.read(image);
	cv::imshow(window_name, image);
}
