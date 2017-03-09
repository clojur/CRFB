#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   Z型钢束类


****************************************************************************************/

class CDIP_BAR_API CBarZSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarZSteelBar)
public:
	CBarZSteelBar();
	CBarZSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	virtual ~CBarZSteelBar();
	virtual void Serialize(CArchive& ar);

	//得到任意里程 X 值时的钢束距离底板底面的距离
	virtual double getYAtX(double x);

	//锚固长度检查
	void CheckAnchorageLength();

	//构造显示用钢束点集
	void createPtArray();

};


