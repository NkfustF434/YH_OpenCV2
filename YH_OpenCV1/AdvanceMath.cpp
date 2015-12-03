#define _USE_MATH_DEFINES
#include "AdvanceMath.h"
#include <stdio.h>
#include <math.h>

float AdvanceMath::Gaussian(float fInputX, float fMean, float fVariance)
{
	/*if (fInputX > fMax || fInputX < fMin)
	{
		return 0;
	}*/

	double fE = pow(M_E, (double)(-pow((fInputX - fMean), 2) / (2 * pow(fVariance, 2))));

	return (1 / (pow(2 * M_PI, 0.5)*fVariance))*fE;

}
