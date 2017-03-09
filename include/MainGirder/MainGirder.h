
/* ######################################################################################

                                  ����: ����

                                  ����: ����������������

                                  �������¼�����:

                                  �ڶΡ����ա�����


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
// 	CString pileGroupDescribe;//׮������
// 	CString	pileFoundationType;//׮������
// 	CString	connectMethod;//���ӷ�ʽ
// 	double	pileFoundationDiameter;//׮��ֱ��
// 	double	primaryRebarDiameter;//����ֱ��
// 	double	primaryRebarCount;//�������
// 	double	pileLength;//׮��
// 	double	pileCount;//׮��
// 	double	hoopRebarDiameter;//����ֱ��
// 	double	erectionRebarDiameter;//������ֱ��
// 	CString	concreteMaterial;//����������
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
	
	//���ܻ��ֽڶ�(���ط����ַ�����)
	vector<CString>& AutoCellsplit();
	//����ָ���Ļ����ַ������ɽڶμ���
	void CreateSegmentsByStr(CString);
	/*���ݽڶα�Ų��ҽڶζ���*/
	CSegment* GetSegmentByID(int m_Id);
	//���½ڶμ��ϵľ������...
	void UpdateCell();
	//���ݳ�����װ��
	double getTB_atX(double);
	//���ݳ���������
	double getH_AtX(double);
	void SetRoadData(CRoadData *road) { m_Road = road; }
	void CalcOutline();
	void CreatePier();
	void SetSection(CStdSection *section) { m_Section = section; }
	CMainGirderCallback* getUpdateCallback() { return _callback; }
	osg::Group* getRoot() const { return _root; }

private:
	//���ɵ��ڶ���������ĵ�Ԫ
	void CMainGirder::CreateStdSegment();
	double getE_atX(double x);
	// ��������ȡ�ָ�����ߵ��ַ���
	CString LSplit(CString str, TCHAR chr);
	// ��������ȡ�ָ����ұߵ��ַ���
	CString RSplit(CString str, TCHAR chr);
	//�����ڶλ����ַ�����ֵ�������۵ĳ�������
	void CMainGirder::ResolveStrToCell(CString str);
	osg::ref_ptr<osg::Geometry> CreatePrismoid(osg::ref_ptr<osg::Vec3Array> vertices);
	void GetModelAttrib();

public:
	enum  GirderType
	{
		SIDE_SPAN,			//�߿��ֽ���
		SIDE_CLOSURE,		//�߿��£��
		MID_CLOSURE,		//�п��£��
		T_CONSTRUCTION		//T��
	};

	CString m_Id;           //���    
	int m_Type;             //�������ͣ�T����£�Ρ��߿��
	double m_Left;
	double m_Right;
	double m_Center;        //������� 
	double m_CurveLen;
	double m_AllLen;        //����
	double m_BottomLen;     //����ֱ�߶γ�
	double m_Remain;        //���۸�����ʣ�೤�ȣ�����ֱ�߶αȶ�˳����ߴ����ĳ��ȣ�
	double m_HeightRatio;   //���������߱仯��
	double m_ThickRatio;    //���׺�ȱ仯��
	double m_TolesHeight;   //���˲���
	double m_RootHeight;    //��������
	double m_TolesThick;    //���˲��װ��
	double m_RootThick;     //�������װ��
	CString m_ResSplit;     //�������ֽ��(�ַ���)

	vector<CString> resSplit;//�������ֽ����

	vector<CSegment*> segments;
	CMainPier* m_MainPier;  //���ն���
	CRoadData* m_Road;

	/*Ĭ�ϲ���*/
	CDefaultParam m_DefaultParam;

private:
	vector<double> cell;    //�����۵ĳ�������
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
