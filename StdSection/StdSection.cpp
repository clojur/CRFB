#include "stdafx.h"
#include "StdSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CStdSection, CObject, 0);
CStdSection::CStdSection()
{
	m_LocElevation = _T("设计中心线");
	m_SlpType = _T("单向坡");
	m_SlpVal = -0.02;
	m_Guardrail = 0.5;

	m_DboxWidth = 6.0;
	m_TboxWidth = 12.0;
	m_A = 0.2;
	m_B = 1.0;
	m_H = 0.29;
	m_A1 = 0.75;
	m_A2 = 2.0;
	m_A3 = 2.0;
	m_A4 = 1.0;
	m_A5 = 0.5;
	m_B1 = 0.1;
	m_B2 = 0.35;
	m_B3 = 0.65;
	m_B4 = 0.1;
	m_B5 = 0.1;
	m_B6 = 0.58;
	m_B7 = 0.5;
	m_B8 = 0.1;
	m_B9 = 0.5;
}

CStdSection::CStdSection(CStdSection& section)
{
	m_LocElevation = section.m_LocElevation;
	m_SlpType = section.m_SlpType;
	m_SlpVal = section.m_SlpVal;
	m_Guardrail = section.m_Guardrail;

	m_DboxWidth = section.m_DboxWidth;
	m_TboxWidth = section.m_TboxWidth;
	m_A = section.m_A;
	m_B = section.m_B;
	m_H = section.m_H;
	m_A1 = section.m_A1;
	m_A2 = section.m_A2;
	m_A3 = section.m_A3;
	m_A4 = section.m_A4;
	m_A5 = section.m_A5;
	m_B1 = section.m_B1;
	m_B2 = section.m_B2;
	m_B3 = section.m_B3;
	m_B4 = section.m_B4;
	m_B5 = section.m_B5;
	m_B6 = section.m_B6;
	m_B7 = section.m_B7;
	m_B8 = section.m_B8;
	m_B9 = section.m_B9;
}

CStdSection::~CStdSection()
{
}

void CStdSection::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << m_LocElevation << m_SlpType << m_SlpVal
		   << m_TboxWidth << m_DboxWidth
		   << m_A << m_B << m_H << m_A1 << m_A2 << m_A3 << m_A4 << m_A5
           << m_B1 << m_B2 << m_B3 << m_B4 << m_B5 << m_B6 << m_B7 << m_B8 << m_B9;
	}
	else{
		ar >> m_LocElevation >> m_SlpType >> m_SlpVal
		   >> m_TboxWidth >> m_DboxWidth
		   >> m_A >> m_B >> m_H >> m_A1 >> m_A2 >> m_A3 >> m_A4 >> m_A5
		   >> m_B1 >> m_B2 >> m_B3 >> m_B4 >> m_B5 >> m_B6 >> m_B7 >> m_B8 >> m_B9;
	}
}

osg::ref_ptr<osg::Geometry> CStdSection::Draw(double stake, double centerY, double centerZ, double height)
{
	osg::ref_ptr<osg::Vec3Array> outer = new osg::Vec3Array(11);
	osg::ref_ptr<osg::Vec3Array> inner = new osg::Vec3Array(11);
	return Draw(stake, centerY, centerZ, height, outer, inner);
}

osg::ref_ptr<osg::Geometry> CStdSection::Draw(double stake, double centerY, double centerZ, double height, osg::Vec3Array* outer, osg::Vec3Array* inner)
{
	CalcOuter(stake, centerY, centerZ, height, outer);
	CalcInner(stake, centerY, centerZ, height, inner);

	osg::ref_ptr<osg::Vec3Array> vertexs = new osg::Vec3Array(*outer);
	vertexs->insert(vertexs->end(), inner->begin(), inner->end());
	
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	geometry->setVertexArray(vertexs);
	
	osg::ref_ptr<osg::DrawElementsUInt> top_left1 = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_FAN, 0);
	top_left1->push_back(1);
	top_left1->push_back(0);
	top_left1->push_back(11);
	top_left1->push_back(12);
	top_left1->push_back(13);
	
	osg::ref_ptr<osg::DrawElementsUInt> top_left2 = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_FAN, 0);
	top_left2->push_back(13);
	top_left2->push_back(1);
	top_left2->push_back(2);
	top_left2->push_back(3);
	top_left2->push_back(4);
	top_left2->push_back(14);
	
	osg::ref_ptr<osg::DrawElementsUInt> top_right1 = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_FAN, 0);
	top_right1->push_back(10);
	top_right1->push_back(0);
	top_right1->push_back(11);
	top_right1->push_back(21);
	top_right1->push_back(20);
	
	osg::ref_ptr<osg::DrawElementsUInt> top_right2 = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_FAN, 0);
	top_right2->push_back(20);
	top_right2->push_back(10);
	top_right2->push_back(9);
	top_right2->push_back(8);
	top_right2->push_back(7);
	top_right2->push_back(19);
	
	osg::ref_ptr<osg::DrawElementsUInt> left = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_FAN, 0);
	left->push_back(5);
	left->push_back(4);
	left->push_back(14);
	left->push_back(15);
	left->push_back(16);
	
	osg::ref_ptr<osg::DrawElementsUInt> right = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_FAN, 0);
	right->push_back(6);
	right->push_back(7);
	right->push_back(19);
	right->push_back(18);
	right->push_back(17);
	
	osg::ref_ptr<osg::DrawElementsUInt> bottom = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	bottom->push_back(5);
	bottom->push_back(6);
	bottom->push_back(17);
	bottom->push_back(16);

	geometry->addPrimitiveSet(top_left1);
	geometry->addPrimitiveSet(top_left2);
	geometry->addPrimitiveSet(top_right1);
	geometry->addPrimitiveSet(top_right2);
	geometry->addPrimitiveSet(left);
	geometry->addPrimitiveSet(right);
	geometry->addPrimitiveSet(bottom);

	return geometry;
}

void CStdSection::CalcOuter(double stake, double centerY, double centerZ, double height, osg::Vec3Array* outer)
{
	double y = 0.0, z = 0.0;

	if (outer->size() != 11)
		outer->resize(11);


	outer->at(0) = osg::Vec3(stake, centerY, centerZ);

	y = centerY + m_TboxWidth / 2;
	z = centerZ + m_TboxWidth / 2 * m_SlpVal;
	outer->at(1) = osg::Vec3(stake, y, z);

	z -= m_A;
	outer->at(2) = osg::Vec3(stake, y, z);

	y -= m_A1;
	z -= m_B1;
	outer->at(3) = osg::Vec3(stake, y, z);

	y = centerY + m_DboxWidth / 2;
	z -= m_B2;
	outer->at(4) = osg::Vec3(stake, y, z);

	y = centerY - m_TboxWidth / 2;
	z = centerZ - m_TboxWidth / 2 * m_SlpVal;
	outer->at(10) = osg::Vec3(stake, y, z);

	z -= m_A;
	outer->at(9) = osg::Vec3(stake, y, z);

	y += m_A1;
	z -= m_B8;
	outer->at(8) = osg::Vec3(stake, y, z);

	y = centerY - m_DboxWidth / 2;
	z -= m_B7;
	outer->at(7) = osg::Vec3(stake, y, z);

	if (m_SlpVal < 0)
	{
		z = centerZ + m_DboxWidth / 2 * m_SlpVal - height;

	}
	else
	{
		z = centerZ - m_DboxWidth / 2 * m_SlpVal - height;
	}
	outer->at(5) = osg::Vec3(stake, centerY + m_DboxWidth / 2, z);
	outer->at(6) = osg::Vec3(stake, centerY - m_DboxWidth / 2, z);
}

void CStdSection::CalcInner(double stake, double centerY, double centerZ, double height, osg::Vec3Array* inner)
{
	if (inner->size() != 11)
		inner->resize(11);

	double y = 0.0, z = 0.0;
	double fbh = 0.5;
	double dbh = 0.5;
	double bottom = centerZ - m_DboxWidth / 2 * fabs(m_SlpVal) - height + dbh;

	inner->at(0) = osg::Vec3(stake, centerY, centerZ - m_H);

	y = centerY + m_B / 2;
	z = centerZ - m_H + m_B / 2 * m_SlpVal;
	inner->at(1) = osg::Vec3(stake, y, z);

	y += m_A4;
	z -= m_B4;
	inner->at(2) = osg::Vec3(stake, y, z);

	z = z - m_B3 / m_A3 * (centerY + m_DboxWidth / 2 - fbh - y);
	y = centerY + m_DboxWidth / 2 - fbh;
	inner->at(3) = osg::Vec3(stake, y, z);

	z = bottom + m_A5;
	inner->at(4) = osg::Vec3(stake, y, z);
	inner->at(7) = osg::Vec3(stake, centerY - m_DboxWidth / 2 + fbh, z);

	inner->at(5) = osg::Vec3(stake, centerY + m_DboxWidth / 2 - fbh - m_B9, bottom);
	inner->at(6) = osg::Vec3(stake, centerY - m_DboxWidth / 2 + fbh + m_B9, bottom);

	y = centerY - m_B / 2;
	z = centerZ - m_H - m_B / 2 * m_SlpVal;
	inner->at(10) = osg::Vec3(stake, y, z);

	y -= m_A4;
	z -= m_B5;
	inner->at(9) = osg::Vec3(stake, y, z);

	z = z - m_B6 / m_A3 * (y - centerY + m_DboxWidth / 2 - fbh);
	y = centerY - m_DboxWidth / 2 + fbh;
	inner->at(8) = osg::Vec3(stake, y, z);
}
