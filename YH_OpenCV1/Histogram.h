#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_
#include <opencv2\highgui\highgui.hpp>
#include <array>

using namespace cv;

class Histogram{

private:
public:

	void GetImageHistogram(Mat clsSouceImage, std::array<int, 256>& iSouceHistogramArray);
	void DrawHistogram(string strHistogramName, Mat clsHistogramImage, std::array<int, 256> iSouceDatas, float fRatio, bool bShowHistogram);
	void SetValueToImage(Mat clsSouceImage, Mat& clsTargetImage, std::array<int, 256> fTransformRatioDatas, bool bShowTargetImage);
	void SetReverseValueToImage(Mat clsSouceImage, Mat& clsTargetImage, std::array<float, 256> fTransformRatioDatas, bool bShowTargetImage);
	void HistogramEqualization(int iPixelCount, std::array<int, 256> iSouceDatas, std::array<int, 256>& fTransformRatioDatas);

	void Matching(Mat clsSouceImage, Mat clsTargerImage);

};

#endif