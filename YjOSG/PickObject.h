#pragma once
#include "afx.h"
#include "common.h"
/*用于用户拾取信息的保存*/
struct CStringPair{
	CString key;
	CString value;
	CStringPair(){
		key = "";
		value = "";
	}
	CStringPair(CString k,CString v){
		key = k;
		value = v;
	}
};
class YJOSG_API CPickObject :public CObject
{
public:
	CPickObject();
	~CPickObject();
	virtual void GetModelAttrib()=0;
};

class YJOSG_API CClassGeode :public osg::Geode
{
public:
	CClassGeode(CPickObject* modelClass);
	~CClassGeode();
	void GetModelAttrib();
private:
	CPickObject* m_ModelClass;
};