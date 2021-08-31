//shape detection for 2D shapes -circle,square, rectangle

#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<string>

using namespace std;
using namespace cv;

void getcontours(Mat src, Mat edge) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	string type;
	findContours(edge, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(src, contours, -1, Scalar(255, 0, 255), 2);  //-1 to draw for all shapes
	vector<vector<Point>> conpoly(contours.size());
	vector<Rect> bound(contours.size());

	//filter
	
	for (int i = 0; i < contours.size(); ++i) {
		int area = contourArea(contours[i]);
		if (area > 1000) {
			float peri = arcLength(contours[i], 1); //bool value ie closed shape
			
			approxPolyDP(contours[i], conpoly[i], 0.02 * peri, 1);
			//drawContours(src, conpoly, i, Scalar(255, 0, 255), 2); 
			cout << conpoly[i].size() << endl;
			bound[i] = boundingRect(conpoly[i]);
			int crnrpt = (int)conpoly[i].size();
			if (crnrpt == 3)
				type = "Triangle";
			else if (crnrpt == 4) {
				float aspectratio = (float)bound[i].width / (float)bound[i].height;
				if (aspectratio > 0.95 && aspectratio < 1.05)
					type = "Square";
				else
					type = "Rectangle";
			}
			else if (crnrpt > 4) 
				type = "circle";
			putText(src, type, { bound[i].x,bound[i].y-2  }, FONT_HERSHEY_DUPLEX, 0.4, Scalar(0, 70, 255), 1);

		}
	}


}

//idea is to find edges of image
int main() {
	string p = "Resources/shapes.png";
	Mat i = imread(p);
	imshow("original", i);
	//preprocessed input
	Mat canny, blr, gscale,dil;
	cvtColor(i, gscale, COLOR_BGR2GRAY);
	GaussianBlur(gscale, blr, Size(3, 3), 3, 0);
	Canny(blr, canny, 25, 60);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(canny, dil, kernel);

	getcontours(i,dil);

	imshow("after detection", i);
	
	//imshow("edges", dil);
	waitKey(0);

}