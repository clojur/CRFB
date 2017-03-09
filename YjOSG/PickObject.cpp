#include "stdafx.h"
#include "PickObject.h"

/*CPickObjectÀà*/
CPickObject::CPickObject()
{
}


CPickObject::~CPickObject()
{
}


/*CClassGeodeÀà*/
CClassGeode::CClassGeode(CPickObject* modelClass) :m_ModelClass(modelClass)
{

}

CClassGeode::~CClassGeode()
{

}

void CClassGeode::GetModelAttrib()
{
	m_ModelClass->GetModelAttrib();
}
