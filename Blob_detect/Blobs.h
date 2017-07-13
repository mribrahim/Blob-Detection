
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class Blobs
{

public:
	Point2f center;

	float area;
	float perimeter;
	float eccentricity;
	float solidity;
	float compactness;
	float angle;

	Vec3b mean_intensity;
	Rect bounding_box;
};

