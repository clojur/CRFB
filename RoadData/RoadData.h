#pragma once

#include <vector>
#include <YjOSG/YjOSG.h>
#include "macro.h"

using namespace std;

//�����νṹ
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

// �������ϵĵ�
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
	double m_Stake;			// ׮��
	double m_Elevation;		// �߳�
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
	double m_Stake;		// ���µ�׮��
	double m_Elevation;	// ���µ�߳�
	double m_Radius;	// ���µ�뾶
	double t;			// ���߳���
	double b_sta;		// ���µ�������
	double e_sta;		// ���µ��յ����
	bool   ctype;		// �������ͣ���/͹����
};

// CRoadData ����Ŀ��
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
	bool LoadCrossSection(CString fileName);	//���ص�������
	bool GetDesignElevation(double stake, double &height);
	bool GetGroundElevation(double stake, double &height);
	virtual void Serialize(CArchive& ar);
	bool GetTwHeight(double stake, double &height);			//���ڸ߶�
	bool CalcTwBoundry(double baseStake, double gap, double &left, double &right);
	vector<double> GetStakeByTwHeight(double height);

	CRoadDataCallback* getUpdateCallback() { return _callback; }
	osg::Group* getRoot() const { return _root; }


private:
	void InitDesignLine();

public:
	CArray<CGroundPoint, CGroundPoint&> m_GroundPoints;		//������
	CArray<CDesignPoint, CDesignPoint&> m_DesignPoints;		//�����
	osg::ref_ptr<osg::Vec3Array> m_Profile;		//��������������
	vector<osg::ref_ptr<osg::Vec3Array> > m_LeftSection;
	vector<osg::ref_ptr<osg::Vec3Array> > m_RightSection;

	int m_LeftBoundry;
	int m_RightBoundry;
	CString m_GroundFile;
	CString m_DesignFile;
	CString m_CrossFile;
	double m_WaterLevel;

	//���ڱ߽�
	double m_TwBoundryLeft, m_TwBoundryRight;//��ʼ���ڽ����

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
	//ͨ�������㼯�����Ҳ����
	void Draw3DGroundR(vector<osg::ref_ptr<osg::Vec3Array>> &Section, unsigned int beginStake, unsigned int endStake);
	//ͨ�������㼯����������
	void Draw3DGroundL(vector<osg::ref_ptr<osg::Vec3Array>> &Section, unsigned int beginStake, unsigned int endStake);

	//ͨ�������㼯���ɵײ���������
	osg::ref_ptr<osg::Vec3Array> BottomVertices(unsigned int beginStake, unsigned int endStake);
	//ͨ�������㼯�����Ҳඥ������
	osg::ref_ptr<osg::Vec3Array> RightVertices(unsigned int beginStake, unsigned int endStake);
	//ͨ�������㼯������ඥ������
	osg::ref_ptr<osg::Vec3Array> LeftVertices(unsigned int beginStake, unsigned int endStake);
	//ͨ�������㼯���ɽ��涥������
	osg::ref_ptr<osg::Vec3Array> NearVertices(unsigned int beginStake);
	//ͨ�������㼯����Զ�涥������
	osg::ref_ptr<osg::Vec3Array> FarVertices(unsigned int endStake);

	//������ˮ��ˮ��㼯
	osg::ref_ptr<osg::Vec3Array> CreateWaterLevel(unsigned int beginStake, unsigned int endStake, double waterLevel);
	//������ˮ������㼯
	osg::ref_ptr<osg::Vec3Array> CreateWaterLeft(unsigned int beginStake, unsigned int endStake, double waterLevel);
	//������ˮ������㼯
	osg::ref_ptr<osg::Vec3Array> CreateWaterRight(unsigned int beginStake, unsigned int endStake, double waterLevel);
	//������ˮ��ǰ��㼯
	osg::ref_ptr<osg::Vec3Array> CreateWaterFont(unsigned int beginStake, double waterLevel);
	//������ˮ������㼯
	osg::ref_ptr<osg::Vec3Array> CreateWaterBack(unsigned int endStake, double waterLevel);
	//���л��Ƶ�������
	void DrawRoadData(unsigned int beginStake, unsigned int endStake);
	double getCosV(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &p3)const;

private:
	CRoadData* _road;

	osg::ref_ptr<osg::Vec3Array> m_DesignLine;// ����ߵ㼯
	osg::ref_ptr<osg::Vec3Array> m_GroundLine;// �����ߵ㼯
	osg::ref_ptr<osg::Vec3Array> m_LGroundPoints;//��������ȡ�㼯
	osg::ref_ptr<osg::Vec3Array> m_RGroundPoints;//�Ҳ������ȡ�㼯
	double m_RMinZ;
	double m_LMinZ;
};