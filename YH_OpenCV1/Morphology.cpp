#include "Morphology.h"
#include <stdlib.h>
#include "Thresholding.h"

bool Morphology::CheckIsBinary(Mat clsSouceImage)
{
	for (int iRow = 0; iRow < clsSouceImage.rows; iRow++)
	{
		for (int iCol = 0; iCol < clsSouceImage.cols; iCol++)
		{
			if (clsSouceImage.at<unsigned char>(iRow, iCol) != 0 && clsSouceImage.at<unsigned char>(iRow, iCol) != 255)
				return false;
		}
	}

	return true;
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

	for (int iRow = 1; iRow < clsSouceImage.rows - 1; iRow++)
	{
		for (int iCol = 1; iCol < clsSouceImage.cols - 1; iCol++)
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
			if (iIndex == 1 || iIndex == 3 || iIndex == 4 || iIndex == 5 || iIndex == 7)
				iKernel[iIndex] = 1;
			else
				iKernel[iIndex] = 0;
		}
	}

	for (int iRow = 1; iRow < clsSouceImage.rows - 1; iRow++)
	{
		for (int iCol = 1; iCol < clsSouceImage.cols - 1; iCol++)
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

void Morphology::ConnectComponent(Mat clsSouceImage, std::vector<Rect>& clsRectsList, bool bPrint)
{
	Mat clsTempImage = clsSouceImage.clone();

	int** iImageTable;
	iImageTable = new int*[clsTempImage.rows];
	for (int iRowIndex = 0; iRowIndex < clsTempImage.rows; iRowIndex++)
	{
		iImageTable[iRowIndex] = new int[clsTempImage.cols];
	}

	// check this image is binary
	if (!CheckIsBinary(clsSouceImage))
	{
		Thresholding clsThresholding;
		clsThresholding.OtsusThersholding(clsSouceImage, clsTempImage);
	}

	std::map<int, int> iRelationTable;
	int iLebelIndex = 1;
	for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
	{
		for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
		{
			if (clsTempImage.at<unsigned char>(iRow, iCol) != 0)
			{
				bool bNewLebel = true;
				bool bTopRelation = false;
				bool bLeftRelation = false;
				int iMinLebel = iLebelIndex;

				if (iRow != 0)
				{
					if (iImageTable[iRow - 1][iCol] != 0)
					{
						bTopRelation = true;
						if (iImageTable[iRow - 1][iCol] < iMinLebel)
						{
							iMinLebel = iImageTable[iRow - 1][iCol];
							bNewLebel = false;
						}
					}
				}

				if (iCol != 0)
				{
					if (iImageTable[iRow][iCol - 1] != 0)
					{
						bLeftRelation = true;
						if (iImageTable[iRow][iCol - 1] < iMinLebel)
						{
							iMinLebel = iImageTable[iRow][iCol - 1];
							bNewLebel = false;
						}
					}
				}

				//Check relation
				if (bLeftRelation && bTopRelation)
				{
					if (iImageTable[iRow][iCol - 1] != iMinLebel)
						iRelationTable[iImageTable[iRow][iCol - 1]] = iMinLebel;
					else if (iImageTable[iRow - 1][iCol] != iMinLebel)
						iRelationTable[iImageTable[iRow - 1][iCol]] = iMinLebel;
				}

				iImageTable[iRow][iCol] = iMinLebel;

				if (bNewLebel)iLebelIndex++;
			}
			else
			{
				iImageTable[iRow][iCol] = 0;
			}
		}
	}

	//Relation
	for (auto iLebelIndex : iRelationTable)
	{
		for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
		{
			for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
			{
				if (iImageTable[iRow][iCol] == iLebelIndex.first)
				{
					iImageTable[iRow][iCol] = iLebelIndex.second;
				}
			}
		}
	}

	//Print
	if (bPrint)
	{
		for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
		{
			for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
			{
				printf("%d ", iImageTable[iRow][iCol]);
			}
			printf("\n");
		}
	}

	//Get Roi
	int iGetRoiIndex = 1;

	while (iGetRoiIndex <= iLebelIndex)
	{
		Rect clsRoi;
		bool bLeftTop = true;
		for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
		{
			for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
			{
				if (iImageTable[iRow][iCol] == iGetRoiIndex)
				{
					if (bLeftTop)
					{
						if (iCol != 0)
							clsRoi.x = iCol - 1;
						else
							clsRoi.x = iCol;
						if (iRow != 0)
							clsRoi.y = iRow - 1;
						else
							clsRoi.y = iRow;

						clsRoi.height = 2;
						clsRoi.width = 2;
						bLeftTop = false;
					}
					else
					{
						if (iRow - clsRoi.y + 1 < clsTempImage.rows)
							clsRoi.height = iRow - clsRoi.y + 1;
						else
							clsRoi.height = iRow - clsRoi.y;

						if (iCol - clsRoi.x + 1 < clsTempImage.cols)
							clsRoi.width = iCol - clsRoi.x + 1;
						else
							clsRoi.width = 2;
					}
				}
			}
		}
		if (clsRoi.height != 0 && clsRoi.width != 0)
			clsRectsList.push_back(clsRoi);
		iGetRoiIndex++;
	}

	//Clear array
	for (int i = 0; i < clsTempImage.rows; i++)
	{
		delete[] iImageTable[i];
	}
	delete[] iImageTable;
}