#ifndef _MORPHOLOGY_H_
#define _MORPHOLOGY_H_
#include <opencv2\highgui\highgui.hpp>
#include <array>

using namespace cv;

class Morphology{

private:


public:
	void Thersholding(Mat clsSouceImage, Mat& clsTargetImage, int iThersholding);
	void Dilation(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9] = NULL);
	void Erosion(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9] = NULL);
	void Opening(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9] = NULL);
	void Closing(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9] = NULL);

};

#endif