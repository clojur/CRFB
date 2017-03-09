#pragma once
#include "BarSteelBar.h"

/***************************************************************************************


                                   X型钢束类


****************************************************************************************/

class CDIP_BAR_API CBarXSteelBar : public CBarSteelBar
{
	DECLARE_SERIAL(CBarXSteelBar)
public:
	CBarXSteelBar();
	CBarXSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR);
	~CBarXSteelBar();
	virtual void Serialize(CArchive& ar);

	//构造显示用钢束点集(构造沿前进方向左腹板那组)
	void CBarXSteelBar::createPtArray();

	//根据基本参数计算其他细部参数
	virtual void CalDetailProPara();
	void CalDetailProPara_Mbar();

	//得到任意里程 X 值时的钢束 Y 值(Y为距离顶板距离)
	double getYAtX(double x);
	//得到任意里程 X 值时的钢束 Y 值(Y为距离顶板距离)
	double getYAtX_M(double x);

	//锚固长度检查
	void CheckAnchorageLength();

	bool m_IsXmbar;

};


