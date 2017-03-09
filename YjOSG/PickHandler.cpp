#include "stdafx.h"
#include "PickHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


PickHandler::PickHandler(osg::ref_ptr<osgText::Text> updatetext, osg::ref_ptr<osg::Group> model)
{
	m_updateText = updatetext;
	m_Root = model;
	m_intersection = false;
	m_clickCount = 0;
}


PickHandler::~PickHandler()
{
}

bool PickHandler::handle(const osgGA::GUIEventAdapter&ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
	switch (ea.getEventType())
	{
		case (osgGA::GUIEventAdapter::MOVE) :
		{
			std::string str = "";
			std::ostringstream os;

			//求出当前屏幕鼠标的世界坐标位置
			osg::ref_ptr<osg::Camera> camera = viewer->getCamera();
			osg::Matrix VPW = camera->getViewMatrix() *
				camera->getProjectionMatrix() *
				camera->getViewport()->computeWindowMatrix();
			osg::Matrix inverseVPW = osg::Matrix::inverse(VPW);
			osg::Vec3 window(ea.getX(),ea.getY(),0.0f);
			osg::Vec3d world = window * inverseVPW;

			//求出当前相机位置
			osg::Matrix mat;
			mat = osg::Matrix::inverse(viewer->getCamera()->getViewMatrix());
			m_cameraPosition = mat.getTrans();

			//求出相机到鼠标的方向
			osg::Vec3 CToM_direction;
			CToM_direction = world-m_cameraPosition;
			CToM_direction.normalize();

			//设计射线求交器
			osgUtil::RayIntersector::Intersections intersections;//保存交到的集合
			osg::ref_ptr<osgUtil::RayIntersector> ri = new osgUtil::RayIntersector(m_cameraPosition,CToM_direction);//初始化求交器
			osg::ref_ptr<osgUtil::IntersectionVisitor> iv = new osgUtil::IntersectionVisitor(ri);//求交遍历器
			osgUtil::RayIntersector::Intersections::iterator RayIter;//交点集合迭代器
			m_Root->accept(*iv);//求交遍历整个场景树

			//如果交到对象
			if (ri->containsIntersections())
			{
				m_intersection = true;
				intersections = ri->getIntersections();
				//只需要交点集合中的第一个点
				RayIter = intersections.begin();
				//打印交点
				m_intersectionPoint = RayIter->getWorldIntersectPoint();
				os << m_intersectionPoint.x() << " , " << m_intersectionPoint.y() << " , " << m_intersectionPoint.z() << std::endl;
				str += os.str();
				setLable(str);
			}
			else
				setLable("");
			
			return false;
		}
		case(osgGA::GUIEventAdapter::PUSH):
		{
			
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				if (m_intersection)
				{
					m_clickCount++;
					if (m_clickCount == 1)
					{
						m_Root->addChild(CreatePixel(m_intersectionPoint,"p1"));
						m_OldP = m_intersectionPoint;
					}	
					if (m_clickCount == 2)
					{
						m_Root->addChild(CreatePixel(m_intersectionPoint, "p2"));
						m_NewP = m_intersectionPoint;
						this->RemoveNodeByName("intersectionLine");
						m_Root->addChild(CreateLine(m_OldP,m_NewP));
					}
					if (m_clickCount == 3)
					{
						this->RemoveNodeByName("p1");
						this->RemoveNodeByName("p2");
						this->RemoveNodeByName("intersectionLine");
						m_clickCount = 1;
						m_Root->addChild(CreatePixel(m_intersectionPoint, "p1"));
						m_OldP = m_intersectionPoint;
					}
				}	
			}
			
			return false;
		}
		default:
			return false;
	}
}

bool PickHandler::setLable(const std::string &updatetext)
{
	if (m_updateText.get()) 
	{
		osg::ref_ptr<osgText::Font> font = osgText::readFontFile("C:\\Windows\\Fonts\\fonts\\simsun.ttc");//读取中文字体
		m_updateText->setFont(font);
		m_updateText->setText(updatetext);
		return true;
	}
	if (updatetext == "")
		return false;
	return false;
}

osg::ref_ptr<osg::Geode> PickHandler::CreateShape(osg::Vec3& ct,const char* str)
{
	osg::ref_ptr < osg::Geode>geode = new osg::Geode;
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(new osg::Sphere(ct, 0.05f));
	sd->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	geode->addDrawable(sd);
	geode->setName(str);
	return geode;
}

osg::ref_ptr<osg::Geode> PickHandler::CreatePixel(osg::Vec3& point, const char* str)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

	vertices->push_back(point);
	colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	gm->setVertexArray(vertices);
	gm->setColorArray(colors, osg::Array::BIND_PER_VERTEX);
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,1));
	geode->addDrawable(gm);
	geode->setName(str);
	return geode;
}
osg::ref_ptr<osg::Geode> PickHandler::CreateLine(osg::Vec3& begin, osg::Vec3& end)
{
	osg::ref_ptr<osg::Geode>geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry>gm = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

	vertices->push_back(begin);
	vertices->push_back(end);
	gm->setVertexArray(vertices);
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	gm->setColorArray(colors, osg::Array::BIND_OVERALL);
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 2));
	geode->addDrawable(gm);
	geode->setName("intersectionLine");
	return geode;
}

bool PickHandler::RemoveNodeByName(const char* str)
{
	for (unsigned int i = 0; i < m_Root->getNumChildren(); ++i)
	{
		osg::ref_ptr<osg::Node> node = m_Root->getChild(i);
		if (node->getName() == str)
		{
			m_Root->removeChild(node);
			return true;
		}	
	}
	return false;
}


