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

		//��������ת���ڵ�
		osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*>(node);
		//��������
		osg::Matrix mxr;
		osg::Matrix mxt;
		mxr.makeRotate(90.0f, osg::Vec3(1.0f, 0.0f, 0.0f));
		//���þ���
		mxt.makeRotate(camera->getViewMatrix().getRotate());

		mt->setMatrix(mxt*mxr);

		//��������
		traverse(node, nv);

	}

public:
	osg::Camera* camera;
};

