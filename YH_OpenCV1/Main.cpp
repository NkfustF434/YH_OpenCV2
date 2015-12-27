#include <opencv2/opencv.hpp>
#include "DipHomework.h"

void main(){
	DipHomework clsDipHomework;
	clsDipHomework.HwMain();

	/*test Svm*/

	myImageSequence clsImageSequence = myImageSequence("D:/test","","bmp", false);
	clsImageSequence.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
	cv::Mat mPositive;
	std::vector<std::vector<float>> vvfPosFeture;
	while (clsImageSequence >> mPositive)
	{
		myFeatureExtractor clsFeatureExtractor = myFeatureExtractor(mPositive);
		clsFeatureExtractor.EnableFeature(myFeatureExtractor::Mode::LBP_FEATURE);
		std::vector<float > vValue;
		std::vector<float, std::allocator<float> > vfeature;
		for (int i = 0; i < mPositive.rows; i++)
		{
			for (size_t j = 0; j < mPositive.cols; j++)
			{
				clsFeatureExtractor.Describe(j, i, vfeature);
				for (auto value : vfeature)
				{
					vValue.push_back(value);
				}
			}
		}
		vvfPosFeture.push_back(vValue);
	}

	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	cv::SVM clsSvm;
	//clsSvm.save("D:/XsML.xml");
	//clsSvm.write)
	//CvFileStorage clsSorge;
	
	cv::waitKey(0);

}

