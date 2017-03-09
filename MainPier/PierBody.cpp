#include "stdafx.h"
#include "PierBody.h"
#include <osgUtil/SmoothingVisitor>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CPierBody, CObject, 0);
CPierBody::CPierBody() :m_LWidth(9.0), m_CWidth(6.5), m_Height(70.0), m_WallThick(2.0)
{
	m_InfoPair = new std::vector<CStringPair>;
	CString str;

	m_InfoPair->push_back(CStringPair(_T("工程对象"), _T("墩身")));

	str = "";
	str.Format(_T("%.2f"), m_LWidth);
	m_InfoPair->push_back(CStringPair(_T("顺桥向宽度"), str));

	str = "";
	str.Format(_T("%.2f"), m_CWidth);
	m_InfoPair->push_back(CStringPair(_T("横桥向宽度"), str));

	str = "";
	str.Format(_T("%.2f"), m_Height);
	m_InfoPair->push_back(CStringPair(_T("墩高"), str));

	str = "";
	str.Format(_T("%.2f"), m_WallThick);
	m_InfoPair->push_back(CStringPair(_T("敦厚"), str));

	_root = new osg::Group;
	_callback = new CPierBodyCallback(this);
}

CPierBody::CPierBody(double left, double right, double front, double back, double leftTop, double rightTop, double bottom)
	: m_Left(left), m_Right(right)
	, m_Front(front), m_Back(back)
	, m_LeftTop(leftTop), m_RightTop(rightTop), m_Bottom(bottom)
{
	m_InfoPair = new std::vector<CStringPair>;
	CString str;

	m_InfoPair->push_back(CStringPair(_T("工程对象"), _T("墩身")));

	str = "";
	str.Format(_T("%.2f"), m_LWidth);
	m_InfoPair->push_back(CStringPair(_T("顺桥向宽度"), str));

	str = "";
	str.Format(_T("%.2f"), m_CWidth);
	m_InfoPair->push_back(CStringPair(_T("横桥向宽度"), str));

	str = "";
	str.Format(_T("%.2f"), m_Height);
	m_InfoPair->push_back(CStringPair(_T("墩高"), str));

	str = "";
	str.Format(_T("%.2f"), m_WallThick);
	m_InfoPair->push_back(CStringPair(_T("敦厚"), str));

	_root = new osg::Group;
	_callback = new CPierBodyCallback(this);
}

CPierBody::~CPierBody()
{
}

void CPierBody::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << m_LWidth << m_CWidth << m_Height << m_WallThick;
		ar << m_Left << m_Right << m_Front << m_Back;
		ar << m_LeftTop << m_RightTop << m_Bottom;
	}
	else
	{
		ar >> m_LWidth >> m_CWidth >> m_Height >> m_WallThick;
		ar >> m_Left >> m_Right >> m_Front >> m_Back;
		ar >> m_LeftTop >> m_RightTop >> m_Bottom;
	}
}



// CCap 成员函数
double CPierBody::getConcreteVolume()
{
	return 0;
}

void CPierBody::GetModelAttrib()
{
	auto pMainWnd = (AfxGetApp()->GetMainWnd());
	if (pMainWnd != nullptr)
	{
		pMainWnd->SendMessage(WM_MYMSG, 0, (LPARAM)m_InfoPair);
	}
}



void CPierBodyCallback::DoUpdate()
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
	vertices->push_back(osg::Vec3(_pierbody->m_Left, _pierbody->m_Back, _pierbody->m_LeftTop));
	vertices->push_back(osg::Vec3(_pierbody->m_Left, _pierbody->m_Back, _pierbody->m_Bottom));
	vertices->push_back(osg::Vec3(_pierbody->m_Left, _pierbody->m_Front, _pierbody->m_Bottom));
	vertices->push_back(osg::Vec3(_pierbody->m_Left, _pierbody->m_Front, _pierbody->m_LeftTop));
	vertices->push_back(osg::Vec3(_pierbody->m_Right, _pierbody->m_Back, _pierbody->m_RightTop));
	vertices->push_back(osg::Vec3(_pierbody->m_Right, _pierbody->m_Back, _pierbody->m_Bottom));
	vertices->push_back(osg::Vec3(_pierbody->m_Right, _pierbody->m_Front, _pierbody->m_Bottom));
	vertices->push_back(osg::Vec3(_pierbody->m_Right, _pierbody->m_Front, _pierbody->m_RightTop));

	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	geometry->setVertexArray(vertices);

	osg::ref_ptr<osg::DrawElementsUInt> left = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	left->push_back(0);
	left->push_back(1);
	left->push_back(2);
	left->push_back(3);
	geometry->addPrimitiveSet(left);

	osg::ref_ptr<osg::DrawElementsUInt> right = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	right->push_back(4);
	right->push_back(5);
	right->push_back(6);
	right->push_back(7);
	geometry->addPrimitiveSet(right);

	osg::ref_ptr<osg::DrawElementsUInt> up = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	up->push_back(0);
	up->push_back(3);
	up->push_back(7);
	up->push_back(4);
	geometry->addPrimitiveSet(up);

	osg::ref_ptr<osg::DrawElementsUInt> down = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	down->push_back(1);
	down->push_back(2);
	down->push_back(6);
	down->push_back(5);
	geometry->addPrimitiveSet(down);

	osg::ref_ptr<osg::DrawElementsUInt> front = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	front->push_back(3);
	front->push_back(2);
	front->push_back(6);
	front->push_back(7);
	geometry->addPrimitiveSet(front);

	osg::ref_ptr<osg::DrawElementsUInt> back = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	back->push_back(0);
	back->push_back(1);
	back->push_back(5);
	back->push_back(4);
	geometry->addPrimitiveSet(back);

	//osgUtil::SmoothingVisitor::smooth(*geometry);
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->push_back(osg::Vec4(0.5, 0.5, 0.5, 1.0));
	geometry->setColorArray(colors, osg::Array::BIND_OVERALL);

	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
	n->push_back(osg::Vec3(-1.0, 0.0, 0.0));
	n->push_back(osg::Vec3(1.0, 0.0, 0.0));
	n->push_back(osg::Vec3(0.0, 0.0, 1.0));
	n->push_back(osg::Vec3(0.0, 0.0, -1.0));
	n->push_back(osg::Vec3(0.0, -1.0, 0.0));
	n->push_back(osg::Vec3(0.0, 1.0, 0.0));
	geometry->setNormalArray(n, osg::Array::BIND_PER_PRIMITIVE_SET);

	CClassGeode* geode = new CClassGeode(_pierbody);
	geode->addDrawable(geometry);
	_root->addChild(geode);
}
