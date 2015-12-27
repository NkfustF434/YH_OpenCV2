#ifndef _DIP_HOMEWORK_H_
#define _DIP_HOMEWORK_H_

#include <opencv2/opencv.hpp>
#include <math.h>
#include <array>
#include <iostream>
#include <sstream>
#include "Filters.h"
#include "Noise.h"
#include "Histogram.h"
#include "Morphology.h"
#include "Thresholding.h"
#include "myFeatureExtractor.h"
#include "myImageSequence.h"

using namespace cv;
class DipHomework{

private:

	/*Channel*/
	int const HISTOGRAM_EQUALIZATION_CHANNEL = 1;
	int const MATCHING_CHANNEL = 2;
	int const FILTER_CHANNEL = 3;
	int const NOISE_CHANNEL = 4;
	int const MORPHOLOGY_CHANNEL = 5;
	int const THRESHOLDING_CHANNEL = 6;
	int const CONNECT_COMPONNENT_CHANNEL = 7;
	int const EXIT_CHANNEL = -1;

	/*Image*/
	int const LENA_INDEX = 1;
	int const White_BEAN_INDEX = 2;
	int const GRAY_BEAN_INDEX = 3;
	int const BLACK_BEAN_INDEX = 4;
	int const PEPERSALT_INDEX = 5;
	int const NOISETEST_INDEX = 6;
	int const MORPHOLOGY_INDEX = 7;
	int const OTHER_INDEX = 99;

private:
	Mat LoadImage(bool bShowMessage, string strTitle = "", bool bClearView = false);

	int Menu();

	void HistogramEqualization();

	void Matching();

	void Filter();

	void AddNoise();

	void Morphological();

	void Threshold();

	void ConnectCompronent();

public:

	void HwMain();
};

#endif