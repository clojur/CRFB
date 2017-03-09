
#include "stdafx.h"
#include "XBar.h"


IMPLEMENT_SERIAL(CBarXSteelBar, CBarSteelBar, 0);
CBarXSteelBar::CBarXSteelBar()
{
	m_BarType = CBarSteelBar::X;
}


CBarXSteelBar::CBarXSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int segL, int segR) :CBarSteelBar(gdrL, gdrR, segL, segR)
{
	m_BarType = CBarSteelBar::X;
}


CBarXSteelBar::~CBarXSteelBar()
{
}



//得到任意里程 X 值时的钢束 Y 值(Y为距离顶板距离)
double CBarXSteelBar::getYAtX(double x)
{
	if (m_IsXmbar)
	{
		return getYAtX_M(x);
	}
	else
	{
		return CBarSteelBar::getYAtX(x);
	}
}



//得到任意里程 X 值时的钢束 Y 值(Y为距离顶板距离)
double CBarXSteelBar::getYAtX_M(double x)
{
	double sX, eX, elev, elev1, elev2;
	double rB, TB, A, D, ia1, id1, newX, tmp;
	CSegment* seg = nullptr;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	ia1 = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;
	sX = m_Lgirder->m_Center + seg->m_AddLength;

	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	id1 = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;
	eX = m_Lgirder->m_Center + seg->m_AddLength;

	if (sX - x > ACC_NORMAL || x - eX > ACC_NORMAL)
	{
		return ERR_ELEV;
	}
    //重新定义newX值
	newX = x - sX - m_LtoleDis;
	//计算两个钢束底高程
	elev1 = getTopLev(sX) + m_LtoleDis * ia1 - m_H1;
	elev2 = getTopLev(eX) - m_RtoleDis * id1 - m_H2;
	//计算x点的设计高
	elev = getTopLev(x);
	//计算值
	rB = m_Br_S;
	TB = m_Bt_S;
	A = m_Aa_S * PI / 180;
	D = m_Da_S * PI / 180;

	if (newX <= 0 && newX > -m_X1)
	{
		return elev - elev1;
	}
	else if (newX >= 0 && newX < m_X1)
	{
		return elev - elev1 - newX * tan(A);
	}
	else if (newX >= m_X1 && newX < m_X1 + m_X2 + m_X3)
	{
		tmp = newX - m_X1;
		return elev - elev1 - m_Y1 - (sqrt((rB * rB) - pow((tmp - rB * sin(A)), 2)) - rB * cos(A));
	}
	else if (newX >= m_X1 + m_X2 + m_X3 && newX < m_X1 + m_X2 + m_X3 + m_X4)
	{
		tmp = m_X1 + m_X2 + m_X3 + m_X4 - newX;
		return elev - elev2 - tmp * tan(D);
	}
	else
	{
		return elev - elev2;
	}
}



//根据基本参数计算其他细部参数
void CBarXSteelBar::CalDetailProPara()
{
	//判断常规计算模式是否成立，否则调用帽筋模块
	m_Bt_S = m_Br_S * tan((m_Aa_S * PI / 180) / 2);
	m_Ct_S = m_Cr_S * tan((m_Da_S * PI / 180) / 2);
	
	if (m_Bt_S + m_Ct_S + (m_H1 - m_Delta) / tan(m_Aa_S * PI / 180) + (m_H2 - m_Delta) / tan(m_Da_S * PI / 180) + m_LtoleDis + m_RtoleDis > m_DisLtoR)
	{
		m_IsXmbar = true;
		m_Ca_S = m_Da_S;
		CalDetailProPara_Mbar();
		return;
	}
	else
	{
		m_IsXmbar = false;
		CBarSteelBar::CalDetailProPara();
		return;
	}
}



//根据基本参数计算其他细部参数
void CBarXSteelBar::CalDetailProPara_Mbar()
{
	double ia,ic;
	double p0x, p0y, p1x, p1y, pbx, pby;
	double k1, k2;
	double Clen = 0;  
	CSegment* seg = nullptr;  //存放参与计算的节段对象
	
	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	ia = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;
	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	ic = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	p0x = 0;
	p0y = getTopLev(m_Lgirder->m_Center + seg->m_AddLength + m_LtoleDis) - m_H1;

	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	p1x = m_DisLtoR - m_LtoleDis - m_RtoleDis;
	p1y = getTopLev(m_Rgirder->m_Center + seg->m_AddLength - m_RtoleDis) - m_H2;

	k1 = tan(m_Aa_S * PI / 180);
	k2 = -1 * tan(m_Ca_S * PI / 180);

	getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);

	m_Ba_S = m_Aa_S + m_Ca_S;
	m_Bt_S = m_Br_S * tan((m_Ba_S * PI / 180) / 2);
	m_Bl_S = m_Br_S * m_Ba_S * PI / 180;
	m_Be_S = (1 / cos(m_Ba_S * PI / 360) - 1) * m_Br_S;

	m_X2 = m_Bt_S * cos(m_Aa_S * PI / 180);
	m_X1 = pbx - m_X2;

	m_X3 = m_Bt_S * cos(m_Ca_S * PI / 180);
	m_X4 = p1x - pbx - m_X3;

	m_Y2 = m_Bt_S * sin(m_Aa_S * PI / 180);
	m_Y1 = m_X1 * tan(m_Aa_S * PI / 180);

	m_Y3 = m_Bt_S * sin(m_Ca_S * PI / 180);
	m_Y4 = m_X4 * tan(m_Ca_S * PI / 180);
}



//构造显示用钢束点集(构造沿前进方向左腹板那组)
void CBarXSteelBar::createPtArray()
{
	double rLenz, rLeny;      //钢束需要加密计算的区段长度最终采用值(左端和右端)
	double x0;                //纵向里程
	double hj;                //距箱梁中心线横距
	double curX, Ytmp;        //临时x,y坐标变量
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

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	x0 = m_Lgirder->m_Center + seg->m_AddLength + m_LtoleDis;

	if (!m_IsXmbar)
	{
		rLenz = m_X1 + m_X2 + m_X3;
		rLeny = m_X5 + m_X6 + m_X7;

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
	else
	{
		//计算第一条线
		curX = x0;
		CALPT_ADDTOVECTOR

		curX += m_X1;
		CALPT_ADDTOVECTOR
		curX += m_DensityBend;

		//计算第二部分点组
		while (curX < x0 + m_X1 + m_X2 + m_X3)
		{
			CALPT_ADDTOVECTOR
			curX += m_DensityBend;
		}
		curX = x0 + m_X1 + m_X2 + m_X3;
		CALPT_ADDTOVECTOR
		curX += m_DensityBend;
		CALPT_ADDTOVECTOR

		//计算第二条线
		curX = x0 + m_PrjLen;
		CALPT_ADDTOVECTOR
	}
}



void CBarXSteelBar::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

