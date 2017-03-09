#pragma once

#include <vector>
#include <YjOSG/YjOSG.h>
#include "macro.h"

using namespace std;

//三角形结构
typedef struct Triangle
{
	osg::Vec3 p1;
	osg::Vec3 p2;
	osg::Vec3 p3;
	Triangle(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &p3)
	{
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	};
	Triangle()
	{
		this->p1 = osg::Vec3(0.0f, 0.0f, 0.0f);
		this->p2 = osg::Vec3(0.0f, 0.0f, 0.0f);;
		this->p3 = osg::Vec3(0.0f, 0.0f, 0.0f);;
	};
}*ptr_Triangle;

// 地面线上的点
class CGroundPoint : public CObject
{
public:
	DECLARE_SERIAL(CGroundPoint)
	CGroundPoint();
	CGroundPoint(double stake, double elevation);
	CGroundPoint(const CGroundPoint& ground);
	~CGroundPoint();

	CGroundPoint& operator = (const CGroundPoint& src);

	double GetStake() const
	{
		return m_Stake;
	}

	double GetElevation() const
	{
		return m_Elevation;
	}

private:
	double m_Stake;			// 桩号
	double m_Elevation;		// 高程
};

class CDesignPoint : public CObject
{
public:
	DECLARE_SERIAL(CDesignPoint)
	CDesignPoint();
	CDesignPoint(double stake, double elevation, double radius);
	~CDesignPoint();

	CDesignPoint& operator = (const CDesignPoint& src);
	double GetStake() const
	{
		return m_Stake;
	}

	double GetElevation() const
	{
		return m_Elevation;
	}

	double GetRadius() const
	{
		return m_Radius;
	}

	double GetBSta() const
	{
		return b_sta;
	}

	double GetESta() const
	{
		return e_sta;
	}

	bool GetCurveType() const
	{
		return ctype;
	}

	void SetParams(double t, double b_sta, double e_sta, bool ctype);

private:
	double m_Stake;		// 变坡点桩号
	double m_Elevation;	// 变坡点高程
	double m_Radius;	// 变坡点半径
	double t;			// 弧线长度
	double b_sta;		// 变坡点起点里程
	double e_sta;		// 变坡点终点里程
	bool   ctype;		// 曲线类型，凹/凸曲线
};

// CRoadData 命令目标
class CRoadDataCallback;

class ROADDATA_API CRoadData : public CObject
{
	friend CRoadDataCallback;
public:
	DECLARE_SERIAL(CRoadData)
	CRoadData();
	CRoadData(CRoadData& road);
	CRoadData& operator = (CRoadData& src);
	~CRoadData();
	bool LoadGroundLine(CString fileName);
	bool LoadDesignLine(CString fileName);
	bool LoadCrossSection(CString fileName);	//加载地面横断面
	bool GetDesignElevation(double stake, double &height);
	bool GetGroundElevation(double stake, double &height);
	virtual void Serialize(CArchive& ar);
	bool GetTwHeight(double stake, double &height);			//填挖高度
	bool CalcTwBoundry(double baseStake, double gap, double &left, double &right);
	vector<double> GetStakeByTwHeight(double height);

	CRoadDataCallback* getUpdateCallback() { return _callback; }
	osg::Group* getRoot() const { return _root; }


private:
	void InitDesignLine();

public:
	CArray<CGroundPoint, CGroundPoint&> m_GroundPoints;		//地面线
	CArray<CDesignPoint, CDesignPoint&> m_DesignPoints;		//设计线
	osg::ref_ptr<osg::Vec3Array> m_Profile;		//地面线纵面数据
	vector<osg::ref_ptr<osg::Vec3Array> > m_LeftSection;
	vector<osg::ref_ptr<osg::Vec3Array> > m_RightSection;

	int m_LeftBoundry;
	int m_RightBoundry;
	CString m_GroundFile;
	CString m_DesignFile;
	CString m_CrossFile;
	double m_WaterLevel;

	//填挖边界
	double m_TwBoundryLeft, m_TwBoundryRight;//起始填挖交界点

private:
	osg::ref_ptr<osg::Group> _root;
	CRoadDataCallback* _callback;
};

class ROADDATA_API CRoadDataCallback : public ComponentCallback
{
#define BZ ((m_RMinZ < m_LMinZ)? (m_RMinZ-20.0):(m_LMinZ-20.0))

public:
	CRoadDataCallback(CRoadData* road)
		: ComponentCallback(road->getRoot())
	{
		_road = road;
		_root->setName("CRoadData");
	}
	void DoUpdate() override;

private:
	//通过横截面点集绘制右侧地面
	void Draw3DGroundR(vector<osg::ref_ptr<osg::Vec3Array>> &Section, unsigned int beginStake, unsigned int endStake);
	//通过横截面点集绘制左侧地面
	void Draw3DGroundL(vector<osg::ref_ptr<osg::Vec3Array>> &Section, unsigned int beginStake, unsigned int endStake);

	//通过横截面点集生成底部顶点数组
	osg::ref_ptr<osg::Vec3Array> BottomVertices(unsigned int beginStake, unsigned int endStake);
	//通过横截面点集生成右侧顶点数组
	osg::ref_ptr<osg::Vec3Array> RightVertices(unsigned int beginStake, unsigned int endStake);
	//通过横截面点集生成左侧顶点数组
	osg::ref_ptr<osg::Vec3Array> LeftVertices(unsigned int beginStake, unsigned int endStake);
	//通过横截面点集生成近面顶点数组
	osg::ref_ptr<osg::Vec3Array> NearVertices(unsigned int beginStake);
	//通过横截面点集生成远面顶点数组
	osg::ref_ptr<osg::Vec3Array> FarVertices(unsigned int endStake);

	//创建淹水区水面点集
	osg::ref_ptr<osg::Vec3Array> CreateWaterLevel(unsigned int beginStake, unsigned int endStake, double waterLevel);
	//创建淹水区左面点集
	osg::ref_ptr<osg::Vec3Array> CreateWaterLeft(unsigned int beginStake, unsigned int endStake, double waterLevel);
	//创建淹水区右面点集
	osg::ref_ptr<osg::Vec3Array> CreateWaterRight(unsigned int beginStake, unsigned int endStake, double waterLevel);
	//创建淹水区前面点集
	osg::ref_ptr<osg::Vec3Array> CreateWaterFont(unsigned int beginStake, double waterLevel);
	//创建淹水区后面点集
	osg::ref_ptr<osg::Vec3Array> CreateWaterBack(unsigned int endStake, double waterLevel);
	//集中绘制地面数据
	void DrawRoadData(unsigned int beginStake, unsigned int endStake);
	double getCosV(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &p3)const;

private:
	CRoadData* _road;

	osg::ref_ptr<osg::Vec3Array> m_DesignLine;// 设计线点集
	osg::ref_ptr<osg::Vec3Array> m_GroundLine;// 地面线点集
	osg::ref_ptr<osg::Vec3Array> m_LGroundPoints;//左侧地面提取点集
	osg::ref_ptr<osg::Vec3Array> m_RGroundPoints;//右侧地面提取点集
	double m_RMinZ;
	double m_LMinZ;
};