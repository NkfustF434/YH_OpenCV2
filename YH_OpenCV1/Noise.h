#ifndef _NOISE_H_
#define _NOISE_H_

#include <opencv2\highgui\highgui.hpp>
using namespace cv;

class Noise{

private:
	float g_fGaussianMean = 0;
	float g_fGaussianVarance = 3.0;

public:
	enum class NoiseKind
	{
		Gaussian,
		SaltAndPepper,
		Uniform
	};

	void AddNoise(NoiseKind eNoiseKind, Mat clsSouceImage, Mat& clsTargetImage);

	void SetGaussainPoroperties(float fGaussianMean, float fGaussianVarance);

};

#endif