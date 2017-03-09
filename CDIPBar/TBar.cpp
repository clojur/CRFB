
#include "stdafx.h"
#include "TBar.h"

IMPLEMENT_SERIAL(CBarTSteelBar, CBarSteelBar, 0);
CBarTSteelBar::CBarTSteelBar()
{
}

CBarTSteelBar::CBarTSteelBar(CMainGirder* gdrL, CMainGirder* gdrR,int segL,int segR) :CBarSteelBar(gdrL,gdrR,segL,segR)
{
	m_BarType = CBarSteelBar::T;
}

CBarTSteelBar::~CBarTSteelBar()
{
}



//构造显示用钢束点集(构造沿前进方向最左侧的那组)
void CBarTSteelBar::createPtArray()
{
	double pLenz, pLeny;      //钢束按平弯计需要加密计算的区段长度(左端和右端)
	double sLenz, sLeny;      //钢束按竖弯计需要加密计算的区段长度(左端和右端)
	double rLenz, rLeny;      //钢束需要加密计算的区段长度最终采用值(左端和右端)

	double x0;                //纵向里程
	double hj;                //距箱梁中心线横距
	double curX,Ytmp;         //临时x,y坐标变量
	int IsAnchor;
	CSegment* seg = nullptr;  //存放参与计算的节段对象
	PT3D* pt = nullptr;       //存放临时三维点对象 

	/******************************************************************************************************/
	#define CALPT_ADDTOVECTOR hj = -getXAt_p(curX, IsAnchor);\
								  Ytmp = getTopLev(curX) - getYAtX(curX);\
								  pt = new PT3D(hj, Ytmp, curX);\
								  m_BarPtarrayReal.push_back(pt);\
   /******************************************************************************************************/

	for (vector<PT3D*>::iterator iter = m_BarPtarrayReal.begin(); iter != m_BarPtarrayReal.end(); iter++)
		if (nullptr != *iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	m_BarPtarrayFace.clear();

	pLenz = m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P;
	pLeny = m_X8_P + m_X9_P + m_X10_P + m_X11_P + m_X12_P + m_X13_P;

	sLenz = m_X1 + m_X2 + m_X3;
	sLeny = m_X5 + m_X6 + m_X7;

	rLenz = (pLenz > sLenz) ? pLenz : sLenz;
	rLeny = (pLeny > sLeny) ? pLeny : sLeny;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	x0 = m_Lgirder->m_Center + seg->m_AddLength + m_LtoleDis;


	//计算第一部分点组
	curX = x0;
	while (curX - x0 < rLenz)
	{
		CALPT_ADDTOVECTOR
		curX += m_DensityBend;
	}
	curX = x0 + rLenz;
	CALPT_ADDTOVECTOR
	curX += m_DensitynoBend;

	//计算第二部分点组
	while (curX < x0 + m_PrjLen - rLeny)
	{
		CALPT_ADDTOVECTOR
		curX += m_DensitynoBend;
	}
	curX = x0 + m_PrjLen - rLeny;
	CALPT_ADDTOVECTOR
	curX += m_DensityBend;

	//计算第三部分点组
	while (curX < x0 + m_PrjLen)
	{
		CALPT_ADDTOVECTOR
		curX += m_DensityBend;
	}

	//计算最后一点
	curX = x0 + m_PrjLen;
	CALPT_ADDTOVECTOR
}




void CBarTSteelBar::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

