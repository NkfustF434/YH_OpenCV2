#include "DipHomework.h"


Mat DipHomework::LoadImage(bool bShowMessage, string strTitle, bool bClearView){

	if (bClearView)
		system("CLS");

	if (bShowMessage)
	{
		printf("%s\n", strTitle.c_str());
		printf("*************************\n");
		printf("1.Lena\n");
		printf("2.White Bean\n");
		printf("3.Gray Beam\n");
		printf("4.Black Beam\n");
		printf("5.Pepersalt\n");
		printf("6.NoiseTestImage\n");
		printf("7.BlackWhitePanel\n");
		printf("99.Other\n");
		printf("*************************\n");
	}

	int iInput = 999;
	scanf("%d", &iInput);

	if (iInput == LENA_INDEX)
	{
		Mat clsImage = imread("Lena.bmp", cv::IMREAD_GRAYSCALE);
		return clsImage;
	}
	else if (iInput == White_BEAN_INDEX)
	{
		Mat clsImage = imread("WhiteBean.bmp", cv::IMREAD_GRAYSCALE);
		return clsImage;
	}
	else if (iInput == GRAY_BEAN_INDEX)
	{
		Mat clsImage = imread("GrayBean.bmp", cv::IMREAD_GRAYSCALE);
		return clsImage;
	}
	else if (iInput == BLACK_BEAN_INDEX)
	{
		Mat clsImage = imread("BlackBean.bmp", cv::IMREAD_GRAYSCALE);
		return clsImage;
	}
	else if (iInput == PEPERSALT_INDEX)
	{
		Mat clsImage = imread("Pepersalt.bmp", cv::IMREAD_GRAYSCALE);
		return clsImage;
	}
	else if (iInput == NOISETEST_INDEX)
	{
		Mat clsImage = imread("NoiseTestImage.bmp", cv::IMREAD_GRAYSCALE);
		return clsImage;
	}
	else if (iInput == MORPHOLOGY_INDEX)
	{
		Mat clsImage = imread("BlackWhitePanel.bmp", cv::IMREAD_GRAYSCALE);
		return clsImage;
	}
	else if (iInput == OTHER_INDEX)
	{
		Mat clsImage = imread("Other.bmp", cv::IMREAD_GRAYSCALE);
		return clsImage;
	}
	else
	{
		Mat clsImage = Mat::zeros(128, 128, CV_8UC1);
		return clsImage;
	}
}

int DipHomework::Menu()
{
	printf("*************************\n");
	printf("1.HstogramEqualization\n");
	printf("2.Matching\n");
	printf("3.Filter\n");
	printf("4.Noise\n");
	printf("5.Morphology\n");
	printf("6.Thresholding\n");
	printf("7.Connect Componnent\n");
	printf("*************************\n");

	int iInput = 999;
	scanf("%d", &iInput);

	return iInput;
}

void DipHomework::HistogramEqualization(){

	//Load Image
	Mat clsSouceImage = LoadImage(true, "Select SouceImage", true);
	if (clsSouceImage.empty())
	{
		clsSouceImage = cv::Mat::zeros(128, 128, CV_8UC1);
	}

	int iArrayCol = clsSouceImage.cols;
	int iArrayRow = clsSouceImage.rows;

	Histogram clsHistogram;

	std::array<int, 256> iSouceHistogramArray;
	iSouceHistogramArray.fill(0);

	//Souce
	cv::namedWindow("Souce", cv::WINDOW_AUTOSIZE);
	cv::imshow("Souce", clsSouceImage);

	clsHistogram.GetImageHistogram(clsSouceImage, iSouceHistogramArray);

	//Draw Histogram
	cv::Mat clsHistogramImage = cv::Mat::zeros(500, 256, CV_8UC1);
	clsHistogram.DrawHistogram("SouceHistogram", clsHistogramImage, iSouceHistogramArray, 0.1, true);


	std::array<int, 256> fTransformRatioDatas;
	clsHistogram.HistogramEqualization(iArrayCol*iArrayRow, iSouceHistogramArray, fTransformRatioDatas);

	cv::Mat clsTargetImage = cv::Mat::zeros(clsSouceImage.rows, clsSouceImage.cols, CV_8UC1);
	clsHistogram.SetValueToImage(clsSouceImage, clsTargetImage, fTransformRatioDatas, true);


	std::array<int, 256> iTargetHistogramArray;
	iTargetHistogramArray.fill(0);
	clsHistogram.GetImageHistogram(clsTargetImage, iTargetHistogramArray);
	Mat clsTargetHistogramImage = Mat::zeros(500, 256, CV_8UC1);
	clsHistogram.DrawHistogram("TargetHistogram", clsTargetHistogramImage, iTargetHistogramArray, 0.1, true);

}

void DipHomework::Matching(){
	//Load SouceImage
	Mat clsSouceImage = LoadImage(true, "Select SouceImage", true);
	Mat clsTargetImage = LoadImage(true, "Select TargetImage", true);

	imshow("SouceImage", clsSouceImage);
	imshow("TargetImage", clsTargetImage);

	Histogram clsHistogram;
	clsHistogram.Matching(clsSouceImage, clsTargetImage);
}

void DipHomework::Filter()
{
	Mat clsSouceImage = LoadImage(true, "Select SouceImage", true);
	Mat clsResultImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);

	system("CLS");
	printf("Filter Kind\n");
	printf("*************************\n");
	printf("1.BoxFilter\n");
	printf("2.LaplacianImageEnhancement\n");
	printf("3.MaxFilter\n");
	printf("4.MediaFiler\n");
	printf("5.MinFilter\n");
	printf("6.SolbeX\n");
	printf("7.SolbeY\n");

	printf("*************************\n");

	int iInput = 999;
	scanf("%d", &iInput);


	Filters::FilterKind eFilterKind;
	switch (iInput)
	{
	case 1:
		eFilterKind = Filters::FilterKind::BoxFilter;
		break;
	case 2:
		eFilterKind = Filters::FilterKind::LaplacianImageEnhancement;
		break;
	case 3:
		eFilterKind = Filters::FilterKind::MaxFilter;
		break;
	case 4:
		eFilterKind = Filters::FilterKind::MediaFiler;
		break;
	case 5:
		eFilterKind = Filters::FilterKind::MinFilter;
		break;
	case 6:
		eFilterKind = Filters::FilterKind::SolbeX;
		break;
	case 7:
		eFilterKind = Filters::FilterKind::SolbeY;
		break;
	default:
		eFilterKind = Filters::FilterKind::BoxFilter;
		break;
	}

	Filters::FilterProcess(eFilterKind, clsSouceImage, clsResultImage);
	imshow("SouceImage", clsSouceImage);
	imshow("FilterResult", clsResultImage);

}

void DipHomework::AddNoise()
{

	Mat clsSouceImage = LoadImage(true, "Select SouceImage", true);
	Mat clsResultImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);

	system("CLS");
	printf("Noise Kind\n");
	printf("*************************\n");
	printf("1.Gaussian\n");
	printf("2.Salt - and - Pepper\n");
	printf("3.Uniform\n");

	printf("*************************\n");

	int iInput = 999;
	scanf("%d", &iInput);

	Noise::NoiseKind eNoiseKind;

	switch (iInput)
	{
	case 1:
		eNoiseKind = Noise::NoiseKind::Gaussian;
		break;
	case 2:
		eNoiseKind = Noise::NoiseKind::SaltAndPepper;
		break;
	case 3:
		eNoiseKind = Noise::NoiseKind::Uniform;
		break;
	default:
		eNoiseKind = Noise::NoiseKind::Gaussian;
		break;
	}
	Noise clsNoiseAdditor;
	clsNoiseAdditor.SetGaussainPoroperties(0, 3);
	clsNoiseAdditor.AddNoise(eNoiseKind, clsSouceImage, clsResultImage);

	Histogram clsSourceHistogram;
	Mat clSourceHistogramImage = Mat::zeros(500, 300, CV_8UC1);
	std::array<int, 256> iSourceHistogramArray;
	clsSourceHistogram.GetImageHistogram(clsSouceImage, iSourceHistogramArray);
	clsSourceHistogram.DrawHistogram("SourceHistogram", clSourceHistogramImage, iSourceHistogramArray, 0.05, true);

	Histogram clsResultHistogram;
	Mat clsResultHistogramImage = Mat::zeros(500, 300, CV_8UC1);
	std::array<int, 256> iResultHistogramArray;
	clsResultHistogram.GetImageHistogram(clsResultImage, iResultHistogramArray);
	clsResultHistogram.DrawHistogram("ResultHistogram", clsResultHistogramImage, iResultHistogramArray, 0.05, true);

	imshow("SouceImage", clsSouceImage);
	imshow("NoiseResult", clsResultImage);

	Mat clsFilterImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);
	Filters::FilterProcess(Filters::FilterKind::AdaptiveMedianFilter, clsResultImage, clsFilterImage);
	imshow("FiltedImage", clsFilterImage);

	Histogram clsFilterHistogram;
	Mat clsFilterHistogramImage = Mat::zeros(500, 300, CV_8UC1);
	std::array<int, 256> iFilterHistogramArray;
	clsFilterHistogram.GetImageHistogram(clsFilterImage, iFilterHistogramArray);
	clsFilterHistogram.DrawHistogram("FilterHistogram", clsFilterHistogramImage, iFilterHistogramArray, 0.05, true);

}

void DipHomework::Morphological()
{
	Mat clsSouceImage = LoadImage(true, "Select SouceImage", true);
	Mat clsDilationImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);
	Mat clsErosiontImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);
	Mat clsOpeningImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);
	Mat clsClosingImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);

	Thresholding clsThresholding;
	Morphology clsMorphology;
	clsThresholding.NomalThersholding(clsSouceImage, clsSouceImage, 127);
	clsMorphology.Dilation(clsSouceImage, clsDilationImage);
	clsMorphology.Erosion(clsSouceImage, clsErosiontImage);
	clsMorphology.Opening(clsSouceImage, clsOpeningImage);
	clsMorphology.Closing(clsSouceImage, clsClosingImage);

	imshow("SouceImage", clsSouceImage);
	imshow("Dilation", clsDilationImage);
	imshow("Erosion", clsSouceImage);
	imshow("Opening", clsOpeningImage);
	imshow("Closing", clsClosingImage);
}

void DipHomework::Threshold()
{
	Mat clsSouceImage = LoadImage(true, "Select SouceImage", true);
	Mat clsResultImage = Mat::zeros(Size(clsSouceImage.cols, clsSouceImage.rows), CV_8UC1);

	Thresholding clsThresholding;
	clsThresholding.OtsusThersholding(clsSouceImage, clsResultImage);

	imshow("SouceImage", clsSouceImage);
	imshow("ResultImage", clsResultImage);
}

void DipHomework::ConnectCompronent()
{
	Mat clsSouceImage = imread("BlackWhitePanel.bmp", cv::IMREAD_GRAYSCALE);
	Mat clsTempImage = clsSouceImage.clone();
	Thresholding clsThresholding;
	Morphology clsMorphology;
	std::vector<Rect> clsRectsList;
	clsThresholding.OtsusThersholding(clsSouceImage, clsSouceImage);	
	clsMorphology.ConnectComponent(clsSouceImage, clsRectsList, 0, false,true);
	
	imshow("SouceImage", clsSouceImage);
}

void DipHomework::HwMain(){

	ConnectCompronent();/*
	int iSelectIndex = 0;
	do
	{
		//break;
		iSelectIndex = Menu();

		if (iSelectIndex == EXIT_CHANNEL)
		{
			break;
		}
		else	if (iSelectIndex == HISTOGRAM_EQUALIZATION_CHANNEL)
		{
			HistogramEqualization();
			break;
		}
		else if (iSelectIndex == MATCHING_CHANNEL)
		{
			Matching();
			break;
		}
		else if (iSelectIndex == FILTER_CHANNEL)
		{
			Filter();
			break;
		}
		else if (iSelectIndex == NOISE_CHANNEL)
		{
			AddNoise();
			break;
		}

		else if (iSelectIndex == MORPHOLOGY_CHANNEL)
		{
			Morphological();
			break;
		}

		else if (iSelectIndex == THRESHOLDING_CHANNEL)
		{
			Threshold();
			break;
		}
		else if (iSelectIndex == CONNECT_COMPONNENT_CHANNEL)
		{
			ConnectCompronent();
			break;
		}
	} while (true);*/
}