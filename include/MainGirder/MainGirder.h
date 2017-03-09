
/* ######################################################################################

                                  类名: 主梁

                                  描述: 用来构建主梁对象

                                  包含的下级对象:

                                  节段、主墩、钢束


########################################################################################*/

#pragma once
#include <Segment/Segment.h>
#include <StdSection/StdSection.h>
#include <MainPier/MainPier.h>
#include <RoadData/RoadData.h>
#include <YjOSG/YjOSG.h>
#include <YJmath/YjMath.h>
#include <DefaultParam/DefaultParam.h>
#include "macro.h"
#include <vector>

using namespace std;
// class CRigidFrameBridge;

// typedef struct PILE_GROUP_INFO{
// 	DECLARE_SERIAL(PILE_GROUP_INFO)
// 	CString pileGroupDescribe;//桩组描述
// 	CString	pileFoundationType;//桩基类型
// 	CString	connectMethod;//连接方式
// 	double	pileFoundationDiameter;//桩基直径
// 	double	primaryRebarDiameter;//主筋直径
// 	double	primaryRebarCount;//主筋根数
// 	double	pileLength;//桩长
// 	double	pileCount;//桩数
// 	double	hoopRebarDiameter;//箍筋直径
// 	double	erectionRebarDiameter;//架立筋直径
// 	CString	concreteMaterial;//混凝土材料
// }PGI;

class CMainGirderCallback;

class MAINGIRDER_API CMainGirder :public CPickObject
{
	friend class CMainGirderCallback;
	DECLARE_SERIAL(CMainGirder)
public:
	CMainGirder();
	CMainGirder(int type, CString id, double start, double length, double height, double thick);
	CMainGirder(int type, CString id, double start, double length, double bottomLen, double remainLen, double rootHeight, double tolesHeight, double rootThick, double tolesThick, double heightRatio, double thickRatio);
	~CMainGirder();
	void Serialize(CArchive& ar);
	
	//智能划分节段(返回方案字符数组)
	vector<CString>& AutoCellsplit();
	//根据指定的划分字符串生成节段集合
	void CreateSegmentsByStr(CString);
	/*根据节段编号查找节段对象*/
	CSegment* GetSegmentByID(int m_Id);
	//更新节段集合的具体参数...
	void UpdateCell();
	//根据长度求底板厚
	double getTB_atX(double);
	//根据长度求截面高
	double getH_AtX(double);
	void SetRoadData(CRoadData *road) { m_Road = road; }
	void CalcOutline();
	void CreatePier();
	void SetSection(CStdSection *section) { m_Section = section; }
	CMainGirderCallback* getUpdateCallback() { return _callback; }
	osg::Group* getRoot() const { return _root; }

private:
	//生成单节段主梁对象的单元
	void CMainGirder::CreateStdSegment();
	double getE_atX(double x);
	// 本函数获取分隔符左边的字符串
	CString LSplit(CString str, TCHAR chr);
	// 本函数获取分隔符右边的字符串
	CString RSplit(CString str, TCHAR chr);
	//解析节段划分字符串赋值给单悬臂的长度数组
	void CMainGirder::ResolveStrToCell(CString str);
	osg::ref_ptr<osg::Geometry> CreatePrismoid(osg::ref_ptr<osg::Vec3Array> vertices);
	void GetModelAttrib();

public:
	enum  GirderType
	{
		SIDE_SPAN,			//边跨现浇段
		SIDE_CLOSURE,		//边跨合拢段
		MID_CLOSURE,		//中跨合拢段
		T_CONSTRUCTION		//T构
	};

	CString m_Id;           //编号    
	int m_Type;             //主梁类型，T、合拢段、边跨段
	double m_Left;
	double m_Right;
	double m_Center;        //中心里程 
	double m_CurveLen;
	double m_AllLen;        //梁长
	double m_BottomLen;     //梁底直线段长
	double m_Remain;        //悬臂根部的剩余长度（梁底直线段比墩顺桥向尺寸多出的长度）
	double m_HeightRatio;   //梁底抛物线变化率
	double m_ThickRatio;    //梁底厚度变化率
	double m_TolesHeight;   //梁端部高
	double m_RootHeight;    //梁根部高
	double m_TolesThick;    //梁端部底板厚
	double m_RootThick;     //梁根部底板厚
	CString m_ResSplit;     //主梁划分结果(字符串)

	vector<CString> resSplit;//主梁划分结果集

	vector<CSegment*> segments;
	CMainPier* m_MainPier;  //主墩对象
	CRoadData* m_Road;

	/*默认参数*/
	CDefaultParam m_DefaultParam;

private:
	vector<double> cell;    //单悬臂的长度数组
	CStdSection *m_Section;
	osg::ref_ptr<osg::Group> m_Root;
	vector<CYNPoint2D> m_BeamTop;
	vector<CYNPoint2D> m_BeamBottom;

	osg::ref_ptr<osg::Group> _root;
	CMainGirderCallback* _callback;
};


class CMainGirderCallback : public ComponentCallback
{
public:
	CMainGirderCallback(CMainGirder* girder)
		: ComponentCallback(girder->getRoot())
	{
		_girder = girder;
		_root->setName("CMainGirder");
	}
	void DoUpdate() override;

private:
	CMainGirder* _girder;
};
