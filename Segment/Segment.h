
/* ######################################################################################

                                 ����: �ڶ�

                                 ����: ���������ڶζ���

                                 �������¼�����: ��


########################################################################################*/

#pragma once

#include <YjMath/YjMath.h>
#include <StdSection/StdSection.h>
#include <RoadData/RoadData.h>
//#include "../RigidFrame/MainFrm.h"
#include "macro.h"


class CSegmentCallback;

class SEGMENT_API CSegment : public CPickObject
{
	friend class CSegmentCallback;
public:
	_DECLARE_DYNCREATE(CSegment)      \
	AFX_EXT_API friend CArchive& AFXAPI \
	operator>>(CArchive& ar, CSegment* &pOb);

	CSegment();
	~CSegment();

	CYjMath Yjmath;         //��ѧ��

	int m_Id;           //�ڶα��
	bool	m_Selected;     //�ڶ��Ƿ�ѡ��
	double  m_LeftStake;
	double	m_RightStake;
	double	m_Length;       //����
	double  m_AddLength;    //�ۼƳ���
	double	m_LlHeight;     //�����������
	double	m_LrHeight;     //�����������
	double	m_RlHeight;     //�ҽ���������
	double	m_RrHeight;     //�ҽ���������
	double	m_LplateThick;  //��װ��
	double	m_RplateThick;  //�ҵװ��
	double	m_LwebThick;    //�󸹰��
	double	m_RwebThick;    //�Ҹ����
	double	m_LdesignElev;  //����Ƹ߳�
	double	m_RdesignElev;  //����Ƹ߳�
	double	m_LlwebElev;    //��������ϸ߳�
	double	m_LrwebElev;    //��������ϸ߳�
	double	m_RlwebElev;    //�ҽ������ϸ߳�
	double	m_RrwebElev;    //�ҽ������ϸ߳�
	double	m_LbottomElev1; //������¸߳�1
	double	m_LbottomElev2; //������¸߳�2
	double	m_RbottomElev1; //�ҽ����¸߳�1
	double	m_RbottomElev2; //�ҽ����¸߳�2
	double	m_Volume;       //���
	double	anti_crackRebar;//���Ѹֽ�

	POINT2dArray Slo_points; //����������㼯
	POINT2dArray Sli_points; //����������㼯
	POINT2dArray  Sro_points;//����������㼯
	POINT2dArray Sri_points; //����������㼯
	CStdSection crossSection;//�������

	void Serialize(CArchive& ar);//���л�
	double  GetVolume();//����ڶ����
	double	GetSlo();//��ȡ�����������
	double	GetSli();//��ȡ�����������
	double	GetSro();//��ȡ�����������
	double	GetSri();//��ȡ�����������
	void SetRoadData(CRoadData *road);
	void SetSection(CStdSection* sect);
	osg::Group* getRoot() const { return _root; }
	CSegmentCallback* getUpdateCallback() const { return _callback; }
	void GetModelAttrib();
private:
	osg::ref_ptr<osg::Group> _root;
	CSegmentCallback* _callback;
	CRoadData* m_Road;
	CStdSection* m_Section;
};



class SEGMENT_API CSegmentCallback : public ComponentCallback
{
public:
	CSegmentCallback(CSegment* segment)
		: ComponentCallback(segment->getRoot())
		, _segment(segment)
	{
	}

	void DoUpdate() override;

private:
	CClassGeode *UpdateScene(double leftStake, double leftElev, double leftHeight, double rightStake, double rightElev, double rightHeight);

private:
	CSegment* _segment;
};