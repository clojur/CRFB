#pragma once
#include "common.h"
class CroodAxisCallBack :public osg::NodeCallback
{
public:
	CroodAxisCallBack(osg::Camera* cm) :camera(cm)
	{

	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{

		//创建矩阵转换节点
		osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*>(node);
		//创建矩阵
		osg::Matrix mxr;
		osg::Matrix mxt;
		mxr.makeRotate(90.0f, osg::Vec3(1.0f, 0.0f, 0.0f));
		//设置矩阵
		mxt.makeRotate(camera->getViewMatrix().getRotate());

		mt->setMatrix(mxt*mxr);

		//继续遍历
		traverse(node, nv);

	}

public:
	osg::Camera* camera;
};

