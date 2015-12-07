#include "Morphology.h"
#include <stdlib.h>

void Morphology::Thersholding(Mat clsSouceImage, Mat& clsTargetImage, int iThersholding)
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

void Morphology::Dilation(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9])
{
	Mat clsTempImage = clsTargetImage.clone();
	if (iKernel == NULL)
	{
		iKernel = new int[9];
		for (int iIndex = 0; iIndex < 9; iIndex++)
		{
			if (iIndex == 1 || iIndex == 3 || iIndex == 4 || iIndex == 5 || iIndex == 7)
				iKernel[iIndex] = 1;
			else
				iKernel[iIndex] = 0;
		}
	}
	
	for (int iCol = 1; iCol < clsSouceImage.cols - 1; iCol++)
	{
		for (int iRow = 1; iRow < clsSouceImage.rows - 1; iRow++)
		{
			//Check image center 1 or 0
			if (iKernel[4] != 0 && clsSouceImage.at<unsigned char>(iRow, iCol) == 255)
			{
				for (int iKernelIndex = 0; iKernelIndex < 9; iKernelIndex++)
				{
					//Dilation
					if (iKernel[iKernelIndex] == 1)
						clsTempImage.at<unsigned char>(iRow + (iKernelIndex / 3) - 1, iCol + (iKernelIndex % 3) - 1) = 255;
				}
			}
			else if (iKernel[4] == 0 && clsSouceImage.at<unsigned char>(iRow, iCol) == 0)
			{
				for (int iKernelIndex = 0; iKernelIndex < 9; iKernelIndex++)
				{
					//Dilation
					if (iKernel[iKernelIndex] == 1)
						clsTempImage.at<unsigned char>(iRow + (iKernelIndex / 3) - 1, iCol + (iKernelIndex % 3) - 1) = 255;
				}
			}
		}
	}
	clsTargetImage = clsTempImage.clone();
}

void Morphology::Erosion(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9])
{
	Mat clsTempImage = clsTargetImage.clone();
	if (iKernel == NULL)
	{
		iKernel = new int[9];
		for (int iIndex = 0; iIndex < 9; iIndex++)
		{
			if (iIndex == 1 || iIndex == 3 || iIndex  == 4 || iIndex == 5 || iIndex == 7)
				iKernel[iIndex] = 1;
			else
				iKernel[iIndex] = 0;
		}
	}

	for (int iCol = 1; iCol < clsSouceImage.cols - 1; iCol++)
	{
		for (int iRow = 1; iRow < clsSouceImage.rows - 1; iRow++)
		{
			//Check image center 1 or 0
			if (iKernel[4] != 0 && clsSouceImage.at<unsigned char>(iRow, iCol) == 255)
			{
				bool bCheckOk = true;
				for (int iKernelIndex = 0; iKernelIndex < 9; iKernelIndex++)
				{
					//Erosion
					clsTempImage.at<unsigned char>(iRow, iCol) = 255;
					int iPixelValue = clsSouceImage.at<unsigned char>(iRow + (iKernelIndex / 3) - 1, iCol + (iKernelIndex % 3) - 1);
					if (iKernel[iKernelIndex] == 1 && iPixelValue != 255)
					{
						clsTempImage.at<unsigned char>(iRow, iCol) = 0;
						break;
					}
				}
			}
			else if (iKernel[4] == 0 && clsSouceImage.at<unsigned char>(iRow, iCol) == 0)
			{
				for (int iKernelIndex = 0; iKernelIndex < 3; iKernelIndex++)
				{
					//Erosion
					if (iKernel[iKernelIndex] == 1)
						clsTempImage.at<unsigned char>(iRow + (iKernelIndex / 3) - 1, iCol + (iKernelIndex % 3) - 1) = 255;
				}
			}


		}
	}
	clsTargetImage = clsTempImage.clone();
}

void Morphology::Opening(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9])
{
	Mat clsTemp = clsTargetImage.clone();
	Erosion(clsSouceImage, clsTemp, iKernel);
	Dilation(clsTemp, clsTargetImage, iKernel);
}

void Morphology::Closing(Mat clsSouceImage, Mat& clsTargetImage, int iKernel[9])
{
	Mat clsTemp = clsTargetImage.clone();
	Dilation(clsSouceImage, clsTemp, iKernel);
	Erosion(clsTemp, clsTargetImage, iKernel);
}
