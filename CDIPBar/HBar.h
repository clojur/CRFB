#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   H型钢束类


****************************************************************************************/

class CDIP_BAR_API CBarHSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarHSteelBar)
public:
	CBarHSteelBar();
	CBarHSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	~CBarHSteelBar();
	virtual void Serialize(CArchive& ar);

	//锚固长度检查
	void CheckAnchorageLength();

	//构造显示用钢束点集
	void createPtArray();

};


