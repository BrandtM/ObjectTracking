#pragma once
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include <thread>
#include <string>
#include <mutex>

class VideoPlayback
{
public:
	int max_frames = 0;
	int current_frame = 0;

	VideoPlayback(cv::String filename, cv::String& win_name);
	~VideoPlayback();

	void play();
	void pause();
	void stop();						// end video thread

	bool is_playing() const;

	void skip_to_current_frame();

private:
	cv::VideoCapture capture;
	bool playback_state = false;		// playing or paused
	volatile bool running = false;		// not sure if it should be volatile (used in thread)
	std::string window_name;

	std::thread video_thread;
	std::mutex mutex;					// for current_frame (modifying can occur while playing)
	void play_video();			
};

