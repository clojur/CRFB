#include "stdafx.h"
#include "Cap.h"
#include <osg/ShapeDrawable>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CCap, CObject, 0);
CCap::CCap() :m_LWidth(11.2), m_CWidth(11.2), m_Thickness(4.0)
{
	_root = new osg::Group;
	_callback = new CCapCallback(this);
}

CCap::CCap(double cx, double cy, double cz, double length, double width, double depth)
{
	m_InfoPair = new std::vector<CStringPair>;
	CString str;
	m_CWidth = width;
	m_LWidth = length;
	m_Thickness = depth;

	m_CenterX = cx;
	m_CenterY = cy;
	m_CenterZ = cz;

	m_InfoPair->push_back(CStringPair(_T("工程对象"), _T("承台")));

	str = "";
	str.Format(_T("%.2f"), m_LWidth);
	m_InfoPair->push_back(CStringPair(_T("顺桥向宽度"), str));

	str = "";
	str.Format(_T("%.2f"), m_CWidth);
	m_InfoPair->push_back(CStringPair(_T("横桥向宽度"), str));

	str = "";
	str.Format(_T("%.2f"), m_Thickness);
	m_InfoPair->push_back(CStringPair(_T("厚度"), str));

	str = "";
	str.Format(_T("(%.2f , %.2f , %.2f)"), m_CenterX,m_CenterY,m_CenterZ);
	m_InfoPair->push_back(CStringPair(_T("位置"), str));
	
	_root = new osg::Group;
	_callback = new CCapCallback(this);
}

CCap::~CCap()
{
	delete m_InfoPair;
}

void CCap::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << m_LWidth << m_CWidth << m_Thickness;
		ar << m_CenterX << m_CenterY << m_CenterZ;
	}
	else
	{
		ar >> m_LWidth >> m_CWidth >> m_Thickness;
		ar >> m_CenterX >> m_CenterY >> m_CenterZ;
	}
}



// CCap 成员函数
double CCap::getConcreteVolume()
{
	return 0;
}

void CCap::GetModelAttrib()
{
	auto pMainWnd = (AfxGetApp()->GetMainWnd());
	if (pMainWnd != nullptr)
	{
		pMainWnd->SendMessage(WM_MYMSG, 0,(LPARAM)m_InfoPair);
	}
		
}



void CCapCallback::DoUpdate()
{
	osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(_cap->m_CenterX, _cap->m_CenterY, _cap->m_CenterZ), _cap->m_LWidth, _cap->m_CWidth, _cap->m_Thickness);
	osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(box);
	osg::ref_ptr<CClassGeode> geode = new CClassGeode(_cap);
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	drawable->setColor(osg::Vec4(0.4f, 0.3f, 0.5f, 1.0f));
	geode->addDrawable(drawable);

	_root->addChild(geode);
}

