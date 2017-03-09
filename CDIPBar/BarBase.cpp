
#include "stdafx.h"
#include "BarBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CBarBase, CObject, 0);
CBarBase::CBarBase()
{
	for (vector<PT3D*>::iterator iter = m_BarPtarrayReal.begin(); iter != m_BarPtarrayReal.end(); iter++)
	{
		*iter = nullptr;
	}

	for (vector<PT3D*>::iterator iter = m_BarPtarrayFace.begin(); iter != m_BarPtarrayFace.end(); iter++)
	{
		*iter = nullptr;
	}
}


CBarBase::~CBarBase()
{
	//删除钢筋点集指针vector防止内存泄露
	for (vector<PT3D*>::iterator iter = m_BarPtarrayReal.begin(); iter != m_BarPtarrayReal.end(); iter++)
	if (nullptr != *iter)
	{
		delete *iter;
		*iter = nullptr;
	}
	m_BarPtarrayFace.clear();

	for (vector<PT3D*>::iterator iter = m_BarPtarrayFace.begin(); iter != m_BarPtarrayFace.end(); iter++)
		if (nullptr != *iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	m_BarPtarrayFace.clear();

}



//获得钢筋总长度(m)
double CBarBase::getAlength()
{
	m_Alength = 0;
	if (m_BarPtarrayFace.size() == 0) return 0;

	PT3D* tmp = m_BarPtarrayFace[0];
	for (vector<PT3D*>::iterator it = m_BarPtarrayFace.begin(); it != m_BarPtarrayFace.end(); it++)
	{
		m_Alength += sqrt(pow((*it)->x - tmp->x,2) + pow((*it)->y - tmp->y,2) + pow((*it)->z - tmp->z,2));
		tmp = *it;
	}
	m_Aweight = m_Alength * m_Uweight;
	return m_Alength;
}



//获得钢筋总重量(m)
double CBarBase::getAweight()
{
	return m_Aweight;
}



//由相对点集生成实际点集
void CBarBase::setRealBarPtarray()
{
	for (unsigned int i = 0; i < m_BarPtarrayFace.size(); i++)
	{
		m_BarPtarrayReal[i]->x = m_BarPtarrayFace[i]->x + m_LocBase.x;
		m_BarPtarrayReal[i]->y = m_BarPtarrayFace[i]->y + m_LocBase.y;
		m_BarPtarrayReal[i]->z = m_BarPtarrayFace[i]->z + m_LocBase.z;
	}
}



//求两直线交点(两点两斜率式)
void CBarBase::getInters(double x1, double y1, double x2, double y2, double k1, double k2, double &x, double &y)
{
	double bb1, bb2;

	if (k1 > 1000)
		bb1 = 1000;
	else
		bb1 = y1 - k1 * x1;

	if (k2 > 1000)
		bb2 = 1000;
	else
		bb2 = y2 - k2 * x2;

	if (k1 > 1000 && k2 < 1000)
	{
		x = x1;
		y = k2 * x + bb2;
	}
	else if (k2 > 1000 && k1 < 1000)
	{
		x = x2;
		y = k1 * x + bb1;
	}
	else if (k1 > 1000 && k2 > 1000)
	{
		x = 0;
		y = 0;
	}
	else
	{
		x = (bb2 - bb1) / (k1 - k2);
		y = k1 * x + bb1;
	}
}



void CBarBase::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}


