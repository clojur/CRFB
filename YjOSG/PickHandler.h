#pragma once
#include "common.h"
class PickHandler :public osgGA::GUIEventHandler
{
public:
	PickHandler(osg::ref_ptr<osgText::Text> updatetext,osg::ref_ptr<osg::Group> model);
	~PickHandler();

	bool handle(const osgGA::GUIEventAdapter&ea,osgGA::GUIActionAdapter& aa);
	inline wchar_t* AnsiToUnicode(const char* szStr);

protected:
	osg::ref_ptr<osgText::Text> m_updateText;
	osg::ref_ptr<osg::Group> m_Root;
	osg::Vec3 m_intersectionPoint; 

	osg::ref_ptr<osg::Geode> CreateShape(osg::Vec3& ct, const char* str);
	osg::ref_ptr<osg::Geode> CreateLine(osg::Vec3& begin, osg::Vec3& end);
	osg::ref_ptr<osg::Geode> CreatePixel(osg::Vec3& point,const char* str);
public:
	bool setLable(const std::string &updatetext);
	bool RemoveNodeByName(const char* str);
	
private:
	bool m_intersection;
	osg::Vec3 m_cameraPosition;
	osg::Vec3 m_OldP;
	osg::Vec3 m_NewP;
	int m_clickCount;

	
};
