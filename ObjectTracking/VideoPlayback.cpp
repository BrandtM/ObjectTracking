#include "VideoPlayback.h"
#include <opencv2/imgproc.hpp>

extern void stabilize_frame(cv::Mat);

VideoPlayback::VideoPlayback(cv::String filename, cv::String& win_name)
{
	capture.open(filename);
	if (!capture.isOpened())
		throw std::runtime_error("--(!)Error opening video capture\n");

	while (capture.grab())
		max_frames++;

	capture.set(cv::CAP_PROP_POS_FRAMES, 0);
	window_name = win_name;
	running = true;

	video_thread = std::thread(&VideoPlayback::play_video, this);
}

void VideoPlayback::play()
{
	playback_state = true;
}

void VideoPlayback::pause()
{
	playback_state = false;
}

void VideoPlayback::stop()
{
	running = false;
	playback_state = false;
}

bool VideoPlayback::is_playing() const
{
	return running && playback_state;
}

cv::Mat VideoPlayback::get_current_frame()
{
	cv::Mat img;

	std::lock_guard<std::mutex> guard(mutex);
	capture.set(cv::CAP_PROP_POS_FRAMES, std::max(1, (int)capture.get(cv::CAP_PROP_POS_FRAMES) - 1));
	capture.read(img);

	return img;
}

VideoPlayback::~VideoPlayback()
{
	video_thread.join();
}

void VideoPlayback::skip_to_current_frame()
{
	cv::Mat image;

	std::lock_guard<std::mutex> guard(mutex);
	capture.set(cv::CAP_PROP_POS_FRAMES, current_frame);
	capture.read(image);
	stabilize_frame(image);
	cv::imshow(window_name, image);
}

void VideoPlayback::play_video()
{
	cv::Mat frame;

	while (running)
	{
		cv::waitKey(20);
		std::lock_guard<std::mutex> guard(mutex);
		if (playback_state && capture.read(frame))
		{
			if (frame.empty())
				throw std::runtime_error(" --(!) No captured frame -- Break!");
			
			stabilize_frame(frame);
			imshow(window_name, frame);
		}
	}
}
