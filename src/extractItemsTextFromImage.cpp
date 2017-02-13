/**********************************************************************
*                     Dominik Schulz, 09.02.2017                     *
**********************************************************************/

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

using namespace std;
using namespace cv;

const bool writeDebugImages = true;
const int sharpening_size = 5;
const double sharpening_alpha = 7;
const int erosion_height = 120;
const int dilate_size = 25;
const int threshold_block_size = 71;
const int threshold_constant_factor = -10;
const int erosion_items_pre = 10;
const int dilation_items_pre = 50;
const int horizontal_line_margin = 10;
const int horizontal_line_avg_thresh = 40;

double getMainLinesRotationAndPosition(vector<pair<Point2f,int> >, vector<double>, int&, int&);
string trim(const string& str){
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first){
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int extractItemsTextFromImage(string filename, vector<string>& text)
{ 
	Mat input_img = imread(filename, IMREAD_GRAYSCALE);
	if (input_img.empty()){
		cout << "Cannot load image " << filename << endl;
		return -1;
	}

	Mat bg;
	GaussianBlur(input_img, bg, Size(sharpening_size,sharpening_size), 0);

	Mat highpass;
	subtract(input_img,bg,highpass, noArray(), CV_16SC1);

	Mat sharpened;
	addWeighted(input_img, 1, highpass, sharpening_alpha, 0,  sharpened, CV_8UC1);

	Mat binarized;
	adaptiveThreshold(sharpened, binarized, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, threshold_block_size,
			threshold_constant_factor);

	Mat eroded;
	Mat elem = getStructuringElement(MORPH_RECT, Size(1, erosion_height));
	erode(binarized, eroded, elem);

	Mat dilated;
	elem = getStructuringElement(MORPH_RECT, Size(dilate_size, 20 * dilate_size));
	dilate(eroded, dilated, elem);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(dilated, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	if(contours.size() < 3){
		cout << "Error! Didn't find vertical lines!" << endl;
		return -1;
	}

	vector<pair<Point2f,int> > centers(contours.size());
	vector<double> orientations(contours.size());
	Mat withcontours = Mat::zeros(dilated.rows, dilated.cols, CV_8UC3);
	for(int i = 0; i < static_cast<int>(contours.size()); i++){
		Scalar color(rand()&255, rand()&255, rand()&255);
		drawContours(withcontours, contours, i, color, CV_FILLED);
		
		Moments mm = moments(contours[i], false);
		Point2f mass_center = Point2f(mm.m10 / mm.m00, mm.m01 / mm.m00);
		//cout << "Contour " << i << " center of mass: " << mass_center.x << ", " << mass_center.y << endl;
		centers[i].first = mass_center;
		centers[i].second = i;

		double theta = 0.5 * atan2(2 * mm.mu11 / mm.m00 , (mm.mu20 / mm.m00 - mm.mu02 /mm.m00));
		//cout << "Orientation: " << theta * 180 / M_PI << endl;
		orientations[i] = theta;
	}
	if(writeDebugImages){
		imwrite("tmp/linecontours.jpg", withcontours);
	}


	Mat rotated;
	Mat rotated_bin;
	int x_left, x_right;
	double line_rot = getMainLinesRotationAndPosition(centers, orientations, x_left, x_right);
	double correcting_rot = -M_PI/2 - line_rot;
	correcting_rot *= -180/M_PI; // opencv rotation uses angle and mathematical positive direction
	Mat rot_mat = getRotationMatrix2D(Point(dilated.cols/2, dilated.rows/2), correcting_rot, 1);
	warpAffine(input_img, rotated, rot_mat, dilated.size());
	warpAffine(binarized, rotated_bin, rot_mat, dilated.size());

	Mat cropped;
	Mat cropped_bin;
	Rect selection(x_left, 0, x_right - x_left, rotated.rows);
	//cout << x_left << "," << x_right << endl;
	cropped = rotated(selection);
	cropped_bin = rotated_bin(selection);

	Mat eroded_horiz;
	elem = getStructuringElement(MORPH_RECT, Size(erosion_height, 1));
	erode(cropped_bin, eroded_horiz, elem);
	if(writeDebugImages){
		imwrite("tmp/horizontal-lines.jpg", eroded_horiz);
	}

	Mat reduced(eroded_horiz.rows, 1, eroded_horiz.type());
	reduce(eroded_horiz, reduced, 1, CV_REDUCE_AVG);
	int horizontal_line_y = reduced.rows;
	for (int i = 0; i < reduced.rows; ++i) {
		if(reduced.at<uchar>(i,0) > horizontal_line_avg_thresh && i > eroded_horiz.rows / 2){
			horizontal_line_y = i;
			break;
		}
	}
	if(writeDebugImages){
		imwrite("tmp/binarized-cropped.jpg", cropped_bin);
	}

	selection = Rect(0, 0, cropped.cols, horizontal_line_y - horizontal_line_margin);
	cropped = cropped(selection);
	cropped_bin = cropped_bin(selection);

	Mat items_eroded;
	elem = getStructuringElement(MORPH_RECT, Size(erosion_items_pre, erosion_items_pre));
	erode(cropped_bin, items_eroded, elem);

	Mat items_dilated;
	elem = getStructuringElement(MORPH_RECT, Size(dilation_items_pre, dilation_items_pre));
	dilate(items_eroded, items_dilated, elem);
	if(writeDebugImages){
		imwrite("tmp/item-sections.jpg", items_dilated);
	}


	tesseract::TessBaseAPI tess;
	if(tess.Init(NULL,"deu")){
		cerr << "Could not init tesseract";
		return -1;
	}
	tess.SetVariable("tessedit_char_whitelist", "abcdefghijklmnopqrstuvwxyzäüöABCDEFGHIJKLMNOPQRSTUVWXYZ012345789()ß");

	findContours(items_dilated, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Mat items_dilated_contours = Mat::zeros(items_dilated.rows, items_dilated.cols, CV_8UC3);
	for(int i = 0; i < static_cast<int>(contours.size()); i++){
		Rect bounding = boundingRect(contours[i]);
		Scalar color_green(0, 255, 0);
		Scalar color_red(0, 0, 255);

		//cout << "Detected: " << bounding.height << "x"<< bounding.width;
		if(bounding.height < bounding.width &&
				bounding.width > items_dilated.cols / 5){
			drawContours(items_dilated_contours, contours, i, color_green, CV_FILLED);
			//cout << "\033[32m item \033[0m" << endl;

			// OCR
			Mat sub = cropped(bounding);
			tess.SetImage(static_cast<uchar*>(sub.data), sub.cols, sub.rows, sub.channels(), sub.step1());
			const char* out = tess.GetUTF8Text();
			float confidence = tess.MeanTextConf();
			//cout << out << " (" << confidence << ")" << endl;
			text.push_back(out);
			delete[] out;


		}else{
			drawContours(items_dilated_contours, contours, i, color_red, CV_FILLED);
			//cout << "\033[31m no item \033[0m" << endl;
		}
	}
	
	//Mat scaled;
	//resize(items_dilated_contours, scaled, Size(0,0), 0.3, 0.3);
	//namedWindow("Display" );
	//imshow("Display", scaled);

	//waitKey();
	//resizeWindow("Display", 1600, 1050);

	return 0;
}

double getMainLinesRotationAndPosition(vector<pair<Point2f,int> > centers, vector<double> orientations,
		int& x_left, int& x_right){
	sort(centers.begin(), centers.end(), 
			[](pair<Point2f,int> a, pair<Point2f,int> b) -> bool {return a.first.x < b.first.x;});

	//check whether leftmost line is actually the one we want
	//distance between two left of item lists is a lot smaller than distance to left border
	int idx = 0;
	while(centers[idx + 1].first.x - centers[idx].first.x > centers[idx].first.x){
		idx++;
	}

	//now take left line of item list
	idx++;
	


	x_left = centers[idx].first.x;
	x_right = centers[idx + 1].first.x;

	// use orientation of second contour from the left
	return orientations[centers[idx].second];
}
