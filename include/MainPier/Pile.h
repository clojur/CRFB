/* ######################################################################################

                                 ����: ׮����

                                 ����: ��������׮�������,������׮����׮����׮��������Ϣ

                                 �������¼�����: ��


########################################################################################*/

#pragma once
#include <YjOSG/YjOSG.h>

class CPileCallback;

class MAINPIER_API CPile : public CPickObject
{
	friend class CPileCallback;
	enum PileType
	{
		FRICTION_PILE,		//Ħ��׮
		END_BEARING_PILE,	//�˳�׮
	};

	DECLARE_SERIAL(CPile)
private:
	CString m_Type; //׮��
	double m_CenterX;
	double m_CenterY;
	double m_CenterZ;
	double m_Diameter;   //׮��
	double m_Length;//׮��
	std::vector<CStringPair> *m_InfoPair;
	osg::ref_ptr<osg::Group> _root;
	CPileCallback* _callback;

public:
	CPile();
	CPile(double cx, double cy, double cz, double diameter, double length);
	~CPile();
	void Serialize(CArchive& ar);

	double getConcreteVolume();
	osg::Group* getRoot() { return _root; }
	CPileCallback* getUpdateCallback() const { return _callback; }
	void GetModelAttrib();
};



class CPileCallback : public ComponentCallback
{
public:
	CPileCallback(CPile* pile)
		: _pile(pile)
		, ComponentCallback(pile->getRoot())
	{
	}

	void DoUpdate() override;

private:
	CPile* _pile;
};