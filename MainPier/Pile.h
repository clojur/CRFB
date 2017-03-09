/* ######################################################################################

                                 类名: 桩基组

                                 描述: 用来构建桩基组对象,包含了桩长、桩径、桩根数等信息

                                 包含的下级对象: 无


########################################################################################*/

#pragma once
#include <YjOSG/YjOSG.h>

class CPileCallback;

class MAINPIER_API CPile : public CPickObject
{
	friend class CPileCallback;
	enum PileType
	{
		FRICTION_PILE,		//摩擦桩
		END_BEARING_PILE,	//端承桩
	};

	DECLARE_SERIAL(CPile)
private:
	CString m_Type; //桩型
	double m_CenterX;
	double m_CenterY;
	double m_CenterZ;
	double m_Diameter;   //桩径
	double m_Length;//桩长
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