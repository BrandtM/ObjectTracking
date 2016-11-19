#include "VideoPlayback.h"
#include <opencv2/imgproc.hpp>

extern void detectAndDisplay(cv::Mat);

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
	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	detectAndDisplay(image);
	cv::imshow(window_name, image);
}

void VideoPlayback::play_video()
{
	cv::Mat frame;

	while (running)
	{
		std::lock_guard<std::mutex> guard(mutex);
		if (playback_state && capture.read(frame))
		{
			if (frame.empty())
				throw std::runtime_error(" --(!) No captured frame -- Break!");

			cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
			detectAndDisplay(frame);
			cv::imshow(window_name, frame);
		}
		cv::waitKey(20);
	}
}
