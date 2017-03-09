#include "stdafx.h"
#include "Pile.h"
#include <osg/ShapeDrawable>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CPile, CObject, 0);
CPile::CPile() 
	: m_CenterX(0.0), m_CenterY(0.0), m_CenterZ(0.0)
	, m_Diameter(2.0), m_Length(40.0), m_Type(_T("摩擦桩"))
{
	m_InfoPair = new std::vector<CStringPair>;
	CString str;
	
	m_InfoPair->push_back(CStringPair(_T("工程对象"), _T("桩基")));

	str = "";
	m_InfoPair->push_back(CStringPair(_T("桩类型"), m_Type));

	str = "";
	str.Format(_T("%.2f"),m_Diameter);
	m_InfoPair->push_back(CStringPair(_T("桩径"), str));

	str = "";
	str.Format(_T("%.2f"), m_Length);
	m_InfoPair->push_back(CStringPair(_T("桩长"), str));


	str = "";
	str.Format(_T("(%.2f , %.2f , %.2f)"), m_CenterX,m_CenterY,m_CenterZ);
	m_InfoPair->push_back(CStringPair(_T("位置"), str));

	_root = new osg::Group;
	_callback = new CPileCallback(this);
	
}

CPile::CPile(double cx, double cy, double cz, double diameter, double length)
	: m_CenterX(cx), m_CenterY(cy), m_CenterZ(cz)
	, m_Diameter(diameter), m_Length(length), m_Type(_T("摩擦桩"))
{
	m_InfoPair = new std::vector<CStringPair>;
	CString str;

	m_InfoPair->push_back(CStringPair(_T("工程对象"), _T("桩基")));

	str = "";
	m_InfoPair->push_back(CStringPair(_T("桩类型"), m_Type));

	str = "";
	str.Format(_T("%.2f"), m_Diameter);
	m_InfoPair->push_back(CStringPair(_T("桩径"), str));

	str = "";
	str.Format(_T("%.2f"), m_Length);
	m_InfoPair->push_back(CStringPair(_T("桩长"), str));


	str = "";
	str.Format(_T("(%.2f , %.2f , %.2f)"), m_CenterX, m_CenterY, m_CenterZ);
	m_InfoPair->push_back(CStringPair(_T("位置"), str));

	_root = new osg::Group;
	_callback = new CPileCallback(this);
}

CPile::~CPile()
{
}

void CPile::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << m_CenterX << m_CenterY << m_CenterZ;
		ar << m_Diameter << m_Length;
	}
	else
	{
		ar >> m_CenterX >> m_CenterY >> m_CenterZ;
		ar >> m_Diameter >> m_Length;
	}
}



// CPile 成员函数
double CPile::getConcreteVolume()
{
	return 0;
}

void CPile::GetModelAttrib()
{
	auto pMainWnd = (AfxGetApp()->GetMainWnd());
	if (pMainWnd != nullptr)
	{
		pMainWnd->SendMessage(WM_MYMSG, 0, (LPARAM)m_InfoPair);
	}
}



void CPileCallback::DoUpdate()
{
	osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(_pile->m_CenterX, _pile->m_CenterY, _pile->m_CenterZ - _pile->m_Length / 2), _pile->m_Diameter / 2, _pile->m_Length);
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(0.2);
	osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(cylinder, hints);
	osg::ref_ptr<CClassGeode> geode = new CClassGeode(_pile);
	drawable->setColor(osg::Vec4(0.654, 0.555, 0.13, 1.0f));
	geode->addDrawable(drawable);
	_root->addChild(geode);
}
