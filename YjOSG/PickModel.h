#pragma once
#include "common.h"
class YJOSG_API PickModel :public osg::Group
{
public:
	PickModel(osg::Node* shape);
	~PickModel(void);

	void EnableDragger(void);
	void DisableDragger(void);

private:
	osg::ref_ptr<osg::Node> mShape;
	osg::ref_ptr<osgManipulator::Dragger> mDragger;
	osg::ref_ptr<osgManipulator::Dragger>mRotateDragger;
	osg::ref_ptr<osgManipulator::Selection> mSelection;
};

