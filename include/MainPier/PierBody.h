/* ######################################################################################

                                 ����: ����

                                 ����: ���������������,�����˳������Ϣ

                                 �������¼�����: ��


########################################################################################*/

#pragma once
#include <YjOSG/YjOSG.h>
#include "macro.h"


class CPierBodyCallback;

class MAINPIER_API CPierBody : public CPickObject
{
	friend class CPierBodyCallback;
	DECLARE_SERIAL(CPierBody)
private:
	double m_LWidth;    //˳������
	double m_CWidth;    //��������
	double m_Height;    //�ո�
	double m_WallThick; //�ձں�

	std::vector<CStringPair> *m_InfoPair;
	double m_Left;
	double m_Right;
	double m_Front;
	double m_Back;
	double m_LeftTop;
	double m_RightTop;
	double m_Bottom;
	osg::ref_ptr<osg::Group> _root;
	CPierBodyCallback* _callback;

public:
	CPierBody();
	CPierBody(double left, double right, double front, double back, double leftTop, double rightTop, double bottom);
	~CPierBody();
	void Serialize(CArchive& ar);

	double getConcreteVolume();
	osg::Group* getRoot() const { return _root; }
	CPierBodyCallback* getUpdateCallback() const { return _callback; }
	void GetModelAttrib();
};


class CPierBodyCallback : public ComponentCallback
{
public:
	CPierBodyCallback(CPierBody* pierbody)
		: _pierbody(pierbody)
		, ComponentCallback(pierbody->getRoot())
	{}

	void DoUpdate() override;

private:
	CPierBody* _pierbody;
};