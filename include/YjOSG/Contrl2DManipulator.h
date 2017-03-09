#pragma once
#include "common.h"
class YJOSG_API Contrl2DManipulator :public osgGA::OrbitManipulator
{
public:
	Contrl2DManipulator(osg::ref_ptr<osg::Group> &model);
	~Contrl2DManipulator();

	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
	virtual void  setByMatrix(const osg::Matrixd &matrix);
	virtual void  setByInverseMatrix(const osg::Matrixd &matrix);
	virtual osg::Matrixd  getMatrix()const;
	virtual osg::Matrixd  getInverseMatrix()const;

	void initCamera();
	osg::Vec3 getMouseToWorld(osg::ref_ptr<osg::Camera> camera, float x, float y);

private:
	double m_radius;
	double m_distance;
	double m_deltaTime;
	double m_lastFrame;
	double m_currentFrame;

	osg::Vec3 m_viewdirection;
	osg::Vec3 m_center;

	osg::Vec3 m_CenterPosition;
	osg::Vec3 m_ViewerPosition;
	osg::Vec3 m_UpNormal;

	osg::ref_ptr<osg::Group> m_model;

	osg::Vec3 MouseWorld;

	bool first;

};


