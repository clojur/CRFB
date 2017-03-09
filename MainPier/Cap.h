/* ######################################################################################

                                 ����: ��̨

                                 ����: ����������̨����,�����˳������Ϣ

                                 �������¼�����: ��


########################################################################################*/

#pragma once
#include <YjOSG/YjOSG.h>
//#include "../RigidFrame/MainFrm.h"
class CCapCallback;
class CClassGeode;
class MAINPIER_API CCap : public CPickObject
{
	friend class CCapCallback;
	DECLARE_SERIAL(CCap)
private:
	double m_LWidth;    //˳������
	double m_CWidth;    //��������
	double m_Thickness; //��̨���
	double m_CenterX;
	double m_CenterY;
	double m_CenterZ;
	std::vector<CStringPair> *m_InfoPair;
	osg::ref_ptr<osg::Group> _root;
	CCapCallback* _callback;

public:
	CCap();
	CCap(double cx, double cy, double cz, double length, double width, double depth);
	~CCap();
	void Serialize(CArchive& ar);

	double getConcreteVolume();
	osg::Group* getRoot() const { return _root; }
	CCapCallback* getUpdateCallback() const { return _callback; }
	osg::ref_ptr<osg::Group> Draw();

	double GetTop() const { return m_CenterZ + m_Thickness / 2; }
	double GetBottom() const { return m_CenterZ - m_Thickness / 2; }
	double GetLeft() const { return m_CenterX - m_LWidth / 2; }
	double GetRight() const { return m_CenterX + m_LWidth / 2; }
	double GetFront() const { return m_CenterY - m_CWidth / 2; }
	double GetBack() const { return m_CenterY + m_CWidth / 2; }
	void GetModelAttrib();
};


class CCapCallback : public ComponentCallback
{
public:
	CCapCallback(CCap* cap)
		: _cap(cap)
		, ComponentCallback(cap->getRoot())
	{
	}
	void DoUpdate() override;

private:
	CCap* _cap;
};

