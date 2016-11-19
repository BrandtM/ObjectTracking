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

void VideoPlayback::skipToFrame(int frameNo, void* data)
{
	VideoPlayback* vp = (VideoPlayback*)data;
	cv::Mat image;
	vp->capture.set(cv::CAP_PROP_POS_FRAMES, frameNo);
	vp->capture.read(image);
	cv::imshow(vp->window_name, image);
}
