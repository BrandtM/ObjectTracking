#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <cstdio>

int main(int argc, char** argv)
{
	cv::Mat image, grayImage, gradient, gradient_x, gradient_y, abs_gradient_x, abs_gradient_y;
	const char* windowName = "Image window";
	image = cv::imread("image3.jpg", cv::IMREAD_COLOR); // Read the file
	int sobelScale = 1, sobelDelta = 0, sobelDepth = 3;

	if (!image.data) // Check for invalid input
	{
		printf("Could not open or find the image\n");
		return -1;
	}

	cv::GaussianBlur(image, image, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
	cv::cvtColor(image, grayImage, cv::COLOR_RGB2GRAY);

	// Horizontal edge detection
	cv::Sobel(grayImage, gradient_x, sobelDepth, 1, 0, 1, sobelScale, sobelDelta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(gradient_x, abs_gradient_x);

	// Vertical edge detection
	cv::Sobel(grayImage, gradient_y, sobelDepth, 0, 1, 1, sobelScale, sobelDelta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(gradient_y, abs_gradient_y);

	cv::addWeighted(abs_gradient_x, 0.5, abs_gradient_y, 0.5, 0, gradient);

	cv::namedWindow(windowName, cv::WINDOW_NORMAL); // Create a window for display.
	imshow(windowName, gradient); // Show our image inside it.

	cv::waitKey(0); // Wait for a keystroke in the window
	return 0;
}