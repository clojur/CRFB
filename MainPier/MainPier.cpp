#include "stdafx.h"
#include "MainPier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CMainPier, CObject, 0);
CMainPier::CMainPier()
{
	_root = new osg::Group;
	_callback = new CMainPierCallback(this);
	m_Cap = nullptr;       //承台对象
}

CMainPier::CMainPier(CRoadData* road, double cx, double cy, double bottomLen, double width)
	: m_Road(road)
	, m_CenterX(cx), m_CenterY(cy)
	, m_Length(bottomLen), m_Width(width)
	, m_RootHeight(0.0)
{
	_root = new osg::Group;
	_callback = new CMainPierCallback(this);
}

CMainPier::~CMainPier()
{
	delete m_Cap;
	m_Cap = nullptr;

	for (vector<CPierBody*>::iterator it = m_PierBodys.begin(); it != m_PierBodys.end(); it++)
		if (nullptr != *it)
		{
			delete *it;
			*it = nullptr;
		}
	m_PierBodys.clear();


	for (vector<CPile*>::iterator it = m_Piles.begin(); it != m_Piles.end(); it++)
		if (nullptr != *it)
		{
			delete *it;
			*it = nullptr;
		}
	m_Piles.clear();
}



void CMainPier::Serialize(CArchive &ar)
{
	int count = 0;
	if (ar.IsStoring())
	{
		ar << m_Cap;
		int count = m_Piles.size();
		ar << count;
		for (int i = 0; i < count; i++)
		{
			ar << m_Piles[i];
		}
	}
	else
	{
		ar >> m_Cap;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			CPile* newPile;
			ar >> newPile;
			m_Piles.push_back(newPile);
		}
	}
}



// CCap 成员函数
double CMainPier::getConcreteVolume()
{
	return 0;
}

void CMainPier::SetPierParam(double thickness, double rootHeight)
{
	m_PierThickness = thickness;
	m_RootHeight = rootHeight;
}

void CMainPier::SetCapParam(double thickness, double padding)
{
	m_CapThickness = thickness;
	m_CapPadding = padding;
}

void CMainPier::SetPileParam(double diameter, double length)
{
	m_PileDiameter = diameter;
	m_PileLength = length;
}

void CMainPier::Create()
{
	double capEle = 0.0;
	m_Road->GetGroundElevation(m_CenterX, capEle);
	m_Cap = new CCap(m_CenterX, m_CenterY, capEle, m_Length + 2 * m_CapPadding, m_Width + 2 * m_CapPadding, m_CapThickness);

	m_Piles.push_back(new CPile(m_CenterX, 0.0, m_Cap->GetBottom(), 2.0, 40.0));
	m_Piles.push_back(new CPile(m_CenterX, 0.0 + 2.5 * m_PileDiameter, m_Cap->GetBottom(), 2.0, 40.0));
	m_Piles.push_back(new CPile(m_CenterX, 0.0 - 2.5 * m_PileDiameter, m_Cap->GetBottom(), 2.0, 40.0));
	m_Piles.push_back(new CPile(m_CenterX + 2.5 * m_PileDiameter, 0.0, m_Cap->GetBottom(), 2.0, 40.0));
	m_Piles.push_back(new CPile(m_CenterX + 2.5 * m_PileDiameter, 0.0 + 2.5 * m_PileDiameter, m_Cap->GetBottom(), 2.0, 40.0));
	m_Piles.push_back(new CPile(m_CenterX + 2.5 * m_PileDiameter, 0.0 - 2.5 * m_PileDiameter, m_Cap->GetBottom(), 2.0, 40.0));
	m_Piles.push_back(new CPile(m_CenterX - 2.5 * m_PileDiameter, 0.0, m_Cap->GetBottom(), 2.0, 40.0));
	m_Piles.push_back(new CPile(m_CenterX - 2.5 * m_PileDiameter, 0.0 + 2.5 * m_PileDiameter, m_Cap->GetBottom(), 2.0, 40.0));
	m_Piles.push_back(new CPile(m_CenterX - 2.5 * m_PileDiameter, 0.0 - 2.5 * m_PileDiameter, m_Cap->GetBottom(), 2.0, 40.0));

	double front = m_CenterY - m_Width / 2;
	double back = m_CenterY + m_Width / 2;
	double bottom = m_Cap->GetTop();

	double left = m_CenterX - m_Length / 2;
	double right = left + m_PierThickness;
	double leftTop = 0.0, rightTop = 0.0;
	m_Road->GetDesignElevation(left, leftTop);
	m_Road->GetDesignElevation(right, rightTop);
	leftTop -= m_RootHeight;
	rightTop -= m_RootHeight;
	m_PierBodys.push_back(new CPierBody(left, right, front, back, leftTop, rightTop, bottom));

	right = m_CenterX + m_Length / 2;
	left = right - m_PierThickness;
	m_Road->GetDesignElevation(left, leftTop);
	m_Road->GetDesignElevation(right, rightTop);
	leftTop -= m_RootHeight;
	rightTop -= m_RootHeight;
	m_PierBodys.push_back(new CPierBody(left, right, front, back, leftTop, rightTop, bottom));
}



void CMainPierCallback::DoUpdate()
{
	_root->addChild(_pier->m_Cap->getRoot());

	for (unsigned int i = 0; i < _pier->m_Piles.size(); i++)
	{
		_root->addChild(_pier->m_Piles[i]->getRoot());
	}

	for (unsigned int i = 0; i < _pier->m_PierBodys.size(); i++)
	{
		_root->addChild(_pier->m_PierBodys[i]->getRoot());
	}
}
