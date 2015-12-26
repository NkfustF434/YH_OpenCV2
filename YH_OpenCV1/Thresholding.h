#ifndef _THRESHOLDING_H_
#define _THRESHOLDING_H_

#include <opencv2\highgui\highgui.hpp>

using namespace cv;

class Thresholding{

private:
public:

	void NomalThersholding(Mat clsSouceImage, Mat& clsTargetImage, int iThersholding);

	void OtsusThersholding(Mat clsSouceImage, Mat& clsTargetImage);
};

#endif