#include "stdafx.h"
#include "PickModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


PickModel::PickModel(osg::Node* shape)
	: mShape(shape)
	, mDragger(new osgManipulator::TranslateAxisDragger())
	, mRotateDragger(new osgManipulator::TrackballDragger())
	, mSelection(new osgManipulator::Selection())
{
	float scale = mShape->getBound().radius() * 1.6;
	mDragger->setMatrix(osg::Matrix::scale(scale, scale, scale) *
		osg::Matrix::translate(mShape->getBound().center()));
	mDragger->setupDefaultGeometry();

	mRotateDragger->setMatrix(osg::Matrix::scale(scale / 3, scale / 3, scale / 3) *
		osg::Matrix::translate(mShape->getBound().center()));
	mRotateDragger->setupDefaultGeometry();

	mSelection->addChild(shape);
	addChild(mSelection);
}


PickModel::~PickModel(void)
{
}


void PickModel::EnableDragger()
{
	//mDragger->setMatrix(mDragger->getMatrix().scale(mDragger->getMatrix().getScale()));
	//mRotateDragger->setMatrix(mSelection->getMatrix().scale(mRotateDragger->getMatrix().getScale()));

	addChild(mDragger);
	mDragger->addTransformUpdating(mSelection);
	mDragger->setHandleEvents(true);


	//addChild(mRotateDragger);
	//mRotateDragger->addTransformUpdating(mSelection);
	//mRotateDragger->setHandleEvents(true);

}


void PickModel::DisableDragger()
{
	removeChild(mDragger);
	mDragger->removeTransformUpdating(mSelection);
	mDragger->setHandleEvents(false);

	//removeChild(mRotateDragger);
	//mRotateDragger->removeTransformUpdating(mSelection);
	//mRotateDragger->setHandleEvents(false);
}
