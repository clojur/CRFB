#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   B型钢束类


****************************************************************************************/

class CDIP_BAR_API CBarBSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarBSteelBar)
public:
	CBarBSteelBar();
	CBarBSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	virtual ~CBarBSteelBar();
	virtual void Serialize(CArchive& ar);

	//得到任意里程 X 值时的钢束距离边跨底板的距离
	virtual double getYAtX(double x);

	//锚固长度检查
	void CheckAnchorageLength();

	//构造显示用钢束点集
	void createPtArray();


};


