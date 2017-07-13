
#include "opencv2/opencv.hpp"

#include <iostream>

#include "Blob_Detect.h"

using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{

	Mat image = imread("blobs.jpg");
	Mat image_blobs;

	namedWindow("Image", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Image", image);


	blur(image, image, Size(5, 5));

	threshold(image, image_blobs, 200, 255, THRESH_BINARY_INV);

	
	Blob_Detect blob_detect;

	vector<Blobs> myblobs = blob_detect.FindBlobs(image,image_blobs);

	string str;
	for (size_t i = 0; i < myblobs.size(); i++)
	{
		//str = to_string(myblobs[i].area);
		//str = to_string(myblobs[i].eccentricity);
		//str = to_string(myblobs[i].solidity);
		//str = to_string(myblobs[i].compactness);
		//str = to_string(myblobs[i].perimeter);
		//str = to_string(myblobs[i].mean_intensity.val[0]);

		str = to_string(myblobs[i].angle);

		putText(image_blobs, str , cvPoint(myblobs[i].center.x, myblobs[i].center.y), FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(100, 200, 250), 1, CV_AA);
	}


	namedWindow("Blobs", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Blobs", image_blobs);

	while (1)
	{

		if (waitKey(4) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}


	return 0;

}