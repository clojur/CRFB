#include "stdafx.h"
#include "PickObject.h"

/*CPickObject��*/
CPickObject::CPickObject()
{
}


CPickObject::~CPickObject()
{
}


/*CClassGeode��*/
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
