#include "../Header files/stdafx.h"
#include "../Header files/ImageTreatment.h"


ImageTreatment::ImageTreatment()
{
	filterUsed[BLUR] = true;
	filterUsed[BILFILTER] = false;
	filterUsed[HISTEQUAL] = true;
	filterUsed[CANNY] = false;
	filterUsed[THRESHOLD] = true;
	filterUsed[MEDIAN] = true;

}


ImageTreatment::~ImageTreatment()
{
}

void ImageTreatment::setImage(cv::Mat pic)
{
	this->currentPic = pic;
	if (pastPictures.size() > 4) {
		pastPictures.pop_back();
		pastPictures.push_front(pic);
	}
}



void ImageTreatment::findShapes()
{
	currentShapes.clear();
	filteredPic = currentPic.clone();
	if (filterUsed[MEDIAN])
		medianBlur(filteredPic, filteredPic, 3);
	if (filterUsed[BLUR])
		blur(filteredPic, filteredPic, cv::Size(3, 3));
	if (filterUsed[BILFILTER])
		cv::bilateralFilter(filteredPic, filteredPic, 15, 30, 30, cv::BORDER_DEFAULT);

	extractShapes();

}

cv::Mat ImageTreatment::getOriginalPic()
{
	return this->currentPic;
}

const cv::Mat & ImageTreatment::getFilteredPic()
{
	return this->filteredPic;
}

const cv::Mat & ImageTreatment::getFilteredPicWithShapes()
{
	return this->filteredPicWithShapes;
}

const cv::Mat & ImageTreatment::getPicWithShapes()
{
	return this->currentPicWithShapes;
}

std::vector<cv::Mat> ImageTreatment::getShapesAvg()
{
	return std::vector<cv::Mat>();
}

std::vector<cv::Mat> ImageTreatment::getShapes()
{
	return currentShapes;
}

void ImageTreatment::setThreshhold(int threshMin, int threshMax)
{
	minThresh = threshMin;
	maxThresh = threshMax;
}

cv::Mat ImageTreatment::equalizeColorHistograms(cv::Mat& imgInput)
{

	if (imgInput.channels() >= 3)
	{
		cv::Mat ycrcb;

		cvtColor(imgInput, ycrcb, CV_BGR2YCrCb);
		std::vector<cv::Mat> channels;
		split(ycrcb, channels);

		equalizeHist(channels[0], channels[0]);
		equalizeHist(channels[1], channels[1]);
		equalizeHist(channels[2], channels[2]);
		

		cv::Mat result;
		merge(channels, ycrcb);

		cvtColor(ycrcb, result, CV_BGR2RGB);

		return result;
	}
	return imgInput;
}

cv::Mat ImageTreatment::threshHoldColors(cv::Mat & imgInput)
{
	if (imgInput.channels() >= 3)
	{
		cv::Mat ycrcb;

		cvtColor(imgInput, ycrcb, CV_BGR2YCrCb);
		std::vector<cv::Mat> channels;
		split(ycrcb, channels);

		
		threshold(channels[0], channels[0], 120, 250, cv::THRESH_BINARY);
		threshold(channels[2], channels[2], 120, 250, cv::THRESH_BINARY);

		cv::Mat result;
		merge(channels, ycrcb);

		cvtColor(ycrcb, result, CV_BGR2RGB);

		return result;
	}
	return imgInput;
}

void ImageTreatment::threshholdGrey(cv::Mat & imgInput)
{
	//Actuellement vide 
}

void ImageTreatment::threshholdCanny(cv::Mat & imgInput)
{
	//Actuellement vide 
}




void ImageTreatment::extractShapes()
{

	cv::Mat threshold_output;

	std::vector<std::vector<cv::Point> > contours;

	std::vector<cv::Vec4i> hierarchy;
	

	//if (USECOLORTHREASH) 
	//	filteredPic = threshHoldColors(filteredPic);
	if (USEHISTEQUAL)
		filteredPic = equalizeColorHistograms(filteredPic);

	cvtColor(filteredPic, filteredGreyPic, cv::COLOR_BGR2GRAY);
	//equalizeHist(filteredGreyPic, filteredGreyPic);

	//threshold(filteredGreyPic, threshold_output, minThresh, maxThresh, cv::THRESH_BINARY);

	Canny(filteredGreyPic, threshold_output, 50, 150, 3);


	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
	vector<Vec3f> circles;

	HoughCircles(threshold_output, circles, HOUGH_GRADIENT, 1, 10,
		100, 30, 1, 100); // change the last two parameters
						  // (min_radius & max_radius) to detect larger circles	for (size_t i = 0; i < lines.size(); i++)
	currentPicWithShapes = currentPic.clone();
	filteredPicWithShapes = threshold_output.clone();
	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		circle(currentPicWithShapes, Point(c[0], c[1]), c[2], Scalar(100, 100, 0), 3, LINE_AA);
		circle(currentPicWithShapes, Point(c[0], c[1]), 2, Scalar(100, 100, 0), 3, LINE_AA);
	}
	vector<Vec4i> lines;
	HoughLinesP(threshold_output, lines, 1, CV_PI / 180, 40, 40, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(currentPicWithShapes, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(50, 0, 155), 3, CV_AA);
	}

	std::vector<std::vector<cv::Point> > contours_poly(contours.size());

	//![allthework]

	std::vector<cv::Rect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++)

	{

		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		cv::Rect bounding = cv::boundingRect(cv::Mat(contours_poly[i]));
		if (bounding.area()  < (0.6 * (filteredGreyPic.cols * filteredGreyPic.rows)))
			boundRect[i] = bounding;



	}

	std::list<cv::Rect> boundRectNew;

	
	for (int i = 0; i < boundRect.size(); i++)
		if (boundRect[i].area() > 10)
			boundRectNew.push_back(boundRect[i]);

	bool verif = true;

	int c = 0;

	list<Rect>::iterator it = boundRectNew.begin();

	while (it != boundRectNew.end()) {
		verif = false;
		list<Rect>::iterator itt = it;
		itt++;
		while (itt != boundRectNew.end()) {
			if ((*it & *itt).area() > 0) {
				Rect temp = *it | *itt;
				boundRectNew.remove(*itt);
				boundRectNew.remove(*it);
				boundRectNew.push_front(temp);
				it = boundRectNew.begin();
				verif = true;
				itt = it;
				itt++;
				continue;
			}
			else
				itt++;
		}
		if (!verif)
			it++;
		std::cout << " The size : " << boundRectNew.size();

	}

	for (it = boundRectNew.begin(); it != boundRectNew.end(); it++) {
		currentShapes.push_back(currentPic(*it));
	}

	//filteredPicWithShapes = filteredPic.clone();
	//currentPicWithShapes = currentPic.clone();
	for (it = boundRectNew.begin(); it != boundRectNew.end();++it)
	{
		Scalar color = Scalar(0, 200, 0);

		rectangle(filteredPicWithShapes, it->tl(), it->br(), color, 2, 8, 0);
		//rectangle(currentPicWithShapes, it->tl(), it->br(), color, 2, 8, 0);

	}
	Scalar color = Scalar(0, 200, 0);

	for (int i = 0; i < boundRect.size(); i++) {
		
		if (hierarchy[i][2] + hierarchy[i][3] < 0)
		{ 
		rectangle(currentPicWithShapes, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		putText(currentPicWithShapes, hierarchy[i][0] + " " + hierarchy[i][1], boundRect[i].tl(), cv::HersheyFonts::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(140, 0, 0));
		}
	}

}
