#ifndef _FILTERS_H_
#define _FILTERS_H_

#include <opencv2\highgui\highgui.hpp>
using namespace cv;

class Filters{

public:
	enum class FilterKind
	{
		BoxFilter,
		MediaFiler,
		MaxFilter,
		MinFilter,
		LaplacianImageEnhancement,
		SolbeX,
		SolbeY,
		AdaptiveMedianFilter
	};

	static void FilterProcess(FilterKind eFilterFind, Mat clsSouceImage, Mat& clsTargetImage);

	static void CustomFilterProcess(int iFilter[3][3], Mat clsSouceImage, Mat& clsTargetImage);


private :

	enum class GetType
	{
		Min,
		Mid,
		Max,
	};

	static void SortFilterProcess(GetType eGetType, Mat clsSouceImage, Mat& clsTargetImage);
	static void Filters::AdaptMedianFilter(Mat clsSouceImage, Mat& clsTargetImage);

};

#endif