#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame);

/** Global variables */
String window_name = "Object Tracking";
String edges_window = "Edges";

/** @function main */
int main(void)
{
	VideoCapture capture("video.mp4");
	Mat frame;

	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		//-- 3. Apply the classifier to the frame
		detectAndDisplay(frame);

		int c = waitKey(20);
		if ((char)c == 27) { break; } // escape
	}
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	Mat edges;
	Canny(frame, edges, 150, 450);

	imshow(window_name, frame);
	imshow(edges_window, edges);
}