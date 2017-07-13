# OpenCV Settings

Download and copy Opencv 2.4.13 version to C:\OpenCV_2.4.13 directory

 Visual Studio settings ( change OpenCV version according to you download )

- VC++ Directories->Include Directories :  C:\OpenCV_2.4.13\build\include;
- VC++ Directories->Library Directories :  C:\OpenCV_2.4.13\build\x86\vc12\lib;
- Linker->Input->Additional Dependencies->Input  opencv_calib3d2413.lib,opencv_video2413.lib .....vs (write .lib you use)


# Blob-Detection Using OPENCV (C++)

A Blob means a group of connected pixels in an image that share near intensity values. 
In the image below, the goal is to detect the blobs and identify some properties belong to that blobs, as if regionprobs in Python 
(http://scikit-image.org/docs/dev/api/skimage.measure.html#skimage.measure.regionprops).


<br>
<font color="red" size="5"> <strong>Measure Properties of Regions</strong> </font>
<br><br>
<strong>center:</strong>      point2f ->  Center coordinates of region
<br>
<strong>area:</strong>      float ->  Number of pixels of region.
<br>
<strong>perimeter:</strong>     float ->  Perimeter of region.
<br>
<strong>eccentricity:</strong>      float ->  Between 1 and 0. (1 for circle)
<br>
<strong>solidity:</strong>      float ->  Between 1 and 0. ( Area / ConvexHullArea ) 
<br>
<strong>compactness:</strong>     float ->  Between 1 and 0. ( Area / BoundingRect Area ) -- measure of how close to a rectangle
<br>
<strong>mean_intensity:</strong>      Vec3b ->  mean intensity values of three channel(blue,green,red) of the region
<br>
<strong>bounding_box:</strong>      Rect  -> Bounding box of the region
<br>

<img height="400" src="https://github.com/mribrahim/Blob-Detection/blob/master/Blob_detect/blobs.jpg" />

Usage of provided Blob Detection Function


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
		str = to_string(myblobs[i].area);
		str = to_string(myblobs[i].eccentricity);
		//str = to_string(myblobs[i].solidity);
		//str = to_string(myblobs[i].compactness);
		//str = to_string(myblobs[i].perimeter);
		//str = to_string(myblobs[i].mean_intensity.val[0]);

		putText(image_blobs, str , cvPoint(myblobs[i].center.x, myblobs[i].center.y), FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(100, 200, 250), 1, CV_AA);
	}

	namedWindow("Blobs", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Blobs", image_blobs);
  
  
  <img height="400" src="https://github.com/mribrahim/Blob-Detection/blob/master/Blob_detect/areaofblobs.png" />
  <img height="400" src="https://github.com/mribrahim/Blob-Detection/blob/master/Blob_detect/eccentricityofblobs.png" />
  
  
