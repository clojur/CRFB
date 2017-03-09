#pragma  once
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>

#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/GUIEventHandler>
#include<osgGA/OrbitManipulator>
#include<osgGA/TrackballManipulator>
#include<osg/PositionAttitudeTransform>

#include <osgUtil/Optimizer>
#include <osgUtil/LineSegmentIntersector>  
#include<osgUtil/RayIntersector>
#include<osgUtil/PlaneIntersector>
#include<osgUtil/SmoothingVisitor>

#include<osgFX/Scribe>
#include<osgFX/Outline>

#include <osgText/Text>

#include <osg/io_utils>
#include <osg/Group>
#include<osg/Material>
#include<osg/Multisample>
#include<osg/MatrixTransform>
#include<osg/PolygonMode>
#include<osg/NodeVisitor>
#include <osg/BlendColor>
#include <osg/BlendFunc>

#include <osgManipulator/Selection>
#include <osgManipulator/CommandManager>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/RotateSphereDragger>
#include <osgManipulator/TranslateAxisDragger>

#include <string>
#include <sstream>
