#include "Thresholding.h"
#include "Histogram.h"
#include <math.h>

void Thresholding::NomalThersholding(Mat clsSouceImage, Mat& clsTargetImage, int iThersholding)
{
	Mat clsTempImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);
	for (int iCol = 1; iCol < clsSouceImage.cols - 1; iCol++)
	{
		for (int iRow = 1; iRow < clsSouceImage.rows - 1; iRow++)
		{

			if (clsSouceImage.at<unsigned char>(iRow, iCol) > iThersholding)
				clsTempImage.at<unsigned char>(iRow, iCol) = 255;
			else
				clsTempImage.at<unsigned char>(iRow, iCol) = 0;
		}
	}
	clsTargetImage = clsTempImage.clone();
}

void Thresholding::OtsusThersholding(Mat clsSouceImage, Mat& clsTargetImage)
{
	Mat clsTempImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);
	float fVaranceArray[256];
	int iLeftCount = 0;
	float fMean1 = 0;
	float fMean2 = 0;
	int iFinalT = 0;
	int iMaxVarence = 0;
	//compute histogram
	Histogram clsHistogram;
	std::array<int, 256> iHistogramData;
	clsHistogram.GetImageHistogram(clsSouceImage, iHistogramData);
	int iPixelCount = 0;

	for (int iPixelValue = 0; iPixelValue < 256; iPixelValue++)
	{
		if (iPixelCount + iHistogramData[iPixelValue] != 0)
		{
			fMean2 = (fMean2*iPixelCount + iHistogramData[iPixelValue] * iPixelValue) / (iPixelCount + iHistogramData[iPixelValue]);
			iPixelCount += iHistogramData[iPixelValue];
		}
	}

	//compyte 0~255 varance
	for (int iT = 0; iT < 256; iT++)
	{
		if (iT != 0)
		{
			if (iLeftCount + iHistogramData[iT] != 0)
				fMean1 = (fMean1*iLeftCount + iT*iHistogramData[iT]) / (iLeftCount + iHistogramData[iT]);
			if (((iPixelCount - iLeftCount) - iHistogramData[iT]) == 0)
				break;// there isn't value after this index
			fMean2 = (fMean2*(iPixelCount - iLeftCount) - iT*iHistogramData[iT]) / ((iPixelCount - iLeftCount) - iHistogramData[iT]);
		}
		float fQ1 = ((float)iLeftCount / (float)iPixelCount) + (iHistogramData[iT] / (float)iPixelCount);
		float fQ2 = ((float)(iPixelCount - iLeftCount) / (float)iPixelCount) - (iHistogramData[iT] / (float)iPixelCount);
		fVaranceArray[iT] = fQ1*fQ2* std::abs(fMean1 - fMean2);
		if (fVaranceArray[iT] > iMaxVarence)
			iMaxVarence = fVaranceArray[iT];

		if (iT != 0)
		{
			if (fVaranceArray[iT] - fVaranceArray[iT - 1] < 0)
			{
				iFinalT = iT;
				break;
			}
		}
		iLeftCount += iHistogramData[iT];
		//printf("%f \n", fVaranceArray[iT]);
	}

	//Thresholding
	NomalThersholding(clsSouceImage, clsTempImage, iFinalT);
	printf("Otsu: %d\n", iFinalT);
	clsTargetImage = clsTempImage.clone();
}

void Thresholding::Negative(Mat clsSouceImage, Mat& clsTargetImage)
{
	Mat clsTempImage = clsSouceImage.clone();
	for (int iCol = 0; iCol < clsSouceImage.cols; iCol++)
	{
		for (int iRow = 0; iRow < clsSouceImage.rows; iRow++)
		{
			if (clsSouceImage.at<unsigned char>(iRow, iCol) == 255)
			{
				clsTempImage.at<unsigned char>(iRow, iCol) = 0;
			}
			else
			{
				clsTempImage.at<unsigned char>(iRow, iCol) = 255;
			}
		}
	}

	clsTargetImage = clsTempImage.clone();
}