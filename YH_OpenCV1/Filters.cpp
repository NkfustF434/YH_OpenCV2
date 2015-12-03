#include <opencv2\highgui\highgui.hpp>
#include <array>
#include "Filters.h"
#include "Sort.h"

void Filters::FilterProcess(Filters::FilterKind eFilterKind, Mat clsSouceImage, Mat& clsTargetImage)
{

	clsTargetImage = clsSouceImage.clone();


	if (eFilterKind == Filters::FilterKind::BoxFilter)
	{
		int iFilter[3][3] = {
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 1 } };

		Filters::CustomFilterProcess(iFilter, clsSouceImage, clsTargetImage);
	}
	else if (eFilterKind == Filters::FilterKind::LaplacianImageEnhancement)
	{
		int iFilter[3][3] = {
			{ 0, -1, 0 },
			{ -1, 5, -1 },
			{ 0, -1, 0 } };

		Filters::CustomFilterProcess(iFilter, clsSouceImage, clsTargetImage);
	}
	else if (eFilterKind == Filters::FilterKind::SolbeX)
	{
		int iFilter[3][3] = {
			{ -1, -2, -1 },
			{ 0, 0, 0 },
			{ 1, 2, 1 } };

		Filters::CustomFilterProcess(iFilter, clsSouceImage, clsTargetImage);
	}
	else if (eFilterKind == Filters::FilterKind::SolbeY)
	{
		int iFilter[3][3] = {
			{ -1, 0, 1 },
			{ -2, 0, 2 },
			{ -1, 0, 1 } };

		Filters::CustomFilterProcess(iFilter, clsSouceImage, clsTargetImage);
	}
	else if (eFilterKind == FilterKind::MediaFiler)
	{
		Filters::SortFilterProcess(GetType::Mid, clsSouceImage, clsTargetImage);
	}
	else if (eFilterKind == FilterKind::MaxFilter)
	{
		Filters::SortFilterProcess(GetType::Max, clsSouceImage, clsTargetImage);
	}
	else if (eFilterKind == FilterKind::MinFilter)
	{
		Filters::SortFilterProcess(GetType::Min, clsSouceImage, clsTargetImage);
	}
	else if (eFilterKind == FilterKind::AdaptiveMedianFilter)
	{
		Filters::AdaptMedianFilter(clsSouceImage, clsTargetImage);
	}
}

void Filters::CustomFilterProcess(int iFilter[3][3], Mat clsSouceImage, Mat& clsTargetImage)
{
	for (int iCol = 1; iCol < clsSouceImage.cols - 1; iCol++)
	{
		for (int iRow = 1; iRow < clsSouceImage.rows - 1; iRow++)
		{
			int iGrayValueSum = 0;
			int iWeightSum = 0;

			for (int iFilterRowIndex = 0; iFilterRowIndex < 3; iFilterRowIndex++)
			{
				for (int iFilterColIndex = 0; iFilterColIndex < 3; iFilterColIndex++)
				{
					iWeightSum += iFilter[iFilterColIndex][iFilterRowIndex];
					iGrayValueSum += iFilter[iFilterColIndex][iFilterRowIndex] * clsSouceImage.at<unsigned char>(iRow + (iFilterRowIndex - 1), iCol + (iFilterColIndex - 1));
				}
			}
			if (iWeightSum != 0)
				iGrayValueSum /= iWeightSum;
			if (iGrayValueSum < 0)iGrayValueSum = 0;
			else if (iGrayValueSum > 255) iGrayValueSum = 255;

			clsTargetImage.at<unsigned char>(iRow, iCol) = iGrayValueSum;
		}
	}
}

void Filters::SortFilterProcess(GetType eGetType, Mat clsSouceImage, Mat& clsTargetImage)
{
	for (int iCol = 1; iCol < clsSouceImage.cols - 1; iCol++)
	{
		for (int iRow = 1; iRow < clsSouceImage.rows - 1; iRow++)
		{
			int iArray[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			int iIndex = 0;

			for (int iFilterRowIndex = 0; iFilterRowIndex < 3; iFilterRowIndex++)
			{
				for (int iFilterColIndex = 0; iFilterColIndex < 3; iFilterColIndex++)
				{
					iArray[iIndex] = clsSouceImage.at<unsigned char>(iRow + (iFilterRowIndex - 1), iCol + (iFilterColIndex - 1));
					iIndex++;
				}
			}

			Sort::BubbleSort(iArray);
			int iValue = 0;
			switch (eGetType)
			{
			case GetType::Max:
				iValue = iArray[8];
				break;
			case GetType::Mid:
				iValue = iArray[4];
				break;
			case GetType::Min:
				iValue = iArray[0];
				break;
			}

			if (iValue < 0)iValue = 0;
			else if (iValue > 255) iValue = 255;
			clsTargetImage.at<unsigned char>(iRow, iCol) = iValue;
		}
	}
}

void Filters::AdaptMedianFilter(Mat clsSouceImage, Mat& clsTargetImage)
{
	for (int iCol = 1; iCol < clsSouceImage.cols - 1; iCol++)
	{
		for (int iRow = 1; iRow < clsSouceImage.rows - 1; iRow++)
		{
			int				iNewValue =0;
			bool bAllowMaxSize = false;
			std::vector<int> iValueList;
			int iWindowLevel = 0;
			int iMidValue = 0;
			int iMaxValue = 0;
			int iMinValue = 0;

			while (true)
			{

				for (int iFilterRowIndex = 0 - iWindowLevel; iFilterRowIndex < 3 + iWindowLevel; iFilterRowIndex++)
				{
					for (int iFilterColIndex = 0 - iWindowLevel; iFilterColIndex < 3 + iWindowLevel; iFilterColIndex++)
					{
						//Check is the max size
						if (iRow + iFilterRowIndex - 1 == 0 || iRow + iFilterRowIndex - 1 == clsSouceImage.rows - 1
							|| iCol + iFilterColIndex - 1 == 0 || iCol + iFilterColIndex - 1 == clsSouceImage.cols - 1)
							bAllowMaxSize = true;

						iValueList.push_back(clsSouceImage.at<unsigned char>(iRow + (iFilterRowIndex - 1), iCol + (iFilterColIndex - 1)));
						
					}
				}

				Sort::BubbleSort(iValueList);

				iMidValue = iValueList[iValueList.size() / 2];
				iMaxValue = iValueList[iValueList.size() - 1];
				iMinValue = iValueList[0];

				if ((iMidValue - iMinValue > 0 && iMidValue - iMaxValue < 0) || bAllowMaxSize == true)
				{
					int iCenterPixelValue = clsSouceImage.at<unsigned char>(iRow, iCol);
					if ((iCenterPixelValue - iMinValue) >0 && (iCenterPixelValue - iMaxValue) < 0)
					{
						//Not Noise
						iNewValue = iCenterPixelValue;
					}
					else
					{
						iNewValue = iMidValue;
					}
					break;
				}
				else if (iWindowLevel == 3)
				{
					iNewValue = clsSouceImage.at<unsigned char>(iRow, iCol);
					break;
				}
				else
				{
					iValueList.clear();
					iWindowLevel++;
				}
			}

			if (iNewValue < 0)iNewValue = 0;
			else if (iNewValue > 255) iNewValue = 255;
			clsTargetImage.at<unsigned char>(iRow, iCol) = iNewValue;
		}
	}
}
