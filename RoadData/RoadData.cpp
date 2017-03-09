// RoadData.cpp : 实现文件
//

#include "stdafx.h"
#include "RoadData.h"
#include <fstream>
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CGroundPoint, CObject, 0);

CGroundPoint::CGroundPoint()
{
}

CGroundPoint::CGroundPoint(double stake, double elevation) :
m_Stake(stake), m_Elevation(elevation)
{
}

CGroundPoint::CGroundPoint(const CGroundPoint& ground)
	: m_Stake(ground.m_Stake), m_Elevation(ground.m_Elevation)
{
}

CGroundPoint::~CGroundPoint()
{
}


CGroundPoint& CGroundPoint::operator = (const CGroundPoint& src)
{
	this->m_Stake = src.m_Stake;
	this->m_Elevation = src.m_Elevation;
	return *this;
}


//设计线上的点

IMPLEMENT_SERIAL(CDesignPoint, CObject, 0);

CDesignPoint::CDesignPoint()
{
}

CDesignPoint::CDesignPoint(double stake, double elevation, double radius) :
m_Stake(stake), m_Elevation(elevation), m_Radius(radius)
{
}

CDesignPoint::~CDesignPoint()
{
}

CDesignPoint& CDesignPoint::operator = (const CDesignPoint& src)
{
	this->m_Stake = src.m_Stake;
	this->m_Elevation = src.m_Elevation;
	this->m_Radius = src.m_Radius;
	this->SetParams(0, src.GetBSta(), src.GetESta(), src.GetCurveType());
	return *this;
}

void CDesignPoint::SetParams(double t, double b_sta, double e_sta, bool ctype)
{
	this->t = t;
	this->b_sta = b_sta;
	this->e_sta = e_sta;
	this->ctype = ctype;
}


// CRoadData
IMPLEMENT_SERIAL(CRoadData, CObject, 0);
CRoadData::CRoadData()
	: m_Profile(NULL)
	, m_LeftBoundry(-1)
	, m_RightBoundry(-1)
	, m_GroundFile(_T(""))
	, m_DesignFile(_T(""))
	, m_CrossFile(_T(""))
{
	_root = new osg::Group;
	_callback = new CRoadDataCallback(this);
}

CRoadData::CRoadData(CRoadData& road)
	: m_Profile(road.m_Profile)
	, m_LeftSection(road.m_LeftSection)
	, m_RightSection(road.m_RightSection)
	, m_LeftBoundry(road.m_LeftBoundry)
	, m_RightBoundry(road.m_RightBoundry)
	, m_DesignFile(road.m_DesignFile)
	, m_GroundFile(road.m_GroundFile)
	, m_CrossFile(road.m_CrossFile)
	, m_WaterLevel(road.m_WaterLevel)
{
	m_GroundPoints.Copy(road.m_GroundPoints);
	m_DesignPoints.Copy(road.m_DesignPoints);
	_root = new osg::Group;
	_callback = new CRoadDataCallback(this);
}

CRoadData& CRoadData::operator = (CRoadData& src)
{
	this->m_Profile = src.m_Profile;
	this->m_LeftSection = src.m_LeftSection;
	this->m_RightSection = src.m_RightSection;
	this->m_LeftBoundry = src.m_LeftBoundry;
	this->m_RightBoundry = src.m_RightBoundry;
	this->m_DesignFile = src.m_DesignFile;
	this->m_GroundFile = src.m_GroundFile;
	this->m_CrossFile = src.m_CrossFile;
	this->m_WaterLevel = src.m_WaterLevel;
	this->m_GroundPoints.Copy(src.m_GroundPoints);
	this->m_DesignPoints.Copy(src.m_DesignPoints);
	return *this;
}


CRoadData::~CRoadData()
{
}


// CRoadData 成员函数
bool CRoadData::LoadGroundLine(CString fileName)
{
	//bool update(false);
	if (m_GroundPoints.GetCount() > 0)
	{
		m_GroundPoints.RemoveAll();
		//update = true;
	}

	if (m_Profile == NULL)
	{
		m_Profile = new osg::Vec3Array();
	}

	if (m_Profile->size() > 0)
	{
		m_Profile->clear();
		//update = true;
	}

	std::fstream f;
	f.open(CStringA(fileName), std::ios::in);
	if (!f.is_open())
	{
		return false;
	}
	std::string line;
	std::getline(f, line);
	while (std::getline(f, line))
	{
		double mileage;
		double height;
		std::stringstream s(line);
		s >> mileage;
		s >> height;
		m_GroundPoints.Add(CGroundPoint(mileage, height));
		m_Profile->push_back(osg::Vec3(mileage, 0.0, height));
	}

	f.close();

	m_GroundFile = fileName;

	return true;
}

bool CRoadData::LoadCrossSection(CString fileName)
{
	bool update(false);

	if (m_LeftSection.size() > 0 || m_RightSection.size() > 0)
	{
		m_LeftSection.clear();
		m_RightSection.clear();
		update = true;
	}

	DWORD start = GetTickCount();

	std::fstream f;
	f.sync_with_stdio(false);
	char buf[8 * 1024];
	f.rdbuf()->pubsetbuf(buf, sizeof(buf));
	//std::ios::sync_with_stdio(false);
	f.open(CStringA(fileName), std::ios::in);
	if (!f.is_open())
	{
		return false;
	}


	unsigned int stk = 0;
	std::string line;
	std::getline(f, line);
	DWORD spend = 0;
	std::istringstream ls;
	ls.sync_with_stdio(false);
	while (!f.eof() && stk < m_Profile->size())
	{
		f.getline(buf, sizeof(buf));
		line.assign(buf);
		if (line.empty())
		{
			continue;
		}

		int count = 0;
		double curY(0.0), curZ(0.0), curX(0.0);
		curX = atof(line.c_str());

		osg::ref_ptr<osg::Vec3Array> left = new osg::Vec3Array();
		curY = m_Profile->at(stk).y();
		curZ = m_Profile->at(stk).z();
		f.getline(buf, sizeof(buf));
		char *next = NULL;
		char *token = strtok_s(buf, "\t ", &next);
		if (token)
			count = atoi(token);
		double y = 0.0, z = 0.0;
		while (count-- && token)
		{
			token = strtok_s(NULL, "\t ", &next);
			if (token)
				y = atof(token);

			token = strtok_s(NULL, "\t ", &next);
			if (token)
				z = atof(token);

			curY += y;
			curZ += z;
			left->push_back(osg::Vec3(curX, curY, curZ));
		}

		osg::ref_ptr<osg::Vec3Array> right = new osg::Vec3Array();
		curY = m_Profile->at(stk).y();
		curZ = m_Profile->at(stk).z();
		f.getline(buf, sizeof(buf));
		next = NULL;
		token = strtok_s(buf, "\t ", &next);
		if (token)
			count = atoi(token);
		y = 0.0, z = 0.0;
		while (count-- && token)
		{
			token = strtok_s(NULL, "\t ", &next);
			if (token)
				y = atof(token);

			token = strtok_s(NULL, "\t ", &next);
			if (token)
				z = atof(token);

			curY -= y;
			curZ += z;
			right->push_back(osg::Vec3(curX, curY, curZ));
		}

		m_LeftSection.push_back(left);
		m_RightSection.push_back(right);

		stk++;
	}

	f.close();

	m_CrossFile = fileName;

	DWORD t = GetTickCount() - start;

	return true;
}

bool CRoadData::LoadDesignLine(CString fileName)
{
	bool update(false);
	if (m_DesignPoints.GetCount())
	{
		m_DesignPoints.RemoveAll();
		update = true;
	}

	std::fstream f;
	f.open(CStringA(fileName), std::ios::in);
	if (!f.is_open())
	{
		return false;
	}
	std::string line;
	std::getline(f, line);
	std::getline(f, line);
	int count = atoi(line.c_str());
	while (std::getline(f, line) && count--)
	{
		double stake;
		double height;
		double r;
		std::stringstream s(line);
		s >> stake;
		s >> height;
		s >> r;
		m_DesignPoints.Add(CDesignPoint(stake, height, r));
	}

	f.close();

	m_DesignFile = fileName;

	InitDesignLine();

	return true;
}

void CRoadData::InitDesignLine()
{
	double b, e, t;
	bool ctype;

	int count = m_DesignPoints.GetCount();
	b = e = m_DesignPoints[0].GetStake();
	m_DesignPoints[0].SetParams(0, b, e, true);

	b = e = m_DesignPoints[count - 1].GetStake();
	m_DesignPoints[count - 1].SetParams(0, b, e, true);

	for (int i = 1; i < count - 1; i++)
	{
		double i1, i2;	//坡度
		i1 = (m_DesignPoints[i].GetElevation() - m_DesignPoints[i - 1].GetElevation()) / (m_DesignPoints[i].GetStake() - m_DesignPoints[i - 1].GetStake());
		i2 = (m_DesignPoints[i + 1].GetElevation() - m_DesignPoints[i].GetElevation()) / (m_DesignPoints[i + 1].GetStake() - m_DesignPoints[i].GetStake());

		if (i1 > i2)
			ctype = true;
		else
			ctype = false;

		t = m_DesignPoints[i].GetRadius() * fabs(i1 - i2) / 2;
		b = m_DesignPoints[i].GetStake() - t;
		e = m_DesignPoints[i].GetStake() + t;
		m_DesignPoints[i].SetParams(t, b, e, ctype);
	}
}

bool CRoadData::GetDesignElevation(double stake, double &height)
{
	double mod;
	int count = m_DesignPoints.GetCount();

	if (stake < m_DesignPoints[0].GetStake() || stake > m_DesignPoints[count - 1].GetStake())
	{
		return false;
	}

	for (int i = 1; i < count; i++)
	{
		if (stake <= m_DesignPoints[i].GetESta())
		{
			// 在第一段直线上
			if (stake <= m_DesignPoints[i].GetBSta())
			{
				double k = (m_DesignPoints[i].GetElevation() - m_DesignPoints[i - 1].GetElevation()) / (m_DesignPoints[i].GetStake() - m_DesignPoints[i - 1].GetStake());
				height = m_DesignPoints[i - 1].GetElevation() + (stake - m_DesignPoints[i - 1].GetStake()) * k;
			}
			// 在第一段曲线上
			else if (stake > m_DesignPoints[i].GetBSta() && stake < m_DesignPoints[i].GetESta())
			{
				mod = (stake - m_DesignPoints[i].GetBSta()) * (stake - m_DesignPoints[i].GetBSta()) / (2 * m_DesignPoints[i].GetRadius());
				double k = (m_DesignPoints[i].GetElevation() - m_DesignPoints[i - 1].GetElevation()) / (m_DesignPoints[i].GetStake() - m_DesignPoints[i - 1].GetStake());
				if (m_DesignPoints[i].GetCurveType())
				{
					height = m_DesignPoints[i - 1].GetElevation() + k * (stake - m_DesignPoints[i - 1].GetStake()) - mod;
				}
				else
				{
					height = m_DesignPoints[i - 1].GetElevation() + k * (stake - m_DesignPoints[i - 1].GetStake()) + mod;
				}
			}
			// 在第二段曲线上
			else if (stake > m_DesignPoints[i].GetStake() && stake < m_DesignPoints[i].GetESta())
			{
				mod = (m_DesignPoints[i].GetESta() - stake) * (m_DesignPoints[i].GetESta() - stake) / (2 * m_DesignPoints[i].GetRadius());
				double k = (m_DesignPoints[i + 1].GetElevation() - m_DesignPoints[i].GetElevation()) / (m_DesignPoints[i + 1].GetStake() - m_DesignPoints[i].GetStake());
				if (m_DesignPoints[i].GetCurveType())
				{
					height = m_DesignPoints[i].GetElevation() + k * (stake - m_DesignPoints[i].GetStake()) - mod;
				}
				else
				{
					height = m_DesignPoints[i].GetElevation() + k * (stake - m_DesignPoints[i].GetStake()) + mod;
				}
			}

			return true;
		}
	}

	return false;
}

bool CRoadData::GetGroundElevation(double stake, double &height)
{
	int count = m_GroundPoints.GetCount();
	if (stake < m_GroundPoints[0].GetStake() || stake > m_GroundPoints[count - 1].GetStake())
	{
		return false;
	}

	for (int i = 0; i < count - 1; i++)
	{
		if (stake >= m_GroundPoints[i].GetStake() && stake <= m_GroundPoints[i + 1].GetStake())
		{
			double k = (m_GroundPoints[i + 1].GetElevation() - m_GroundPoints[i].GetElevation()) / (m_GroundPoints[i + 1].GetStake() - m_GroundPoints[i].GetStake());
			height = m_GroundPoints[i].GetElevation() + k * (stake - m_GroundPoints[i].GetStake());
			return true;
		}
	}

	return false;
}

void CRoadData::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_LeftBoundry;
		ar << m_RightBoundry;
		ar << m_DesignFile;
		ar << m_GroundFile;
		ar << m_CrossFile;
		ar << m_WaterLevel;
	}
	else
	{
		ar >> m_LeftBoundry;
		ar >> m_RightBoundry;
		ar >> m_DesignFile;
		ar >> m_GroundFile;
		ar >> m_CrossFile;
		LoadDesignLine(m_DesignFile);
		LoadGroundLine(m_GroundFile);
		LoadCrossSection(m_CrossFile);
		ar >> m_WaterLevel;
	}
}

bool CRoadData::GetTwHeight(double stake, double &height)
{
	int count = m_GroundPoints.GetCount();
	if (stake < m_GroundPoints[0].GetStake() || stake > m_GroundPoints[count - 1].GetStake())
	{
		return false;
	}

	double hDesign, hGround;
	GetDesignElevation(stake, hDesign);
	GetGroundElevation(stake, hGround);

	height = hDesign - hGround;

	return true;
}

bool CRoadData::CalcTwBoundry(double baseStake, double gap, double &left, double &right)
{
	int count = m_GroundPoints.GetCount();
	if (baseStake < m_GroundPoints[0].GetStake() || baseStake > m_GroundPoints[count - 1].GetStake())
	{
		return false;
	}

	double height;
	GetTwHeight(baseStake, height);
	if (height < 0.0)
	{
		return false;
	}
	
	int i;
	for (i = 0; i < count - 1; i++)
	{
		if (m_GroundPoints[i].GetStake() <= baseStake && m_GroundPoints[i + 1].GetStake() >= baseStake)
		{
			break;
		}
	}

	int idxLeftBoundry, idxRightBoundry;
	double prevH;
	GetTwHeight(m_GroundPoints[i].GetStake(), prevH);
	for (int j = i - 1; j >= 0; j--)
	{
		double h;
		GetTwHeight(m_GroundPoints[j].GetStake(), h);
		if (h > 0.0)
		{
			prevH = h;
			continue;
		}

		if (h == 0)
		{
			left = m_GroundPoints[j].GetStake();
		
			idxLeftBoundry = j;
			break;
		}
		else if (h < 0.0 && prevH > 0.0)
		{
			double leftStake = m_GroundPoints[j].GetStake();
			double rightStake = m_GroundPoints[j + 1].GetStake();
			left = leftStake - h * (rightStake - leftStake) / (prevH - h);
			
			idxLeftBoundry = j;
			break;
		}
	}
	
	while (m_GroundPoints[idxLeftBoundry].GetStake() > left - gap)
	{
		if (idxLeftBoundry <= 0)
			break;
		idxLeftBoundry--;
	}


	GetTwHeight(m_GroundPoints[i].GetStake(), prevH);
	for (int j = i + 1; j < count; j++)
	{
		double h;
		GetTwHeight(m_GroundPoints[j].GetStake(), h);
		if (h > 0.0)
		{
			prevH = h;
			continue;
		}

		if (h == 0)
		{
			right = m_GroundPoints[j].GetStake();
			
			idxRightBoundry = j;
			break;
		}
		else if (h < 0.0 && prevH > 0.0)
		{
			double leftStake = m_GroundPoints[j - 1].GetStake();
			double rightStake = m_GroundPoints[j].GetStake();
			right = leftStake + prevH / (prevH - h) * (rightStake - leftStake);
			
			idxRightBoundry = j;
			break;
		}
	}
	while (m_GroundPoints[idxRightBoundry].GetStake() < right + gap)
	{
		if (idxRightBoundry >= count - 1)
			break;
		idxRightBoundry++;
	}

	// 重构部分
	m_GroundPoints.RemoveAt(idxRightBoundry + 1, count - idxRightBoundry - 1);
	m_GroundPoints.RemoveAt(0, idxLeftBoundry);
	if (m_LeftBoundry == -1 && m_RightBoundry == -1)
	{
		m_LeftBoundry = idxLeftBoundry;
		m_RightBoundry = idxRightBoundry;
	}

	for (int j = 1; j < m_DesignPoints.GetCount(); j++)
	{
		if (m_DesignPoints[j - 1].GetStake() <= left - gap && m_DesignPoints[j].GetStake() >= left - gap)
		{
			idxLeftBoundry = j - 1;
		}

		if (m_DesignPoints[j - 1].GetStake() <= right + gap && m_DesignPoints[j].GetStake() > right + gap)
		{
			idxRightBoundry = j;
		}
	}

	m_DesignPoints.RemoveAt(idxRightBoundry + 1, m_DesignPoints.GetCount() - idxRightBoundry - 1);
	m_DesignPoints.RemoveAt(0, idxLeftBoundry);

	return true;
}

vector<double> CRoadData::GetStakeByTwHeight(double height)
{
	vector<double> stakes;
	int count = m_GroundPoints.GetCount();
	for (int i = 0; i < count - 1; i++)
	{

		double h0, h1, x0, x1;
		x0 = m_GroundPoints[i].GetStake();
		x1 = m_GroundPoints[i + 1].GetStake();

		if (x1 == x0)
		{
			stakes.push_back(x0);
			continue;
		}

		GetTwHeight(x0, h0);
		GetTwHeight(x1, h1);

		if ((h0 <= height && h1 >= height) || (h0 >= height && h1 <= height))
		{
			double x = (x1 - x0) * (height - h0) / (h1 - h0) + x0;
			stakes.push_back(x);
		}
	}

	return stakes;
}

void CRoadDataCallback::DoUpdate()
{
	DrawRoadData(_road->m_LeftBoundry, _road->m_RightBoundry);
	osg::ref_ptr<osgGA::GUIEventAdapter> dummyEvent = _view->getEventQueue()->createEvent();
	_view->getCameraManipulator()->home(*dummyEvent, *_view);
}

void CRoadDataCallback::DrawRoadData(unsigned int beginStake, unsigned int endStake)
	{
	//设计线和地面线的绘制
	int dsgCount = _road->m_DesignPoints.GetCount();
	m_DesignLine = new osg::Vec3Array();
	for (int i = 0; i < dsgCount; i++)
	{
		m_DesignLine->push_back(osg::Vec3(_road->m_DesignPoints[i].GetStake(), 0.0f, _road->m_DesignPoints[i].GetElevation()));
	}
	
	int grdCount = _road->m_GroundPoints.GetCount();
	m_GroundLine = new osg::Vec3Array();
	for (int i = 0; i < grdCount; i++)
	{
		m_GroundLine->push_back(osg::Vec3(_road->m_GroundPoints[i].GetStake(), 0.0f, _road->m_GroundPoints[i].GetElevation()));
}

	osg::ref_ptr<osg::Vec4Array> m_DesignColor = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> m_GroundColor = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> m_BaseColor = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> m_WaterColor = new osg::Vec4Array;
	m_GroundColor->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_DesignColor->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_BaseColor->push_back(osg::Vec4(0.4f, 0.3f, 0.4f, 0.5f));
	m_WaterColor->push_back(osg::Vec4(0.2f,0.4f,0.6f,0.6f));
	if (_root == NULL)
{
		_root = new osg::Group();
		_root->setName("CRoadData");
	}
	else
	{
		unsigned int count = _root->getNumChildren();
		_root->removeChildren(0, count);
	}
	
	//左右地面加入场景树
	Draw3DGroundR(_road->m_RightSection, beginStake, endStake);
	Draw3DGroundL(_road->m_LeftSection, beginStake, endStake);

	//路基加入场景树
	_root->addChild(cOSG::CreateContinuousTriangles(BottomVertices(beginStake, endStake), m_BaseColor));
	_root->addChild(cOSG::CreateContinuousTriangles(RightVertices(beginStake, endStake), m_BaseColor));
	_root->addChild(cOSG::CreateContinuousTriangles(LeftVertices(beginStake, endStake), m_BaseColor));
	_root->addChild(cOSG::CreateContinuousTriangles(NearVertices(beginStake), m_BaseColor));
	_root->addChild(cOSG::CreateContinuousTriangles(FarVertices(endStake), m_BaseColor));
	if (_road->m_WaterLevel != 0.0f)
	{
		_root->addChild(cOSG::CreateContinuousTriangles(CreateWaterLevel(beginStake, endStake, _road->m_WaterLevel), m_WaterColor));
		_root->addChild(cOSG::CreateContinuousTriangles(CreateWaterRight(beginStake, endStake, _road->m_WaterLevel), m_WaterColor));
		_root->addChild(cOSG::CreateContinuousTriangles(CreateWaterLeft(beginStake, endStake, _road->m_WaterLevel), m_WaterColor));
		_root->addChild(cOSG::CreateContinuousTriangles(CreateWaterFont(beginStake, _road->m_WaterLevel), m_WaterColor));
		_root->addChild(cOSG::CreateContinuousTriangles(CreateWaterBack(endStake, _road->m_WaterLevel), m_WaterColor));
}

// 	//截断设计线，并将截断后的设计线加入场景树
// 	_root->addChild(cOSG::CreateContinuousLine(m_DesignLine, m_DesignColor));

	//截断地面线，并将截断后的地面线加入场景树
	osg::ref_ptr<osg::Vec3Array>mGRand = new osg::Vec3Array;
	for (unsigned int stake = beginStake; stake <= endStake; stake++)
	{
		mGRand->push_back(m_GroundLine->at(stake - _road->m_LeftBoundry));
	}
	_root->addChild(cOSG::CreateContinuousLine(mGRand, m_GroundColor));

	/*osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));*/
	_root->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	_root->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	osg::StateSet* stateset = _root->getOrCreateStateSet();
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	stateset->setRenderBinDetails(10, "RenderBin");
}

//绘制右侧三角网
void CRoadDataCallback::Draw3DGroundR(vector<osg::ref_ptr<osg::Vec3Array>> &Section, unsigned int beginStake, unsigned int endStake)
{
	if (beginStake > endStake)
	{
		return;
	}
	std::vector<Triangle>* RTriangles = new std::vector<Triangle>;
	Triangle PreTriangle;
	osg::Vec3 mFirst, mSecond, mThird;
	for (unsigned int stake_index = beginStake; stake_index < endStake; ++stake_index)
	{
		//初始化第一个三角形的前两个顶点
		mFirst = _road->m_Profile->at(stake_index + 1);
		mSecond = _road->m_Profile->at(stake_index);

		//遍历中桩的右横断点
		unsigned int i = 0;//相对的第一个桩号的索引
		unsigned int j = 0;//相对的第二个桩号的索引
		double radian1;
		double radian2;
		for (; (i < Section[stake_index]->size()) || (j < Section[stake_index + 1]->size());)
		{
			
			//判断第三个点的取值
			if (i != Section[stake_index]->size())
				radian1 = getCosV(mFirst, mSecond, Section[stake_index]->at(i));
			else
				radian1 = 0;
			if (j != Section[stake_index + 1]->size())
				radian2 = getCosV(mFirst, mSecond, Section[stake_index + 1]->at(j));
			else
				radian2 = 0;
			if (radian1 > radian2)
			{
				mThird = Section[stake_index]->at(i);
				//组织成一个三角形
				PreTriangle.p1 = mFirst;
				PreTriangle.p2 = mSecond;
				PreTriangle.p3 = mThird;
				if (i < Section[stake_index]->size())
					i++;
				//初始化下一个三角形
				mFirst = PreTriangle.p1;
				mSecond = PreTriangle.p3;
			}
			else
			{
				mThird = Section[stake_index + 1]->at(j);
				//组织成一个三角形
				PreTriangle.p1 = mFirst;
				PreTriangle.p2 = mSecond;
				PreTriangle.p3 = mThird;
				if (j < Section[stake_index + 1]->size())
					j++;
				//初始化下一个三角形
				mFirst = PreTriangle.p3;
				mSecond = PreTriangle.p2;
			}
			//压入三角形数组
			RTriangles->push_back(PreTriangle);
		}
	}

	//绘制右边地面
	m_RGroundPoints = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> Rnormals = new osg::Vec3Array;
	for (unsigned int i = 0; i < RTriangles->size(); ++i)
	{
		m_RGroundPoints->push_back(RTriangles->at(i).p1);
		m_RGroundPoints->push_back(RTriangles->at(i).p2);
		m_RGroundPoints->push_back(RTriangles->at(i).p3);
		osg::Vec3 tmp = cOSG::GetTriangleNormal(RTriangles->at(i).p1, RTriangles->at(i).p2, RTriangles->at(i).p3);
		Rnormals->push_back(tmp);
	}
	//加入场景节点
	_root->addChild(cOSG::CreateTriangularMesh(m_RGroundPoints, Rnormals));

	//循环遍历右边最小的Z值
		//先初始化
	m_RMinZ = m_RGroundPoints->at(0).z();
	for (int i = 0; i < (int)(m_RGroundPoints->size()) - 1; ++i)
	{
		if (m_RGroundPoints->at(i).z() <= m_RGroundPoints->at(i + 1).z())
		{
			if (m_RMinZ > m_RGroundPoints->at(i).z())
				m_RMinZ = m_RGroundPoints->at(i).z();
		}
		else
		{
			if (m_RMinZ > m_RGroundPoints->at(i + 1).z())
				m_RMinZ = m_RGroundPoints->at(i + 1).z();
		}
	}
	delete RTriangles;
}

//绘制左侧三角网
void CRoadDataCallback::Draw3DGroundL(vector<osg::ref_ptr<osg::Vec3Array>> &Section, unsigned int beginStake, unsigned int endStake)
{
	if (beginStake > endStake)
	{
		return;
	}
	std::vector<Triangle>* LTriangles = new std::vector<Triangle>;
	Triangle PreTriangle;
	osg::Vec3 mFirst, mSecond, mThird;
	for (unsigned int stake_index = beginStake; stake_index < endStake; ++stake_index)
	{
		//初始化第一个三角形的前两个顶点
		mFirst = _road->m_Profile->at(stake_index);
		mSecond = _road->m_Profile->at(stake_index + 1);

		//遍历中桩的右横断点
		unsigned int i = 0;//相对的第一个桩号的索引
		unsigned int j = 0;//相对的第二个桩号的索引
		double radian1;
		double radian2;
		for (; (i < Section[stake_index]->size()) || (j < Section[stake_index + 1]->size());)
		{
			//判断第三个点的取值
			if (i != Section[stake_index]->size())
				radian1 = getCosV(mFirst, mSecond, Section[stake_index]->at(i));
			else
				radian1 = 0;
			if (j != Section[stake_index + 1]->size())
				radian2 = getCosV(mFirst, mSecond, Section[stake_index + 1]->at(j));
			else
				radian2 = 0;
			if (radian1 > radian2)
			{
				mThird = Section[stake_index]->at(i);
				//组织成一个三角形
				PreTriangle.p1 = mFirst;
				PreTriangle.p2 = mSecond;
				PreTriangle.p3 = mThird;
				if (i < Section[stake_index]->size())
					i++;
				//初始化下一个三角形的初始边
				mFirst = PreTriangle.p3;
				mSecond = PreTriangle.p2;
			}
			else
			{
				mThird = Section[stake_index + 1]->at(j);
				//组织成一个三角形
				PreTriangle.p1 = mFirst;
				PreTriangle.p2 = mSecond;
				PreTriangle.p3 = mThird;
				if (j < Section[stake_index + 1]->size())
					j++;
				//初始化下一个三角形的初始边
				mFirst = PreTriangle.p1;
				mSecond = PreTriangle.p3;
			}
			//压入三角形数组
			LTriangles->push_back(PreTriangle);
		}
	}

	//绘制左边地面
	m_LGroundPoints = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> Lnormals = new osg::Vec3Array;
	for (unsigned int i = 0; i < LTriangles->size(); ++i)
	{
		m_LGroundPoints->push_back(LTriangles->at(i).p1);
		m_LGroundPoints->push_back(LTriangles->at(i).p2);
		m_LGroundPoints->push_back(LTriangles->at(i).p3);

		Lnormals->push_back(cOSG::GetTriangleNormal(LTriangles->at(i).p1, LTriangles->at(i).p2, LTriangles->at(i).p3));
	}
	//左侧地面加入场景树
	_root->addChild(cOSG::CreateTriangularMesh(m_LGroundPoints, Lnormals));

	//循环遍历左边最小的Z值
		//先初始化
	m_LMinZ = m_LGroundPoints->at(0).z();
	for (int i = 0; i < (int)(m_LGroundPoints->size()) - 1; ++i)
	{
		if (m_LGroundPoints->at(i).z() <= m_LGroundPoints->at(i + 1).z())
		{
			if (m_LMinZ > m_LGroundPoints->at(i).z())
				m_LMinZ = m_LGroundPoints->at(i).z();
		}
		else
		{
			if (m_LMinZ > m_LGroundPoints->at(i + 1).z())
				m_LMinZ = m_LGroundPoints->at(i + 1).z();
		}
	}
	delete LTriangles;
}

double CRoadDataCallback::getCosV(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &p3) const
{
	double a = (p1 - p2).length();
	double b = (p1 - p3).length();
	double c = (p2 - p3).length();

	double alpha = (b*b + c*c - a*a) / (2 * b*c);
	return acosf(alpha);
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::BottomVertices(unsigned int beginStake, unsigned int endStake)
{
	if (endStake < beginStake)
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> BottomPoints = new osg::Vec3Array;
	for (; beginStake <= endStake; ++beginStake)
	{
		//取每个中桩的左边的最后一个空间点
		int indexL = (int)(_road->m_LeftSection.at(beginStake)->size()) - 1;
		osg::Vec3 tmpL(_road->m_LeftSection.at(beginStake)->at(indexL));
		tmpL[2] = BZ;	//保证Z轴齐平
		//将最后一个点保存到顶点数组
		BottomPoints->push_back(tmpL);

		//取每个中桩的右边的最后一个空间点
		int indexR = (int)(_road->m_RightSection.at(beginStake)->size()) - 1;
		osg::Vec3 tmpR(_road->m_RightSection.at(beginStake)->at(indexR));
		tmpR[2] = BZ;	//保证Z轴齐平
		//将最后一个点保存到顶点数组
		BottomPoints->push_back(tmpR);
	}
	return BottomPoints;
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::RightVertices(unsigned int beginStake, unsigned int endStake)
{
	if (endStake < beginStake)
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> RightPoints = new osg::Vec3Array;
	for (; beginStake <= endStake; ++beginStake)
	{
		//取每个中桩的右边的最后一个空间点
		int indexR = (int)(_road->m_RightSection.at(beginStake)->size()) - 1;
		osg::Vec3 tmpR(_road->m_RightSection.at(beginStake)->at(indexR));
		RightPoints->push_back(tmpR);
		tmpR[2] = BZ;	//保证Z轴跟底部齐平
		RightPoints->push_back(tmpR);
	}
	return RightPoints.get();
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::LeftVertices(unsigned int beginStake, unsigned int endStake)
{
	if (endStake < beginStake)
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> LeftPoints = new osg::Vec3Array;
	for (; beginStake <= endStake; ++beginStake)
	{
		//取每个中桩的左边的最后一个空间点
		int indexL = (int)(_road->m_LeftSection.at(beginStake)->size()) - 1;
		osg::Vec3 tmpL(_road->m_LeftSection.at(beginStake)->at(indexL));
		LeftPoints->push_back(tmpL);
		tmpL[2] = BZ;	//保证Z轴跟底部齐平
		LeftPoints->push_back(tmpL);
	}
	return LeftPoints.get();
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::NearVertices(unsigned int beginStake)
{
	if (beginStake < 0)
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> FrontPoints = new osg::Vec3Array;	

	//右侧点集
	for (unsigned int index = 0; index < _road->m_RightSection[beginStake]->size(); index += 3)
	{
		//上面点
		FrontPoints->push_back(_road->m_RightSection[beginStake]->at(index));

		//下面点
		_road->m_RightSection[beginStake]->at(index)[2] = BZ;
		FrontPoints->push_back(_road->m_RightSection[beginStake]->at(index));
	}

	//左侧点集
	for (unsigned int index = 0; index < _road->m_LeftSection[beginStake]->size(); index += 3)
	{
		//上面点
		FrontPoints->push_back(_road->m_LeftSection[beginStake]->at(index));

		//下面点
		_road->m_LeftSection[beginStake]->at(index)[2] = BZ;
		FrontPoints->push_back(_road->m_LeftSection[beginStake]->at(index));
	}
	return FrontPoints.get();
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::FarVertices(unsigned int endStack)
{
	if (endStack > _road->m_LeftSection.size())
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> FarPoints = new osg::Vec3Array;
	//左侧点集
	for (int index = (int)(_road->m_LeftSection[endStack]->size()) - 1; index >= 0; --index)
	{
		FarPoints->push_back(_road->m_LeftSection[endStack]->at(index));
		_road->m_LeftSection[endStack]->at(index)[2] = BZ;
		FarPoints->push_back(_road->m_LeftSection[endStack]->at(index));
	}

	//右侧点集
	for (unsigned int index = 0; index < _road->m_RightSection[endStack]->size(); ++index)
	{
		FarPoints->push_back(_road->m_RightSection[endStack]->at(index));
		_road->m_RightSection[endStack]->at(index)[2] = BZ;
		FarPoints->push_back(_road->m_RightSection[endStack]->at(index));
	}

	return FarPoints.get();
}

//创建淹水区水面点集
osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::CreateWaterLevel(unsigned int beginStake, unsigned int endStake, double waterLevel)
{
	if (endStake < beginStake)
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> BottomPoints = new osg::Vec3Array;
	for (; beginStake <= endStake; ++beginStake)
	{
		//取每个中桩的左边的最后一个空间点
		int indexL = (int)(_road->m_LeftSection.at(beginStake)->size()) - 1;
		osg::Vec3 tmpL(_road->m_LeftSection.at(beginStake)->at(indexL));
		tmpL[2] = waterLevel;	//保证Z轴齐平
		//将最后一个点保存到顶点数组
		BottomPoints->push_back(tmpL);

		//取每个中桩的右边的最后一个空间点
		int indexR = (int)(_road->m_RightSection.at(beginStake)->size()) - 1;
		osg::Vec3 tmpR(_road->m_RightSection.at(beginStake)->at(indexR));
		tmpR[2] = waterLevel;	//保证Z轴齐平
		//将最后一个点保存到顶点数组
		BottomPoints->push_back(tmpR);
	}
	return BottomPoints.get();
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::CreateWaterLeft(unsigned int beginStake, unsigned int endStake, double waterLevel)
{
	if (endStake < beginStake)
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> LeftPoints = new osg::Vec3Array;
	for (; beginStake <= endStake; ++beginStake)
	{
		//取每个中桩的左边的最后一个空间点
		int indexL = (int)(_road->m_LeftSection.at(beginStake)->size()) - 1;
		osg::Vec3 tmpL(_road->m_LeftSection.at(beginStake)->at(indexL));
		LeftPoints->push_back(tmpL);
		tmpL[1] -= 0.5f;
		tmpL[2] = waterLevel;	//保证Z轴跟底部齐平
		LeftPoints->push_back(tmpL);
	}
	return LeftPoints.get();
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::CreateWaterRight(unsigned int beginStake, unsigned int endStake, double waterLevel)
{
	if (endStake < beginStake)
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> RightPoints = new osg::Vec3Array;
	for (; beginStake <= endStake; ++beginStake)
	{
		//取每个中桩的右边的最后一个空间点
		int indexR = (int)(_road->m_RightSection.at(beginStake)->size()) - 1;
		osg::Vec3 tmpR(_road->m_RightSection.at(beginStake)->at(indexR));
		RightPoints->push_back(tmpR);
		tmpR[1] += 0.5;
		tmpR[2] = waterLevel;	//保证Z轴跟底部齐平
		RightPoints->push_back(tmpR);
	}
	return RightPoints.get();
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::CreateWaterFont(unsigned int beginStake, double waterLevel)
{
	if (beginStake < 0)
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> FrontPoints = new osg::Vec3Array;

	//右侧点集
	for (unsigned int index = 0; index < _road->m_RightSection[beginStake]->size(); index += 3)
	{
		//上面点
		FrontPoints->push_back(_road->m_RightSection[beginStake]->at(index));

		//下面点(反着的)
		_road->m_RightSection[beginStake]->at(index)[0] += 0.5f;
		_road->m_RightSection[beginStake]->at(index)[2] = waterLevel;
		FrontPoints->push_back(_road->m_RightSection[beginStake]->at(index));
	}

	//左侧点集
	for (unsigned int index = 0; index < _road->m_LeftSection[beginStake]->size(); index += 3)
	{
		//上面点
		FrontPoints->push_back(_road->m_LeftSection[beginStake]->at(index));

		//下面点
		_road->m_LeftSection[beginStake]->at(index)[0] += 0.5f;
		_road->m_LeftSection[beginStake]->at(index)[2] = waterLevel;
		FrontPoints->push_back(_road->m_LeftSection[beginStake]->at(index));
	}
	return FrontPoints.get();
}

osg::ref_ptr<osg::Vec3Array> CRoadDataCallback::CreateWaterBack(unsigned int endStake, double waterLevel)
{
	if (endStake > _road->m_LeftSection.size())
	{
		return NULL;
	}
	osg::ref_ptr<osg::Vec3Array> FarPoints = new osg::Vec3Array;
	//左侧点集
	for (int index = (int)(_road->m_LeftSection[endStake]->size()) - 1; index >= 0; --index)
	{
		FarPoints->push_back(_road->m_LeftSection[endStake]->at(index));

		_road->m_LeftSection[endStake]->at(index)[0] -= 0.5f;
		_road->m_LeftSection[endStake]->at(index)[2] = waterLevel;
		FarPoints->push_back(_road->m_LeftSection[endStake]->at(index));
	}

	//右侧点集
	for (unsigned int index = 0; index < _road->m_RightSection[endStake]->size(); ++index)
	{
		FarPoints->push_back(_road->m_RightSection[endStake]->at(index));

		_road->m_RightSection[endStake]->at(index)[0] -= 0.5f;
		_road->m_RightSection[endStake]->at(index)[2] = waterLevel;
		FarPoints->push_back(_road->m_RightSection[endStake]->at(index));
	}

	return FarPoints.get();
}


