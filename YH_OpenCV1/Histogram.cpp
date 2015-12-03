#include "Histogram.h"

void Histogram::GetImageHistogram(Mat clsSouceImage, std::array<int, 256>& iSouceHistogramArray)
{
	iSouceHistogramArray.fill(0);
	for (int iCol = 0; iCol < clsSouceImage.cols; iCol++)
	{
		for (int iRow = 0; iRow < clsSouceImage.rows; iRow++)
		{
			int iGrayValue = clsSouceImage.at<unsigned char>(iRow, iCol);
			iSouceHistogramArray[iGrayValue] ++;
		}
	}
}

void Histogram::DrawHistogram(string strHistogramName, Mat clsHistogramImage, std::array<int, 256> iSouceDatas, float fRatio, bool bShowHistogram)
{
	//Draw Histogram
	for (int iGrayLevel = 0; iGrayLevel < 256; iGrayLevel++)
	{
		if (iSouceDatas[iGrayLevel] == 0)continue;
		cv::line(clsHistogramImage, Point(iGrayLevel+2, clsHistogramImage.rows), Point(iGrayLevel+2, clsHistogramImage.rows - (iSouceDatas[iGrayLevel]) * fRatio), cv::Scalar(127, 127, 127));
	}

	if (bShowHistogram)
	{
		cv::namedWindow(strHistogramName, cv::WINDOW_AUTOSIZE);
		cv::imshow(strHistogramName, clsHistogramImage);
	}
}

void Histogram::SetValueToImage(Mat clsSouceImage, Mat& clsTargetImage, std::array<int, 256> fTransformRatioDatas, bool bShowTargetImage)
{

	if (clsTargetImage.empty())
	{
		Mat clsTargetImage = Mat::zeros(clsSouceImage.rows, clsSouceImage.cols, CV_8UC1);
	}

	for (int iColIndex = 0; iColIndex < clsSouceImage.cols; iColIndex++)
	{
		for (int iRowIndex = 0; iRowIndex < clsSouceImage.rows; iRowIndex++)
		{
			int iGrayValue = clsSouceImage.at<unsigned char>(iRowIndex, iColIndex);
			clsTargetImage.at<unsigned char>(iRowIndex, iColIndex) = fTransformRatioDatas[iGrayValue];
		}
	}
	if (bShowTargetImage)
	{
		cv::namedWindow("Set Histogram TargetImage", cv::WINDOW_AUTOSIZE);
		cv::imshow("Set Histogram TargetImage", clsTargetImage);
	}
}

void Histogram::SetReverseValueToImage(Mat clsSouceImage, Mat& clsTargetImage, std::array<float, 256> fTransformRatioDatas, bool bShowTargetImage)
{

	if (clsTargetImage.empty())
	{
		Mat clsTargetImage = Mat::zeros(clsSouceImage.rows, clsSouceImage.cols, CV_8UC1);
	}

	for (int iColIndex = 0; iColIndex < clsSouceImage.cols; iColIndex++)
	{
		for (int iRowIndex = 0; iRowIndex < clsSouceImage.rows; iRowIndex++)
		{
			int iGrayValue = clsSouceImage.at<unsigned char>(iRowIndex, iColIndex);

			//Get Reverse Valueint  
			int DataIndex = 0;
			for (DataIndex = 0; DataIndex < fTransformRatioDatas.size(); DataIndex++)
			{
				if (iGrayValue == fTransformRatioDatas[DataIndex])
				{
					clsTargetImage.at<unsigned char>(iRowIndex, iColIndex) = DataIndex;
					break;
				}
				else if (iGrayValue > fTransformRatioDatas[DataIndex])
				{
					clsTargetImage.at<unsigned char>(iRowIndex, iColIndex) = DataIndex;
					break;
				}
			}
		}
	}
	if (bShowTargetImage)
	{
		cv::namedWindow("ReverseTargetImage", cv::WINDOW_AUTOSIZE);
		cv::imshow("ReverseTargetImage", clsTargetImage);
	}
}

void Histogram::HistogramEqualization(int iPixelCount, std::array<int, 256> iSouceDatas, std::array<int, 256>& fTransformRatioDatas)
{
	//int iTransformDatas[256];
	if (fTransformRatioDatas.empty())
	{
		std::array<float, 256> fTransformRatioDatas;
		fTransformRatioDatas.fill(0);
	}

	std::array<int, 256> fTransformResultDatas;
	fTransformResultDatas.fill(0);

	float iPrevious = iSouceDatas[0];

	for (int i = 0; i < iSouceDatas.size(); i++)
	{
		iPrevious += iSouceDatas[i];

		fTransformRatioDatas[i] = 255 * (iPrevious / iPixelCount);

		fTransformResultDatas[i] = iSouceDatas[i] * fTransformRatioDatas[i];
	}

	cv::Mat clsRatioHistogramImage = cv::Mat::zeros(500, 255, CV_8UC1);
	DrawHistogram("Ratio", clsRatioHistogramImage, fTransformRatioDatas, 1, true);
}

void Histogram::Matching(Mat clsSouceImage, Mat clsTargerImage)
{
	//Souce T
	std::array<int, 256> iSouceImageHistogram;
	GetImageHistogram(clsSouceImage, iSouceImageHistogram);
	std::array<int, 256> iSouceImageTransformHistogram;
	HistogramEqualization(clsSouceImage.rows*clsSouceImage.cols, iSouceImageHistogram, iSouceImageTransformHistogram);
	Mat clsSouceHistogramImage = Mat::zeros(500, 256, CV_8UC1);
	DrawHistogram("SouceHistogram", clsSouceHistogramImage, iSouceImageHistogram, 0.1, true);

	//Target G
	std::array<int, 256> iTargetImageHistogram;
	GetImageHistogram(clsTargerImage, iTargetImageHistogram);
	std::array<int, 256> iTargetImageTransformHistogram;
	HistogramEqualization(clsTargerImage.rows*clsTargerImage.cols, iTargetImageHistogram, iTargetImageTransformHistogram);
	Mat clsTargetHistogramImage = Mat::zeros(500, 256, CV_8UC1);
	DrawHistogram("TargetHistogramImage", clsTargetHistogramImage, iTargetImageHistogram, 0.1, true);

	//Reverse
	for (int iIndex = 0; iIndex < iSouceImageTransformHistogram.size(); iIndex++)
	{
		int iDataIndex = 0;
		int iPreValue = 0;

		for (iDataIndex = 0; iDataIndex < iTargetImageTransformHistogram.size(); iDataIndex++)
		{
			if (iSouceImageTransformHistogram[iIndex] == iTargetImageTransformHistogram[iDataIndex])
			{
				iSouceImageTransformHistogram[iIndex] = iDataIndex;
				break;
			}
			else if (iTargetImageTransformHistogram[iDataIndex] > iSouceImageTransformHistogram[iIndex])
			{
				//Calculate Near Value
				if ((abs(iPreValue - iSouceImageTransformHistogram[iIndex])) < abs((iTargetImageTransformHistogram[iDataIndex] - iSouceImageTransformHistogram[iIndex])))
					iSouceImageTransformHistogram[iIndex] = iDataIndex - 1;
				else
					iSouceImageTransformHistogram[iIndex] = iDataIndex;
				break;
			}
			iPreValue = iTargetImageTransformHistogram[iDataIndex];

		}
	}

	//Matching
	Mat clsResultImage = Mat::zeros(clsSouceImage.rows, clsSouceImage.cols, CV_8UC1);
	SetValueToImage(clsSouceImage, clsResultImage, iSouceImageTransformHistogram, true);
	Mat clsResultHistogramImage = Mat::zeros(500, 256, CV_8UC1);
	DrawHistogram("ResultHistogram", clsResultHistogramImage, iSouceImageTransformHistogram, 0.1, true);
}