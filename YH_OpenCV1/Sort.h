#ifndef _SORT_H
#define _SORT_H

class Sort{

public:
	template <typename InputArray, int iCount>
	static void BubbleSort(InputArray(&iInputArray)[iCount])
	{
		for (int iRound = 0; iRound < iCount - 1; iRound++)
		{
			for (int iIndexL = 0; iIndexL < iCount - 1; iIndexL++)
			{
				if (iInputArray[iIndexL]> iInputArray[iIndexL + 1])
				{
					int iTemp = iInputArray[iIndexL + 1];
					iInputArray[iIndexL + 1] = iInputArray[iIndexL];
					iInputArray[iIndexL] = iTemp;
				}
			}
		}
	}

static void BubbleSort(std::vector<int>& iValueList)
{
	for (int iRound = 0; iRound < iValueList.size() - 1; iRound++)
	{
		for (int iIndexL = 0; iIndexL < iValueList.size() -1; iIndexL++)
		{
			if (iValueList[iIndexL]> iValueList[iIndexL + 1])
			{
				int iTemp = iValueList[iIndexL + 1];
				iValueList[iIndexL + 1] = iValueList[iIndexL];
				iValueList[iIndexL] = iTemp;
			}
		}
	}
}
};

#endif