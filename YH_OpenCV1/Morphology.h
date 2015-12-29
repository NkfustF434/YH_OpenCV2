#ifndef _MORPHOLOGY_H_
#define _MORPHOLOGY_H_
#include <opencv2\highgui\highgui.hpp>
#include <array>

using namespace cv;

class Morphology{

private:
	bool CheckIsBinary(Mat clsSouceImage);

public:

	void Dilation(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9] = NULL);
	void Erosion(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9] = NULL);
	void Opening(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9] = NULL);
	void Closing(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9] = NULL);

	void ConnectComponent(Mat clsSouceImage, std::vector<Rect>& clsRectsList, int iMinArea = 0, bool bEightConnect = false, bool bPrint = false);

	void ConnectComponent2(Mat clsSouceImage, std::vector<Rect>& clsRectsList, int iMinArea = 0, bool bEightConnect = false, bool bPrint = false);
};

#endif