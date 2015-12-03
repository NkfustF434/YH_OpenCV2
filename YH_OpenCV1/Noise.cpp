#include "Noise.h"
#include "AdvanceMath.h"
#include <stdlib.h>
#include <time.h>

void Noise::SetGaussainPoroperties(float fGaussianMean, float fGaussianVarance)
{
	g_fGaussianMean = fGaussianMean;
	g_fGaussianVarance = fGaussianVarance;
}

void Noise::AddNoise(NoiseKind eNoiseKind, Mat clsSouceImage, Mat& clsTargetImage)
{
	srand(time(NULL));
	int iGaussianValue[11];
	if (eNoiseKind == Noise::NoiseKind::Gaussian)
	{
		int iIndex = 0;
		for (int i = -5; i < 6; i++)
		{
			//Gaussian -5~5 		
			iGaussianValue[iIndex] = AdvanceMath::Gaussian(i, g_fGaussianMean, g_fGaussianVarance) * 100;
			iIndex++;
		}
	}

	for (int iColIndex = 0; iColIndex < clsSouceImage.cols; iColIndex++)
	{
		for (int iRowIndex = 0; iRowIndex < clsSouceImage.rows; iRowIndex++)
		{
			int iSourceValue = clsSouceImage.at<unsigned char>(iRowIndex, iColIndex);
			int iRandValue = 0;
			int iNewValue = 0;

			int iPreSum = 0;

			switch (eNoiseKind)
			{
			case Noise::NoiseKind::Gaussian:
				iRandValue = rand() % 101;
				iNewValue = iSourceValue;
				for (int iIndex = 0; iIndex < 11; iIndex++)
				{
					if (iPreSum < iRandValue &&  iRandValue < iPreSum + iGaussianValue[iIndex])
					{
						iNewValue = iSourceValue + iIndex - 5;
						break;
					}
					iPreSum += iGaussianValue[iIndex];
				}

				break;
			case Noise::NoiseKind::SaltAndPepper:

				iRandValue = rand() % 11;

				if (iRandValue == 0)
					iNewValue = 0;
				else if (iRandValue == 1)
					iNewValue = 255;
				else
					iNewValue = iSourceValue;

				break;
			case Noise::NoiseKind::Uniform:
				iRandValue = (rand() % 21) - 10;
				iNewValue = iSourceValue + iRandValue;
				break;
			default:
				break;
			}

			if (iNewValue < 0)iNewValue = 0;
			else if (iNewValue > 255)iNewValue = 255;

			clsTargetImage.at<unsigned char>(iRowIndex, iColIndex) = iNewValue;
		}
	}

}