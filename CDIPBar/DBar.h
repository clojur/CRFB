#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   D型钢束类


****************************************************************************************/

class CDIP_BAR_API CBarDSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarDSteelBar)
public:
	CBarDSteelBar();
	CBarDSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	~CBarDSteelBar();
	virtual void Serialize(CArchive& ar);

	//锚固长度检查
	void CheckAnchorageLength();

	//构造显示用钢束点集
	void createPtArray();

};


