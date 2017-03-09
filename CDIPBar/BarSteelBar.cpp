
#include "stdafx.h"
#include "BarSteelBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


double CBarSteelBar::m_DensityBend = 0.3;        //钢束弯曲部分的点集密度(米)
double CBarSteelBar::m_DensitynoBend = 5;        //钢束平直部分的点集密度(米)

CRigidFrameBridge* CBarSteelBar::brg = nullptr;
bool CBarSteelBar::b_HasGetinfo = false;
CString CBarSteelBar::m_LocElevation;
CString CBarSteelBar::m_SlpType;
double CBarSteelBar::m_SlpVal;
double CBarSteelBar::m_Guardrail;
double CBarSteelBar::m_TboxWidth;
double CBarSteelBar::m_DboxWidth;
double CBarSteelBar::m_Pavement = 0.0;     

const double CBarSteelBar::ERR_ELEV = -2016;     //计算中错误的高程返回此值 


IMPLEMENT_SERIAL(CBarSteelBar, CBarBase, 0);
CBarSteelBar::CBarSteelBar()
{
}

CBarSteelBar::~CBarSteelBar()
{
}



//获得桥梁基本信息:设计标高位置,横坡类型,横坡值,护栏宽度,箱梁顶宽,箱梁底宽 [(验证日期 2016.10.28)]
void CBarSteelBar::GetBridgeInfo(CRigidFrameBridge* bridge)
{
	brg = bridge;

	m_LocElevation = brg->m_StdSect.m_LocElevation;
	m_SlpType = brg->m_StdSect.m_SlpType;
	m_SlpVal = brg->m_StdSect.m_SlpVal;
	m_Guardrail = brg->m_StdSect.m_Guardrail;
	m_TboxWidth = brg->m_StdSect.m_TboxWidth;
	m_DboxWidth = brg->m_StdSect.m_DboxWidth;
	b_HasGetinfo = true;
}



//基本构造函数(传入左右锚固节段所在的主梁对象,以及左右锚固节段编号) [(验证日期 2016.10.28)]
CBarSteelBar::CBarSteelBar(CMainGirder* gdrL, CMainGirder* gdrR, int lAnchor, int rAnchor)
{
	m_SteelClass = CBarBase::STEEL_BAR;
	m_MultiBend = false;
	m_IsReserveBar = false;

	m_Lgirder = gdrL;
	m_Rgirder = gdrR;

	m_Lseg = lAnchor;
	m_Rseg = rAnchor;

	//顺便把节段间距离(接缝距离)计算出来备用
	CSegment* seg = nullptr;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	m_DisL = seg->m_LeftStake;

	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	m_DisR = seg->m_RightStake;

	m_DisLtoR = m_DisR - m_DisL;
}





//根据钢束内含钢绞线根数计算【保护层厚度】,【钢束最小间距】,【波纹管直径】
double CBarSteelBar::GetParaByBeamct(short count, short calType)
{
	double retMaskant = 0;    //最小保护层厚度
	double retBarSpace = 0;   //最小钢束间距
	double retDiaBellows = 0; //最小波纹管直径

	if (count <= 5)
	{
		retMaskant = 1.1 * 63;
		retBarSpace = 1.6 * 63;
		retDiaBellows = 63;
	}
	else if (count <= 7)
	{
		retMaskant = 1.1 * 73;
		retBarSpace = 1.6 * 73;
		retDiaBellows = 73;
	}
	else if (count <= 12)
	{
		retMaskant = 1.1 * 88;
		retBarSpace = 1.6 * 88;
		retDiaBellows = 88;
	}
	else if (count <= 17)
	{
		retMaskant = 1.1 * 106;
		retBarSpace = 1.6 * 106;
		retDiaBellows = 106;
	}
	else if (count <= 22)
	{
		retMaskant = 1.1 * 116;
		retBarSpace = 1.6 * 116;
		retDiaBellows = 116;
	}
	else if (count <= 27)
	{
		retMaskant = 1.1 * 131;
		retBarSpace = 1.6 * 131;
		retDiaBellows = 131;
	}
	else
	{
		retMaskant = 1.1 * 146;
		retBarSpace = 1.6 * 146;
		retDiaBellows = 146;
	}

	retMaskant = (retMaskant <= 140 ? 0.14 : (int(retMaskant / 10) + 1) / 100.0);
	retBarSpace = (int(retBarSpace / 10) + 1) / 100.0;
	retDiaBellows = (int(retDiaBellows / 10) + 1) / 100.0;

	if (calType == 1)
		return retMaskant;
	else if (calType == 2)
		return retBarSpace;
	else
		return retDiaBellows;
}



//设置本钢束的基本平弯参数 [(验证日期 2016.10.28)]
void CBarSteelBar::SetBasePLanPara(double z0, double z1, double z8, 
	                               double xz1, double xr2, 
								   double Aa, double Ar, double Da, double Dr)
{
	m_Z0 = z0;
	m_Z1 = z1;
	m_Z8 = z8;
	m_XZ1 = xz1;
	m_XR2 = xr2;
	m_Aa_P = Aa ;
	m_Ar_P = Ar;
	m_Da_P = Da ;
	m_Dr_P = Dr;
}



//设置本钢束的基本竖弯参数(适用于T,X,H,D,Z。如果是Z钢束,h1,h2代表左右齿板高) [(验证日期 2016.10.28)]
void CBarSteelBar::SetBaseProPara(CString strName, double h1, double h2, double delta,
	                              double Aa, double Da, double Br, double Cr,
	                              double bj1, double bj2)
{
	m_Name = strName;
	if (m_BarType == CBarSteelBar::Z)
	{
		m_LtplateH = h1;
		m_RtplateH = h2;
	}
	else
	{
		m_H1 = h1;
		m_H2 = h2;
	}

	m_Delta = delta;
	m_Aa_S = Aa;
	m_Da_S = Da;
	m_Br_S = Br;
	m_Cr_S = Cr;
	m_LtoleDis = bj1;
	m_RtoleDis = bj2;
}



//设置本钢束的基本竖弯参数(适用于B) +2重载 [(验证日期 2016.10.28)]
void CBarSteelBar::SetBaseProPara(CString strName, CString side, double h, double d, double delta,
                         	      double Aa, double Da, double Br, double Cr,
	                              double bj1, double bj2)
{
	m_Name = strName;
	m_side = side;
	if (side == _T("左"))
	{
		m_LtplateH = 0;
		m_RtplateH = d;
	}
	else
	{
		m_LtplateH = d;
		m_RtplateH = 0;
	}

	m_H1 = h;
	m_H2 = h;
	m_Delta = delta;
	m_Aa_S = Aa;
	m_Da_S = Da;
	m_Br_S = Br;
	m_Cr_S = Cr;
	m_LtoleDis = bj1;
	m_RtoleDis = bj2;
}



//更新计算平弯具体参数(常规平弯) [(验证日期 2016.10.28)]
void CBarSteelBar::CalDetailPlanPara()
{
	double Clen;//左右锚固节段间的长度(锚固点间距离)

	if (m_MultiBend)
	{
		CalDetailPlanPara_Multi();
		return;
	}


	if (m_BarType == CBarSteelBar::B || m_BarType == CBarSteelBar::Z)
	{
		Clen = m_DisLtoR - m_LtoleDis - m_RtoleDis - m_LtplateH * sin(m_Aa_S * PI / 180) - -m_RtplateH * sin(m_Da_S * PI / 180);
	}
	else
	{
		Clen = m_DisLtoR - m_LtoleDis - m_RtoleDis;
	}
	
	m_At_P = m_Ar_P * tan(fabs(m_Aa_P* PI / 180) / 2);
	m_Dt_P = m_Dr_P * tan(fabs(m_Da_P* PI / 180) / 2);
	m_Ae_P = (1 / cos(m_Aa_P* PI / 180 / 2) - 1) * m_Ar_P;
	m_De_P = (1 / cos(m_Da_P* PI / 180 / 2) - 1) * m_Dr_P;
	m_Al_P = m_Ar_P * fabs(m_Aa_P* PI / 180);
	m_Dl_P = m_Dr_P * fabs(m_Da_P* PI / 180);

	m_Ba_P = m_Aa_P;
	m_Br_P = m_Ar_P;
	m_Bt_P = m_At_P;
	m_Be_P = m_Ae_P;
	m_Bl_P = m_Al_P;

	m_Ca_P = m_Da_P;
	m_Cr_P = m_Dr_P;
	m_Ct_P = m_Dt_P;
	m_Ce_P = m_De_P;
	m_Cl_P = m_Dl_P;

	m_X1_P = m_XZ1 - m_At_P;
	m_X2_P = m_At_P;

	if (m_Aa_P == 0)
	{
		m_X3_P = m_X4_P = m_X5_P = m_X6_P = 0;
	}
	else
	{
		m_X3_P = m_X5_P = m_At_P * cos(fabs(m_Aa_P* PI / 180));
		m_X4_P = fabs(m_Z0 - m_Z1) / tan(fabs(m_Aa_P* PI / 180)) - 2 * m_At_P * cos(fabs(m_Aa_P* PI / 180));
		m_X6_P = m_Bt_P;
	}

	m_X13_P = m_XR2 - m_Dt_P;
	m_X12_P = m_Dt_P;

	if (m_Da_P == 0)
	{
		m_X8_P = m_X9_P = m_X10_P = m_X11_P = 0;
	}
	else
	{
		m_X10_P = fabs(m_Z0 - m_Z8) / tan(fabs(m_Da_P* PI / 180)) - 2 * m_Dt_P * cos(fabs(m_Da_P* PI / 180));
		m_X9_P = m_X11_P = m_Dt_P * cos(fabs(m_Da_P* PI / 180));
		m_X8_P = m_Dt_P;
	}

	m_X7_P = Clen - m_XZ1 - m_XR2 - m_X3_P - m_X4_P - m_X5_P - m_X6_P - m_X8_P - m_X9_P - m_X10_P - m_X11_P;
	m_PrjLen = Clen; //在平弯函数中计算钢束投影长是合适的,没有耦合

	m_Z2 = m_Z4 = m_At_P * sin(fabs(m_Aa_P* PI / 180));
	m_Z3 = m_X4_P * tan(fabs(m_Aa_P* PI / 180));
	m_Z5 = m_Bt_P * sin(fabs(m_Ba_P* PI / 180));
	m_Z7 = m_Dt_P * sin(fabs(m_Da_P* PI / 180));
	m_Z6 = m_X10_P * tan(fabs(m_Da_P* PI / 180));
}




//更新计算平弯具体参数(多次平弯)
void CBarSteelBar::CalDetailPlanPara_Multi()
{
	double Clen = 0; //左右锚固节段间的长度(锚固点间距离)
	m_PrjLen = Clen;
	//.....
}



//更新计算竖弯具体参数 [(验证日期 2016.10.28)]
void CBarSteelBar::CalDetailProPara()
{
	double Clen = 0;            //左右锚固节段间的长度(锚固点间距离)
	double TBB, TCC;            //B,C两特征点T长的水平投影
	double ii, k1, k2;          //节段顶板的纵坡,斜率变量
	double p0x, p0y, p1x, p1y;  //计算两直线交点时的两个直线起点
	double pbx, pby, pcx, pcy;  //存放直线相交结果
	CSegment* seg = nullptr;    //存放参与计算的节段对象
	int curSeg;                 //当前参与计算的节段编号(视计算需要随时改变)


	//如果是底板竖,自动调用计算底板竖弯具体参数模块
	if (m_BarType == CBarSteelBar::Z)
	{
		CalDetailProPara_Z();
		return;
	}
	else if (m_BarType == CBarSteelBar::B)
	{
		if (m_side == _T("左"))
			CalDetailProPara_BL();
		else
			CalDetailProPara_BR();
		return;
	}


	if (m_Aa_S == 0)            //没有竖弯的情况(一般仅出现在 T束)
	{
		m_Ba_S = 0; m_Bt_S = 0; TBB = 0; m_Bl_S = 0; m_Ca_S = 0; m_Ct_S = 0; TCC = 0; m_Cl_S = 0;
	}
	else                        //有竖弯的情况
	{
		//算第一部分(左锚固端部分)
		curSeg = m_Lseg;
		seg = m_Lgirder->GetSegmentByID(m_Lseg);

		ii = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;
		p0x = 0;
		p0y = getTopLev(m_DisL + m_LtoleDis) - m_H1;
		p1x = seg->m_Length - m_LtoleDis;
		p1y = getTopLev(seg->m_RightStake) - m_Delta;
		k1 = tan(m_Aa_S * PI / 180);
		k2 = ii;

		getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);

	ReCal1:
		m_Ba_S = m_Aa_S - (atan(ii) * 180 / PI);
		m_Bt_S = m_Br_S * tan((m_Ba_S * PI / 180) / 2);
		m_Bl_S = m_Br_S * m_Ba_S * PI / 180;
		m_Be_S = (1 / cos(m_Ba_S * PI / 360) - 1) * m_Br_S;
		TBB = m_Bt_S * cos(atan(ii));

		Clen += seg->m_Length;

		if (pbx + TBB + m_LtoleDis > Clen)
		{
			curSeg++;
			seg = m_Lgirder->GetSegmentByID(curSeg);
			ii = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;
			p1x = Clen + seg->m_Length - m_LtoleDis;
			p1y = getTopLev(seg->m_RightStake) - m_Delta;
			k2 = ii;
			getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);
			goto ReCal1;
		}

		//算第二部分(右锚固端部分)
		Clen = 0;
		curSeg = m_Rseg;
		seg = m_Rgirder->GetSegmentByID(m_Rseg);
		ii = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;
		p0x = 0;
		p0y = getTopLev(seg->m_RightStake - m_RtoleDis) - m_H2;

		p1x = m_RtoleDis - seg->m_Length;
		p1y = getTopLev(seg->m_LeftStake) - m_Delta;
		k1 = -tan(m_Da_S * PI / 180);
		k2 = ii;

		getInters(p0x, p0y, p1x, p1y, k1, k2, pcx, pcy);

	ReCal2:
		m_Ca_S = m_Da_S + (atan(ii) * 180 / PI);
		m_Ct_S = m_Cr_S * tan((m_Ca_S * PI / 180) / 2);
		m_Cl_S = m_Cr_S * m_Ca_S * PI / 180;
		m_Ce_S = (1 / cos(m_Ca_S * PI / 360) - 1) * m_Cr_S;
		TCC = m_Ct_S * cos(atan(ii));

		Clen += seg->m_Length;

		if (fabs(pcx) + TCC + m_RtoleDis > Clen)
		{
			curSeg--;
			seg = m_Rgirder->GetSegmentByID(curSeg);
			ii = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;
			p1x = m_RtoleDis - seg->m_Length - Clen;
			p1y = getTopLev(seg->m_LeftStake) - m_Delta;
			k2 = ii;
			getInters(p0x, p0y, p1x, p1y, k1, k2, pcx, pcy);
			goto ReCal2;
		}
	}

	m_X2 = m_Bt_S * cos(m_Aa_S * PI / 180);
	m_X1 = pbx - m_X2;
	m_X3 = TBB;
	m_X5 = TCC;
	m_X6 = m_Ct_S * cos(m_Da_S * PI / 180);
	m_X7 = fabs(pcx) - m_X6;
	m_X4 = m_DisLtoR - m_LtoleDis - m_RtoleDis - m_X1 - m_X2 - m_X3 - m_X5 - m_X6 - m_X7;

	m_Y1 = m_X1 * tan(m_Aa_S * PI / 180);
	m_Y2 = m_Bt_S * sin(m_Aa_S * PI / 180);
	m_Y3 = m_X7 * tan(m_Da_S * PI / 180);
	m_Y4 = m_Ct_S * sin(m_Da_S * PI / 180);
}



//更新计算竖弯具体参数(适用于Z钢束) [(验证日期 2016.10.28)]
void CBarSteelBar::CalDetailProPara_Z()
{
	int curSeg;                //当前参与计算的节段编号
	double iah, idh;           //节段底板纵坡
	double ii, k1, k2;
	double p0x, p0y, p1x, p1y; //计算两直线交点时的两个直线起点
	double pbx, pby, pcx, pcy; //存放直线相交结果
	double TBB, TCC;
	CSegment* seg = nullptr;   //存放参与计算的节段对象


	curSeg = m_Lseg;
	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	iah = (seg->m_RbottomElev1 - seg->m_LbottomElev1) / seg->m_Length;
	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	idh = (seg->m_RbottomElev1 - seg->m_LbottomElev1) / seg->m_Length;

	//算第一部分
	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;

	p0x = m_LtoleDis + m_LtplateH * sin(m_Aa_S * PI / 180);
	p0y = seg->m_LbottomElev2 + seg->m_LplateThick + m_LtoleDis * iah + m_LtplateH * cos(m_Aa_S * PI / 180);
	p1x = seg->m_Length;
	p1y = seg->m_RbottomElev2 + m_Delta;
	m_H1 = p0y - seg->m_LbottomElev2;

	k1 = -tan(m_Aa_S * PI / 180);
	k2 = ii;
	getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);

ReCal1:
	m_Ba_S = m_Aa_S + (atan(ii) * 180 / PI);
	m_Bt_S = m_Br_S * tan((m_Ba_S * PI / 180) / 2);
	TBB = m_Bt_S * cos(atan(ii));
	m_Bl_S = m_Br_S * m_Ba_S * PI / 180;
	m_Be_S = (1 / cos(m_Ba_S * PI / 360) - 1) * m_Br_S;

	m_Y2 = m_Bt_S * sin(m_Aa_S * PI / 180);

	if (pbx + TBB > p1x)
	{
		curSeg++;
		seg = m_Lgirder->GetSegmentByID(curSeg);
		ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;
		p1x = p1x + seg->m_Length;
		p1y = seg->m_RbottomElev2 + m_Delta;
		k2 = ii;
		getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);
		goto ReCal1;
	}

	//算第二部分
	curSeg = m_Rseg;
	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;

	p1x = -seg->m_Length;
	p1y = seg->m_LbottomElev2 + m_Delta;
	p0x = -m_RtoleDis - m_RtplateH * sin(m_Da_S * PI / 180);
	p0y = seg->m_RbottomElev2 + seg->m_RplateThick + m_RtoleDis * fabs(idh) + m_RtplateH * cos(m_Da_S * PI / 180);
	m_H2 = p0y - seg->m_RbottomElev2;

	k1 = tan(m_Da_S * PI / 180);
	k2 = ii;
	getInters(p0x, p0y, p1x, p1y, k1, k2, pcx, pcy);

ReCal2 :
	m_Ca_S = m_Da_S + (fabs(atan(ii)) * 180 / PI);
	m_Ct_S = m_Br_S * tan((m_Ca_S * PI / 180) / 2);
	TCC = m_Ct_S * cos(fabs(atan(ii)));
	m_Cl_S = m_Br_S * m_Ca_S * PI / 180;
	m_Ce_S = (1 / cos(m_Ca_S * PI / 360) - 1) * m_Br_S;

	m_Y5 = m_Bt_S * sin(m_Da_S * PI / 180);

	if (fabs(pcx)+ TCC > fabs(p1x))
	{
		curSeg--;
		seg = m_Rgirder->GetSegmentByID(curSeg);
		ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;
		p1x = p1x - seg->m_Length;
		p1y = seg->m_LbottomElev2 + m_Delta;
		k2 = ii;
		getInters(p0x, p0y, p1x, p1y, k1, k2, pcx, pcy);
		goto ReCal2;
	}

	m_X1 = m_LtoleDis + m_LtplateH * sin(m_Aa_S * PI / 180);
	m_X3 = m_Bt_S * cos(m_Aa_S * PI / 180);
	m_X2 = pbx - m_X1 - m_X3;

	m_X4 = TBB;
	m_X9 = m_RtoleDis + m_RtplateH * sin(m_Da_S * PI / 180);
	m_X7 = m_Ct_S * cos(m_Da_S * PI / 180);
	m_X8 = fabs(pcx) - m_X9 - m_X7;

	m_X6 = TCC;
	m_X5 = m_DisLtoR - m_X1 - m_X2 - m_X3 - m_X4 - m_X6 - m_X7 - m_X8 - m_X9;

	m_Y3 = m_X2 * tan(m_Aa_S * PI / 180);
	m_Y1 = m_H1 - (m_Y2 + m_Y3);
	m_Y6 = m_X8 * tan(m_Da_S * PI / 180);
	m_Y4 = m_H2 - (m_Y5 + m_Y6);
}



//根据基本参数计算其他细部参数 [(验证日期 2016.10.28)]
void CBarSteelBar::CalDetailProPara_BL()
{
	double p0x, p0y, p1x, p1y;
	double pbx, pby, pcx, pcy;
	double k1, k2;
	double TBB, TCC;

	int curSeg;       //当前参与计算的节段编号
	double ii;
	double idh;     //有齿板侧边距导致的纵坡
	CSegment* seg = nullptr;

	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	idh = (seg->m_RbottomElev1 - seg->m_LbottomElev1) / seg->m_Length;

	//算第一部分
	curSeg = m_Lseg;
	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;

	p1x = seg->m_Length;
	p1y = seg->m_RbottomElev2 + m_Delta;

	p0x = m_LtoleDis;
	p0y = seg->m_LbottomElev2 + m_H1;

	k1 = -tan(m_Aa_S * PI / 180);
	k2 = ii;

	getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);

ReCal1:
	m_Ba_S = m_Aa_S + (atan(ii) * 180 / PI);
	m_Bt_S = m_Br_S * tan((m_Ba_S * PI / 180) / 2);
	TBB = m_Bt_S * cos(atan(ii));
	m_Bl_S = m_Br_S * m_Ba_S * PI / 180;

	m_Y2 = m_Bt_S * sin(m_Aa_S * PI / 180);
	m_Be_S = (1 / cos(m_Ba_S * PI / 360) - 1) * m_Br_S;

	if (pbx + TBB > p1x)
	{
		curSeg++;
		seg = m_Lgirder->GetSegmentByID(curSeg);
		ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;
		p1x = p1x + seg->m_Length;
		p1y = seg->m_RbottomElev2 + m_Delta;
		k2 = ii;
		getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);
		goto ReCal1;
	}

	//算第二部分
	curSeg = m_Rseg;
	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;

	p1x = -seg->m_Length;
	p1y = seg->m_LbottomElev2 + m_Delta;
	p0x = -m_RtoleDis - m_RtplateH * sin(m_Da_S * PI / 180);
	p0y = seg->m_RbottomElev2 + seg->m_RplateThick + m_RtoleDis * fabs(idh) + m_RtplateH * cos(m_Da_S * PI / 180);
	m_H2 = p0y - seg->m_RbottomElev2;

	k1 = tan(m_Da_S * PI / 180);
	k2 = ii;

	getInters(p0x, p0y, p1x, p1y, k1, k2, pcx, pcy);

ReCal2:
	m_Ca_S = m_Da_S + (fabs(atan(ii)) * 180 / PI);
	m_Ct_S = m_Cr_S * tan((m_Ca_S * PI / 180) / 2);
	TCC = m_Ct_S * cos(fabs(atan(ii)));
	m_Cl_S = m_Cr_S * m_Ca_S * PI / 180;

	m_Y5 = m_Ct_S * sin(m_Da_S * PI / 180);
	m_Ce_S = (1 / cos(m_Ca_S * PI / 360) - 1) * m_Cr_S;

	if (fabs(pcx) + TCC > fabs(p1x))
	{
		curSeg--;
		seg = m_Rgirder->GetSegmentByID(curSeg);
		ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;

		p1x = p1x - seg->m_Length;
		p1y = seg->m_LbottomElev2 + m_Delta;
		k2 = ii;
		getInters(p0x, p0y, p1x, p1y, k1, k2, pcx, pcy);
		goto ReCal2;
	}

	m_X1 = m_LtoleDis;
	m_X3 = m_Bt_S * cos(m_Aa_S * PI / 180);
	m_X2 = pbx - m_X1 - m_X3;

	m_X4 = TBB;

	m_X9 = m_RtoleDis + m_RtplateH * sin(m_Da_S * PI / 180);
	m_X7 = m_Ct_S * cos(m_Da_S * PI / 180);
	m_X8 = fabs(pcx) - m_X9 - m_X7;

	m_X6 = TCC;
	m_X5 = m_DisLtoR - m_X1 - m_X2 - m_X3 - m_X4 - m_X6 - m_X7 - m_X8 - m_X9;

	m_Y3 = m_X2 * tan(m_Aa_S * PI / 180);
	m_Y1 = m_H1 - (m_Y2 + m_Y3);
	m_Y6 = m_X8 * tan(m_Da_S * PI / 180);
	m_Y4 = m_H2 - (m_Y5 + m_Y6);
}



//根据基本参数计算其他细部参数 [(验证日期 2016.10.28)]
void CBarSteelBar::CalDetailProPara_BR()
{
	double p0x, p0y, p1x, p1y;
	double pbx, pby, pcx, pcy;
	double k1, k2;
	double TBB, TCC;

	int curSeg;       //当前参与计算的节段编号
	double ii;
	double iah;      //有齿板侧边距导致的纵坡
	CSegment* seg = nullptr;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	iah = (seg->m_RbottomElev1 - seg->m_LbottomElev1) / seg->m_Length;

	//算第一部分
	curSeg = m_Lseg;
	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;

	p1x = seg->m_Length;
	p1y = seg->m_RbottomElev2 + m_Delta;
	p0x = m_LtoleDis + m_LtplateH * sin(m_Aa_S * PI / 180);
	p0y = seg->m_LbottomElev2 + seg->m_LplateThick + m_LtoleDis * iah + m_LtplateH * cos(m_Aa_S * PI / 180);

	k1 = -tan(m_Aa_S * PI / 180);
	k2 = ii;

	getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);

ReCal1:
	m_Ba_S = m_Aa_S + (atan(ii) * 180 / PI);
	m_Bt_S = m_Br_S * tan((m_Ba_S * PI / 180) / 2);
	TBB = m_Bt_S * cos(atan(ii));
	m_Bl_S = m_Br_S * m_Ba_S * PI / 180;

	m_Y2 = m_Bt_S * sin(m_Aa_S * PI / 180);
	m_Be_S = (1 / cos(m_Ba_S * PI / 360) - 1) * m_Br_S;

	if (pbx + TBB > p1x)
	{
		curSeg++;
		seg = m_Lgirder->GetSegmentByID(curSeg);
		ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;
		p1x += seg->m_Length;
		p1y = seg->m_RbottomElev2 + m_Delta;
		k2 = ii;
		getInters(p0x, p0y, p1x, p1y, k1, k2, pbx, pby);
		goto ReCal1;
	}

	//算第二部分
	curSeg = m_Rseg;
	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;

	p1x = -seg->m_Length;
	p1y = seg->m_LbottomElev2 + m_Delta;
	p0x = -m_RtoleDis;
	p0y = m_H2 + seg->m_RbottomElev2;

	k1 = tan(m_Da_S * PI / 180);
	k2 = ii;

	getInters(p0x, p0y, p1x, p1y, k1, k2, pcx, pcy);

ReCal2:
	m_Ca_S = m_Da_S + (fabs(atan(ii)) * 180 / PI);//atan是否加绝对值请检查!!
	m_Ct_S = m_Cr_S * tan((m_Ca_S * PI / 180) / 2);
	TCC = m_Ct_S * cos(fabs(atan(ii)));
	m_Cl_S = m_Cr_S * m_Ca_S * PI / 180;

	m_Y5 = m_Ct_S * sin(m_Da_S * PI / 180);
	m_Ce_S = (1 / cos(m_Ca_S * PI / 360) - 1) * m_Cr_S;

	if (fabs(pcx) + TCC > fabs(p1x))
	{
		curSeg--;
		seg = m_Rgirder->GetSegmentByID(curSeg);
		ii = (seg->m_RbottomElev2 - seg->m_LbottomElev2) / seg->m_Length;

		p1x -= seg->m_Length;
		p1y = seg->m_LbottomElev2 + m_Delta;
		k2 = ii;
		getInters(p0x, p0y, p1x, p1y, k1, k2, pcx, pcy);
		goto ReCal2;
	}

	m_X1 = m_LtoleDis + m_LtplateH * sin(m_Aa_S * PI / 180);
	m_X3 = m_Bt_S * cos(m_Aa_S * PI / 180);
	m_X2 = pbx - m_X1 - m_X3;

	m_X4 = TBB;

	m_X9 = m_RtoleDis;
	m_X7 = m_Ct_S * cos(m_Da_S * PI / 180);
	m_X8 = fabs(pcx) - m_X9 - m_X7;

	m_X6 = TCC;
	m_X5 = m_DisLtoR - m_X1 - m_X2 - m_X3 - m_X4 - m_X6 - m_X7 - m_X8 - m_X9;

	m_Y3 = m_X2 * tan(m_Aa_S * PI / 180);
	m_Y1 = m_H1 - (m_Y2 + m_Y3);
	m_Y6 = m_X8 * tan(m_Da_S * PI / 180);
	m_Y4 = m_H2 - (m_Y5 + m_Y6);
}



//得到任意里程x值时的钢束平弯距离(距箱梁中心线,始终为正值) [(验证日期 2016.10.28)]
double CBarSteelBar::getXAt_p(double x, int& IsAnchor)
{
	double newX;
	double tmp;

	//(1) 如果不在刚束起止点范围内则退出
	if (m_DisL - x > ACC_NORMAL || x - m_DisR > ACC_NORMAL)
	{
		return -1.0;
	}

	//(2) 计算该钢束的锚固段起止边距
	double bj1, bj2;
	if (m_BarType == CBarSteelBar::B || m_BarType == CBarSteelBar::Z)
	{
		bj1 = m_X1;
		bj2 = m_X9;
	}
	else
	{
		bj1 = m_LtoleDis;
		bj2 = m_RtoleDis;
	}

	//(3) 重新定义newX值,前锚固端至所求点的距离
	newX = x - m_DisL;

	if (newX >= 0 && newX <= bj1 + m_X1_P)
	{
		return  m_Z1;
	}
	else if (newX > bj1 + m_X1_P && newX <= bj1 + m_X1_P + m_X2_P + m_X3_P)
	{
		tmp = newX - m_X1_P - bj1;
		if (m_Z0 > m_Z1)
			return m_Z1 + (m_Ar_P - sqrt(m_Ar_P * m_Ar_P - tmp * tmp));
		else
			return m_Z1 - (m_Ar_P - sqrt(m_Ar_P * m_Ar_P - tmp * tmp));
	}
	else if (newX > bj1 + m_X1_P + m_X2_P + m_X3_P && newX <= bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P)
	{
		tmp = newX - (m_X1_P + m_X2_P + bj1);
		if (m_Z0 > m_Z1)
			return m_Z1 + tmp * tan(m_Aa_P * PI/180);
		else
			return m_Z1 - tmp * tan(m_Aa_P * PI/180);
	}
	else if (newX > bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P && newX <= bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P)
	{
		tmp = m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + bj1 - newX;
		if (m_Z0 > m_Z1)
			return m_Z0 - (m_Ar_P - sqrt(m_Ar_P * m_Ar_P - tmp * tmp));
		else
			return m_Z0 + (m_Ar_P - sqrt(m_Ar_P * m_Ar_P - tmp * tmp));
	}
	else if (newX > bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P && newX <= bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P)
	{
		return m_Z0;
	}
	else if (newX > bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P && newX <= bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P + m_X8_P + m_X9_P)
	{
		tmp = newX - (m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P + bj1);
		if (m_Z0 > m_Z8)
			return m_Z0 - (m_Dr_P - sqrt(m_Dr_P * m_Dr_P - tmp * tmp));
		else
			return m_Z0 + (m_Dr_P - sqrt(m_Dr_P * m_Dr_P - tmp * tmp));
	}
	else if (newX > bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P + m_X8_P + m_X9_P && newX <= bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P + m_X8_P + m_X9_P + m_X10_P)
	{
		tmp = newX - (m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P + m_X8_P + bj1);
		if (m_Z0 > m_Z8)
			return m_Z0 - tmp * tan(m_Da_P * PI/180);
		else
			return m_Z0 + tmp * tan(m_Da_P * PI/180);
	}
	else if (newX > bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P + m_X8_P + m_X9_P + m_X10_P && newX <= bj1 + m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P + m_X8_P + m_X9_P + m_X10_P + m_X11_P + m_X12_P)
	{
		tmp = m_X1_P + m_X2_P + m_X3_P + m_X4_P + m_X5_P + m_X6_P + m_X7_P + m_X8_P + m_X9_P + m_X10_P + m_X11_P + m_X12_P + bj1 - newX;
		if (m_Z0 > m_Z8)
			return m_Z8 + (m_Dr_P - sqrt(m_Dr_P * m_Dr_P - tmp * tmp));
		else
			return m_Z8 - (m_Dr_P - sqrt(m_Dr_P * m_Dr_P - tmp * tmp));
	}
	else
	{
		return m_Z8;
	}

	//处理锚固束判断
	if (fabs(newX) < ACC_NORMAL || fabs(newX - m_DisLtoR) < ACC_NORMAL) //在梁端
		IsAnchor = 1;
	else if (bj1 != 0 && newX >= 0 && newX <= bj1)
		IsAnchor = 1;
	else if (bj2 != 0 && newX >= m_DisLtoR - bj2 && newX <= m_DisLtoR)
		IsAnchor = 1;
	else
		IsAnchor = 0;
}



//得到任意X值时钢束对应顶板的高程 [(验证日期 2016.10.28)]
double CBarSteelBar::getTopLev(double x)
{
	int IsAnchor;
	double DesignLev;
	double x0;

	//计算该X值对应的平弯水平距离
	x0 = getXAt_p(x, IsAnchor);

	brg->m_RoadData.GetDesignElevation(x, DesignLev);
	
	if (m_SlpType == _T("双向坡"))
	{
		if (m_LocElevation == _T("设计中心线"))
			return  DesignLev - x0 * fabs(m_SlpVal) - m_Pavement;
		else if (m_LocElevation == _T("加宽前路基边缘")) 
			return DesignLev + (m_TboxWidth / 2 - x0) * fabs(m_SlpVal) - m_Pavement;
		else
			return ERR_ELEV;
	}
	else
	{
		if (m_SlpVal>0)
			return DesignLev - (m_TboxWidth / 2 - m_Guardrail + x0) * m_SlpVal - m_Pavement;
		else
			return DesignLev + (m_TboxWidth / 2 - m_Guardrail - x0) * m_SlpVal - m_Pavement;
	}
}



//得到任意里程x值时钢束对应底板的高程 [(验证日期 2016.10.28)]
double CBarSteelBar::getBottomLev(double x)
{
	double DesignLev;

	//计算该X值对应设计高程
	brg->m_RoadData.GetDesignElevation(x, DesignLev);
	//根据x里程获得主梁对象
	CMainGirder* gdr = nullptr;
	for (vector<CMainGirder*>::iterator iter = brg->m_MainGirders.begin(); iter != brg->m_MainGirders.end(); iter++)
	{
		if (x >= (*iter)->m_Center - (*iter)->m_AllLen / 2 && x <= (*iter)->m_Center + (*iter)->m_AllLen / 2)
		{
			gdr = *iter;
			break;
		}
	}

	if (gdr == nullptr) return ERR_ELEV;

	if (m_LocElevation == _T("设计中心线"))
		return DesignLev - m_DboxWidth / 2 * fabs(m_SlpVal) - gdr->getH_AtX(x - gdr->m_Center) - m_Pavement;
	else if (m_LocElevation == _T("中央分隔带边缘"))
		return DesignLev - (m_TboxWidth / 2 - m_Guardrail + m_DboxWidth / 2) * fabs(m_SlpVal) - gdr->getH_AtX(x-gdr->m_Center) - m_Pavement;
	else
		return DesignLev + (m_TboxWidth / 2 - m_DboxWidth / 2) * fabs(m_SlpVal) - gdr->getH_AtX(x-gdr->m_Center) - m_Pavement;
}



//得到任意里程 X 值时的钢束距离顶板或者底板的距离(本函数为虚函数,T、X、H、D 钢束用基类成员,B、Z钢束会重写成员) [(验证日期 2016.10.28)]
double CBarSteelBar::getYAtX(double x)
{
	double elev, elev1, elev2;
	double rB, rC, A, D, ia1, id1, newX, tmp;
	CSegment* seg = nullptr;

	seg = m_Lgirder->GetSegmentByID(m_Lseg);
	ia1 = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;

	seg = m_Rgirder->GetSegmentByID(m_Rseg);
	id1 = (seg->m_RdesignElev - seg->m_LdesignElev) / seg->m_Length;

	if (m_DisL - x > ACC_NORMAL || x - m_DisR > ACC_NORMAL)
	{
		return ERR_ELEV;
	}

	//重新定义newX值
	newX = x - m_DisL - m_LtoleDis;
	//计算两个钢束底高程
	elev1 = getTopLev(m_DisL) + m_LtoleDis * ia1 - m_H1;
	elev2 = getTopLev(m_DisR) - m_RtoleDis * id1 - m_H2;


	//计算x点对应的顶板高程
	elev = getTopLev(x);

	rB = m_Br_S;
	rC = m_Cr_S;
	A = m_Aa_S * PI / 180;
	D = m_Da_S * PI / 180;

	if (newX <= 0)
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
		return elev - elev1 - m_Y1 - (sqrt(pow(rB, 2) - pow(tmp - rB * sin(A), 2)) - rB * cos(A));
	}
	else if (newX >= m_X1 + m_X2 + m_X3 && newX < m_X1 + m_X2 + m_X3 + m_X4)
	{
		return m_Delta;
	}
	else if (newX >= m_X1 + m_X2 + m_X3 + m_X4 && newX < m_X1 + m_X2 + m_X3 + m_X4 + m_X5 + m_X6)
	{
		tmp = m_X1 + m_X2 + m_X3 + m_X4 + m_X5 + m_X6 - newX;
		return elev - elev2 - m_Y3 - (sqrt((rC * rC) - pow(rC * sin(D) - tmp, 2)) - rC * cos(D));
	}
	else if (newX >= m_X1 + m_X2 + m_X3 + m_X4 + m_X5 + m_X6 && newX < m_X1 + m_X2 + m_X3 + m_X4 + m_X5 + m_X6 + m_X7)
	{
		tmp = m_X1 + m_X2 + m_X3 + m_X4 + m_X5 + m_X6 + m_X7 - newX;
		return elev - elev2 - tmp * tan(D);
	}
	else
	{
		return elev - elev2;
	}
}



void CBarSteelBar::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}


