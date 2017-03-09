// YNPoint3D.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Point.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//�����л�double��2D�����

IMPLEMENT_SERIAL(CYNPoint2D, CObject, 0);

CYNPoint2D::CYNPoint2D()
{
}

CYNPoint2D::CYNPoint2D(double CoordinateX, double CoordinateY) :
m_CoordinateX(CoordinateX), m_CoordinateY(CoordinateY)
{
}

CYNPoint2D::~CYNPoint2D()
{
}

CYNPoint2D& CYNPoint2D::operator = (CYNPoint2D& src)
{
	this->m_CoordinateX = src.m_CoordinateX;
	this->m_CoordinateY = src.m_CoordinateY;
	return *this;
}

void CYNPoint2D::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_CoordinateX << m_CoordinateY;
	}
	else
	{	// loading code
		ar >> m_CoordinateX >> m_CoordinateY;
	}
}

//void CYNPoint2D::SetPtX(double CoordinateX) //���õ�����X
//{ 
//	m_CoordinateX = CoordinateX; 
//}
//
//void CYNPoint2D::SetPtY(double CoordinateY) //���õ�����Y
//{ 
//	m_CoordinateY = CoordinateY; 
//}
//
//double CYNPoint2D::GetPtX() //��õ�����X
//{ 
//	return m_CoordinateX; 
//}
//
//double CYNPoint2D::GetPtY() //��õ�����Y
//{ 
//	return m_CoordinateY; 
//}



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//�����л�double��3D�����

IMPLEMENT_SERIAL(CYNPoint3D, CObject, 0);

CYNPoint3D::CYNPoint3D()
{
}

CYNPoint3D::CYNPoint3D(double CoordinateX, double CoordinateY, double CoordinateZ) :
m_CoordinateX(CoordinateX), m_CoordinateY(CoordinateY), m_CoordinateZ(CoordinateZ)
{
}

CYNPoint3D::~CYNPoint3D()
{
}

CYNPoint3D& CYNPoint3D::operator = (CYNPoint3D& src)
{
	this->m_CoordinateX = src.m_CoordinateX;
	this->m_CoordinateY = src.m_CoordinateY;
	this->m_CoordinateZ = src.m_CoordinateZ;
	return *this;
}

void CYNPoint3D::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_CoordinateX << m_CoordinateY << m_CoordinateZ;
	}
	else
	{  // loading code
		ar >> m_CoordinateX >> m_CoordinateY >> m_CoordinateZ;
	}
}

//void CYNPoint3D::SetPtX(double CoordinateX) //���õ�����X
//{
//	m_CoordinateX = CoordinateX;
//}
//
//void CYNPoint3D::SetPtY(double CoordinateY) //���õ�����Y
//{
//	m_CoordinateY = CoordinateY;
//}
//
//void CYNPoint3D::SetPtZ(double CoordinateZ) //���õ�����Z
//{
//	m_CoordinateZ = CoordinateZ;
//}
//
//
//double CYNPoint3D::GetPtX() //��õ�����X
//{
//	return m_CoordinateX;
//}
//
//double CYNPoint3D::GetPtY() //��õ�����Y
//{
//	return m_CoordinateY;
//}
//
//double CYNPoint3D::GetPtZ() //��õ�����Z
//{
//	return m_CoordinateZ;
//}
