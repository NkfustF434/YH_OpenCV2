#include "Morphology.h"
#include <stdlib.h>
#include "Thresholding.h"
#include <utility>  // make_pair()

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

void Morphology::ConnectComponent(Mat clsSouceImage, std::vector<Rect>& clsRectsList, int iMinArea, bool bEightConnect, bool bPrint)
{
	Mat clsTempImage = clsSouceImage.clone();
	int** iImageTable;
	iImageTable = new int*[clsSouceImage.rows];
	for (int iRowIndex = 0; iRowIndex < clsSouceImage.rows; iRowIndex++)
	{
		iImageTable[iRowIndex] = new int[clsSouceImage.cols];
	}

	// check this image is binary
	if (!CheckIsBinary(clsSouceImage))
	{
		Thresholding clsThresholding;
		clsThresholding.OtsusThersholding(clsSouceImage, clsTempImage);
	}

	//Initial label
	std::map<int, int> iRelationTable;
	int iLebelIndex = 1;
	for (int iRow = 0; iRow < clsSouceImage.rows; iRow++)
	{
		for (int iCol = 0; iCol < clsSouceImage.cols; iCol++)
		{
			if (clsSouceImage.at<unsigned char>(iRow, iCol) != 0)
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
						else
						{
							if (iImageTable[iRow - 1][iCol] != iMinLebel)
								iRelationTable[iImageTable[iRow - 1][iCol]] = iMinLebel;
						}
					}
				}

				if (iCol != 0)
				{
					if (iImageTable[iRow][iCol - 1] != 0)
					{
						bTopRelation = true;

						if (iImageTable[iRow][iCol - 1] < iMinLebel)
						{
							if (iMinLebel != iLebelIndex)
							{
								iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
							}
							iMinLebel = iImageTable[iRow][iCol - 1];

							bNewLebel = false;
						}
						else
						{

							if (iImageTable[iRow][iCol - 1] != iMinLebel)
								iRelationTable[iImageTable[iRow][iCol - 1]] = iMinLebel;							
						}
					}
					//----------------------//
					/*if (iImageTable[iRow][iCol - 1] != 0)
					{
					bLeftRelation = true;
					if (iImageTable[iRow][iCol - 1] < iMinLebel)
					{
					if (!bNewLebel)
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel]>iImageTable[iRow][iCol - 1])
					iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
					}
					}
					iMinLebel = iImageTable[iRow][iCol - 1];
					bNewLebel = false;
					}
					else
					{
					if (iMinLebel != iImageTable[iRow][iCol - 1])
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel] > iImageTable[iRow][iCol - 1])
					iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
					}
					}
					}
					}*/
					/*
					if (bEightConnect)
					{
					if (iRow != 0)
					{
					if (iImageTable[iRow - 1][iCol - 1] != 0)
					{
					bLeftRelation = true;
					if (iImageTable[iRow - 1][iCol - 1] < iMinLebel)
					{
					if (!bNewLebel)
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel]>iImageTable[iRow - 1][iCol - 1])
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol - 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					}
					iMinLebel = iImageTable[iRow - 1][iCol - 1];
					bNewLebel = false;
					}
					else
					{
					if (iMinLebel != iImageTable[iRow - 1][iCol - 1])
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel] > iImageTable[iRow - 1][iCol - 1])
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol - 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					}
					}
					}

					if (iCol < clsTempImage.cols - 1)
					{
					if (iImageTable[iRow - 1][iCol + 1] != 0)
					{
					bLeftRelation = true;
					if (iImageTable[iRow - 1][iCol + 1] < iMinLebel)
					{
					if (!bNewLebel)
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel]>iImageTable[iRow - 1][iCol + 1])
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					}
					iMinLebel = iImageTable[iRow - 1][iCol + 1];
					bNewLebel = false;
					}
					else
					{
					if (iMinLebel != iImageTable[iRow - 1][iCol + 1])
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel] > iImageTable[iRow - 1][iCol + 1])
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					}
					}
					}
					}
					}
					}*/
				}
				if (iMinLebel < 0)
					int xxxx = 0;
				iImageTable[iRow][iCol] = iMinLebel;

				if (bNewLebel)iLebelIndex++;
			}
			else
			{
				iImageTable[iRow][iCol] = 0;
			}
			//printf("%d ", iImageTable[iRow][iCol]);
		}
		//printf("\n");
	}

	//Relation
	/*for (auto iLebel : iRelationTable)
	{*/
	for (int iLebel = iLebelIndex; iLebel > 0; iLebel--)
	{
		if (iRelationTable.count(iLebel) != 0)
		{
			//system("CLS");
			for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
			{
				for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
				{
					if (iImageTable[iRow][iCol] == iLebel)
						iImageTable[iRow][iCol] = iRelationTable[iLebel];
					//printf("%d ", iImageTable[iRow][iCol]);
				}
				//printf("\n");
			}
		}
	}


	//Print
	if (bPrint)
	{
		int iCols = 255 / (iLebelIndex - iRelationTable.size());
		for (int iColIndex = 0; iColIndex < iLebelIndex; iColIndex++)
		{
			for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
			{
				for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
				{
					if (iColIndex == iImageTable[iRow][iCol])
						clsTempImage.at<unsigned char>(iRow, iCol) = iImageTable[iRow][iCol] * iCols+50;
					//printf("%d ", iImageTable[iRow][iCol]);
				}
				//printf("\n");
			}

		}
	}
	imshow("ConnectedCompronent", clsTempImage);
	printf("%d Block\n", iLebelIndex - iRelationTable.size());
	//Get Roi
	int iGetRoiIndex = 1;

	while (iGetRoiIndex <= iLebelIndex)
	{
		int iLeftX = 9999999;
		int iLeftY = 9999999;
		int iBotX = 0;
		int iBotY = 0;
		Rect clsRoi;
		bool bLeftTop = true;
		for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
		{
			for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
			{
				if (iImageTable[iRow][iCol] == iGetRoiIndex)
				{

					if (iCol < iLeftX)
						iLeftX = iCol;
					if (iRow < iLeftY)
						iLeftY = iRow;

					if (iCol > iBotX)
						iBotX = iCol;
					if (iRow > iBotY)
						iBotY = iRow;
				}
			}
		}

		clsRoi.x = iLeftX;
		clsRoi.y = iLeftY;
		clsRoi.width = iBotX - iLeftX;
		clsRoi.height = iBotY - iLeftY;
		if (clsRoi.height > 0 && clsRoi.width > 0)
		{
			if (clsRoi.width*clsRoi.height > iMinArea)
				clsRectsList.push_back(clsRoi);
		}
		iGetRoiIndex++;
	}

	//Clear array
	for (int i = 0; i < clsTempImage.rows; i++)
	{
		delete[] iImageTable[i];
	}
	delete[] iImageTable;
}

void Morphology::ConnectComponent2(Mat clsSouceImage, std::vector<Rect>& clsRectsList, int iMinArea, bool bEightConnect, bool bPrint)
{
	Mat clsTempImage = clsSouceImage.clone();
	int** iImageTable;
	iImageTable = new int*[clsSouceImage.rows];
	for (int iRowIndex = 0; iRowIndex < clsSouceImage.rows; iRowIndex++)
	{
		iImageTable[iRowIndex] = new int[clsSouceImage.cols];
	}

	// check this image is binary
	if (!CheckIsBinary(clsSouceImage))
	{
		Thresholding clsThresholding;
		clsThresholding.OtsusThersholding(clsSouceImage, clsTempImage);
	}

	//Initial label
	std::map<int, int> iRelationTable;
	int iLebelIndex = 1;
	for (int iRow = 0; iRow < clsSouceImage.rows; iRow++)
	{
		for (int iCol = 0; iCol < clsSouceImage.cols; iCol++)
		{
			if (clsSouceImage.at<unsigned char>(iRow, iCol) != 0)
			{
				if (iRow == 2)
					int ixx = 0;
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
						else
						{
							if (iImageTable[iRow - 1][iCol] != iMinLebel)
								iRelationTable[iImageTable[iRow - 1][iCol]] = iMinLebel;
						}
					}
				}

				if (iCol != 0)
				{
					if (iImageTable[iRow][iCol - 1] != 0)
					{
						bTopRelation = true;

						if (iImageTable[iRow][iCol - 1] < iMinLebel)
						{
							if (iMinLebel != iLebelIndex)
							{
								iRelationTable.insert(std::make_pair(iImageTable[iRow][iCol - 1], iMinLebel));
							}
							iMinLebel = iImageTable[iRow][iCol - 1];

							bNewLebel = false;
						}
						else
						{
							if (iImageTable[iRow][iCol - 1] != iMinLebel)
								iRelationTable.insert(std::make_pair(iImageTable[iRow][iCol - 1], iMinLebel));
						}
					}
					//----------------------//
					/*if (iImageTable[iRow][iCol - 1] != 0)
					{
					bLeftRelation = true;
					if (iImageTable[iRow][iCol - 1] < iMinLebel)
					{
					if (!bNewLebel)
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel]>iImageTable[iRow][iCol - 1])
					iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
					}
					}
					iMinLebel = iImageTable[iRow][iCol - 1];
					bNewLebel = false;
					}
					else
					{
					if (iMinLebel != iImageTable[iRow][iCol - 1])
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel] > iImageTable[iRow][iCol - 1])
					iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow][iCol - 1];
					}
					}
					}
					}*/
					/*
					if (bEightConnect)
					{
					if (iRow != 0)
					{
					if (iImageTable[iRow - 1][iCol - 1] != 0)
					{
					bLeftRelation = true;
					if (iImageTable[iRow - 1][iCol - 1] < iMinLebel)
					{
					if (!bNewLebel)
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel]>iImageTable[iRow - 1][iCol - 1])
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol - 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					}
					iMinLebel = iImageTable[iRow - 1][iCol - 1];
					bNewLebel = false;
					}
					else
					{
					if (iMinLebel != iImageTable[iRow - 1][iCol - 1])
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel] > iImageTable[iRow - 1][iCol - 1])
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol - 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					}
					}
					}

					if (iCol < clsTempImage.cols - 1)
					{
					if (iImageTable[iRow - 1][iCol + 1] != 0)
					{
					bLeftRelation = true;
					if (iImageTable[iRow - 1][iCol + 1] < iMinLebel)
					{
					if (!bNewLebel)
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel]>iImageTable[iRow - 1][iCol + 1])
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					}
					iMinLebel = iImageTable[iRow - 1][iCol + 1];
					bNewLebel = false;
					}
					else
					{
					if (iMinLebel != iImageTable[iRow - 1][iCol + 1])
					{
					//check key
					if (iRelationTable.count(iMinLebel) != 0)
					{
					if (iRelationTable[iMinLebel] > iImageTable[iRow - 1][iCol + 1])
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					else
					{
					iRelationTable[iMinLebel] = iImageTable[iRow - 1][iCol + 1];
					}
					}
					}
					}
					}
					}
					}*/
				}
				if (iMinLebel < 0)
					int xxxx = 0;
				iImageTable[iRow][iCol] = iMinLebel;

				if (bNewLebel)iLebelIndex++;
			}
			else
			{
				iImageTable[iRow][iCol] = 0;
			}
			//printf("%d ", iImageTable[iRow][iCol]);
		}
		//printf("\n");
	}

	//Relation
	/*for (auto iLebel : iRelationTable)
	{*/
	for (int iLebel = iLebelIndex; iLebel > 0; iLebel--)
	{
		if (iRelationTable.count(iLebel) != 0)
		{
			for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
			{
				for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
				{
					iImageTable[iRow][iCol] == iRelationTable[iLebel];
				}
			}
		}
	}

	//Print
	if (true)
	{
		int iCols = 255 / iLebelIndex;
		for (int iColIndex = 0; iColIndex < iLebelIndex; iColIndex++)
		{
			for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
			{
				for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
				{
					if (iColIndex == iImageTable[iRow][iCol])
						clsTempImage.at<unsigned char>(iRow, iCol) = iImageTable[iRow][iCol] * iCols;
					//printf("%d ", iImageTable[iRow][iCol]);
				}
				//printf("\n");
			}

		}
	}
	imshow("CC", clsTempImage);
	//Get Roi
	int iGetRoiIndex = 1;

	while (iGetRoiIndex <= iLebelIndex)
	{
		int iLeftX = 9999999;
		int iLeftY = 9999999;
		int iBotX = 0;
		int iBotY = 0;
		Rect clsRoi;
		bool bLeftTop = true;
		for (int iRow = 0; iRow < clsTempImage.rows; iRow++)
		{
			for (int iCol = 0; iCol < clsTempImage.cols; iCol++)
			{
				if (iImageTable[iRow][iCol] == iGetRoiIndex)
				{

					if (iCol < iLeftX)
						iLeftX = iCol;
					if (iRow < iLeftY)
						iLeftY = iRow;

					if (iCol > iBotX)
						iBotX = iCol;
					if (iRow > iBotY)
						iBotY = iRow;
				}
			}
		}

		clsRoi.x = iLeftX;
		clsRoi.y = iLeftY;
		clsRoi.width = iBotX - iLeftX;
		clsRoi.height = iBotY - iLeftY;
		if (clsRoi.height > 0 && clsRoi.width > 0)
		{
			if (clsRoi.width*clsRoi.height > iMinArea)
				clsRectsList.push_back(clsRoi);
		}
		iGetRoiIndex++;
	}

	//Clear array
	for (int i = 0; i < clsTempImage.rows; i++)
	{
		delete[] iImageTable[i];
	}
	delete[] iImageTable;
}
