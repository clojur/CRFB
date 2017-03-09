/* ######################################################################################

                                 ����: ����

                                 ����: �����������ն���,�����˳������Ϣ

                                 �������¼�����: ������̨��׮����


########################################################################################*/

#pragma once

#include <RoadData/RoadData.h>
#include "PierBody.h"
#include "Cap.h"
#include "Pile.h"
#include "macro.h"

class CMainPierCallback;
class CRoadData;

class MAINPIER_API CMainPier : public CObject
{
	friend class CMainPierCallback;
	DECLARE_SERIAL(CMainPier)
public:
	std::vector<CPierBody*> m_PierBodys;  //�������
	CCap* m_Cap;            //��̨����
	std::vector<CPile*> m_Piles;        //׮�������

	double m_Width;
	double m_Length;

	float  m_Lspace;//˳������
	float  m_Cspace;//��������

private:
	CRoadData *m_Road;
	double m_CenterX;			//���������ߵ�X����
	double m_CenterY;			//���������ߵ�Y����
	double m_RootHeight;		//0�ſ������
	double m_PierThickness;		//�ձں�
	double m_CapThickness;		//��̨���
	double m_CapPadding;		//��̨���
	double m_PileDiameter;		//׮��
	double m_PileLength;		//׮��
	osg::ref_ptr<osg::Group> _root;
	CMainPierCallback* _callback;

public:
	CMainPier();
	CMainPier(CRoadData *road, double cx, double cy, double bottomLen, double width);
	~CMainPier();
	void Serialize(CArchive& ar);

	double getConcreteVolume();

	void SetPierParam(double thickness, double rootHeight);
	void SetCapParam(double thickness, double padding);
	void SetPileParam(double diameter, double length);

	CMainPierCallback* getUpdateCallback() const { return _callback; }
	osg::Group* getRoot() const { return _root; }

	void Create();

	void CreatePierBody();
	void CreateCap();
	void CreatePills();
};



class CMainPierCallback : public ComponentCallback
{
public:
	CMainPierCallback(CMainPier* pier)
		: ComponentCallback(pier->getRoot())
	{
		_pier = pier;
	}

	void DoUpdate() override;
	
private:
	CMainPier* _pier;
};