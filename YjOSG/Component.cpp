// Engineering objects\Composite.cpp : 实现文件
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

// 如果使用自旋锁，在UI线程与Viewer线程在同一个CPU时，会导致死锁
void ComponentCallback::Lock()
{
	_mutex.lock();
}

void ComponentCallback::Unlock()
{
	_mutex.unlock();
}
