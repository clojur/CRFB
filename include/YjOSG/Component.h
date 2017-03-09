#pragma once
#include <afx.h>
#include <osg/Group>
#include <osg/NodeCallback>
#include <osgViewer/View>

class YJOSG_API ComponentCallback : public osg::NodeCallback
{
public:
	ComponentCallback(osg::Group* root);
	~ComponentCallback();

public:
	osg::Group* getRoot() const
	{
		return _root.get();
	}

	void setView(osgViewer::View* view) { _view = view; }
	osgViewer::View* getView() const { return _view.get(); }

	void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
	virtual void DoUpdate() {}

public:
	void Update();
	void Lock();
	void Unlock();

protected:
	osg::observer_ptr<osg::Group> _root;
	osg::observer_ptr<osgViewer::View> _view;
	bool _changed;

private:
	OpenThreads::Mutex _mutex;
};
