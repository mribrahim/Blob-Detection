
#include "Blob_Detect.h"


Blob_Detect::Blob_Detect()
{
	// minimum blob size
	blob_size_min = 10;
}


Blob_Detect::~Blob_Detect()
{
}


double Blob_Detect::get_eccentricity(Moments mu)
{
	double bigSqrt = sqrt((mu.mu20 - mu.mu02) *  (mu.mu20 - mu.mu02) + 4 * mu.mu11 * mu.mu11);
	return (double)(mu.mu20 + mu.mu02 - bigSqrt) / (mu.mu20 + mu.mu02 + bigSqrt);
}

double Blob_Detect::get_eccentricity2(vector<Point> contour)
{ 
	RotatedRect ellipse = fitEllipse(contour); 
	float a = ellipse.size.height;
	float b = ellipse.size.width;

	if (a<b)
		return  a / b;
	else return b / a;
}

double Blob_Detect::get_solidity(float area, InputArray contour)
{
	vector<Point> ch; convexHull(contour, ch);
	double CHA = contourArea(ch);

	return area / CHA;
}

vector<Blobs> Blob_Detect::FindBlobs(Mat image,Mat input)
{
	std::vector<Blobs> myBlobs;

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;



	int width = input.cols;
	int height = input.rows;

	try {
		/// Detect edges using canny
		Canny(input, canny_output, 200, 10);
		/// Find contours
		//findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

		cv::Mat labels = cv::Mat::zeros(input.size(), CV_8UC1);
		std::vector<float> cont_avgs(contours.size(), 0.f); // This contains the averages of each contour

		/// Get the moments
		vector<Moments> mu(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			if (contourArea(contours[i]) > blob_size_min)
				mu[i] = moments(contours[i], false);
		}

		///  Get the mass centers:
		vector<Point2f> mc(mu.size());
		for (size_t i = 0; i < mu.size(); i++)
		{
			Blobs b;

			mc[i] = Point2f((mu[i].m10 / mu[i].m00), (mu[i].m01 / mu[i].m00));

			RotatedRect rotatedRect = cv::minAreaRect(contours[i]);
			cv::Point2f rect_points[4];
			rotatedRect.points(rect_points);
			
			// choose the longer edge of the rotated rect to compute the angle
			cv::Point2f edge1 = cv::Vec2f(rect_points[1].x, rect_points[1].y) - cv::Vec2f(rect_points[0].x, rect_points[0].y);
			cv::Point2f edge2 = cv::Vec2f(rect_points[2].x, rect_points[2].y) - cv::Vec2f(rect_points[1].x, rect_points[1].y);

			cv::Point2f usedEdge = edge1;
			if (cv::norm(edge2) > cv::norm(edge1))
				usedEdge = edge2;

			cv::Point2f reference = cv::Vec2f(1, 0); // horizontal edge

			b.angle = 180.0f / CV_PI * acos((reference.x*usedEdge.x + reference.y*usedEdge.y) / (cv::norm(reference) *cv::norm(usedEdge)));

			
			b.center = mc[i];
			b.area = contourArea(contours[i]);
			b.eccentricity = get_eccentricity(mu[i]);
			//b.eccentricity = get_eccentricity2(contours[i]);

			Rect boundrect = boundingRect(contours[i]);
			b.bounding_box = boundrect;

			b.solidity = get_solidity(b.area,contours[i]);

			b.perimeter = arcLength(contours[i],true);

			b.compactness = b.area / (boundrect.width*boundrect.height);
			
			Vec3b mean_intensity;
			int numberofpixels = contours[i].size();
			for (size_t k = 0; k < numberofpixels; k++)
			{
				int x = contours[i][k].x;
				int y = contours[i][k].y;

				Vec3b intensity = image.at<Vec3b>(y,x);
				int blue = intensity.val[0];
				int green = intensity.val[1];
				int red = intensity.val[2];

				mean_intensity.val[0] += (float)blue/ numberofpixels;   
				mean_intensity.val[1] += (float)green/ numberofpixels;
				mean_intensity.val[2] += (float)red/ numberofpixels;
			}


			b.mean_intensity = mean_intensity ;

			myBlobs.push_back(b);

			//break;
		}

	}
	catch (Exception e) {}
	//myBlobDetector.detect(input, myBlobs);

	return myBlobs;
}