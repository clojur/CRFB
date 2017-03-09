// Engineering objects\Composite.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Component.h"

ComponentCallback::ComponentCallback(osg::Group* root)
{
	_changed = true;
	_root = root;
	_root->setUpdateCallback(this);
}

ComponentCallback::~ComponentCallback()
{
}

void ComponentCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	_mutex.lock();
	if (_changed == true)
	{
		DoUpdate();
		_changed = false;
	}
	_mutex.unlock();

	traverse(node, nv);
}

void ComponentCallback::Update()
{
	_changed = true;
}

// ���ʹ������������UI�߳���Viewer�߳���ͬһ��CPUʱ���ᵼ������
void ComponentCallback::Lock()
{
	_mutex.lock();
}

void ComponentCallback::Unlock()
{
	_mutex.unlock();
}
