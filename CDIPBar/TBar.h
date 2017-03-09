#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   T型钢束类


****************************************************************************************/

class CDIP_BAR_API CBarTSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarTSteelBar)
public:
	CBarTSteelBar();
	CBarTSteelBar(CMainGirder* gdrL, CMainGirder* grdR, int segL, int segR);
	~CBarTSteelBar();
	virtual void Serialize(CArchive& ar);

	//锚固长度检查
	void CheckAnchorageLength();

	//构造显示用钢束点集
	void createPtArray();

protected:

};


