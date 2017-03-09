#pragma once
#include "common.h"
#include "Contrl2DManipulator.h"
#include "PickHandler.h"
#include "Contrl3DManipulator.h"

class YJOSG_API cOSG
{
/*初始化OSG*/
public:
	cOSG(CWnd* obj);
	~cOSG();
	virtual void InitOSG();
	virtual void InitCameraConfig(void);
	virtual void PreFrameUpdate();
	virtual void PostFrameUpdate(void);
	void Done(bool value) { mDone = value; }
	bool Done(void) { return mDone; }
	static void Render(void* ptr);

/*场景操作*/
public:
	//增加子节点到根节点
	void addNode(osg::ref_ptr<osg::Node> node);
	//增加Text到根节点
	void addText(const osg::Vec3 &position, const std::string &str,bool mark=true);
	//设置操作器
	void SetManipulator(osgGA::CameraManipulator *manipulator);
	//设置场景树
	void SetRoot(osg::Group* root);
	//获取root
	osg::ref_ptr<osg::Group> GetRoot() const;
	//获取Viewer
	osgViewer::Viewer* GetViewer() const;
	//删除全部非HUD节点
	void removeAllNode(bool all=false);
	//通过名字移除Node
	void removeNodeByName(const char* name);

/*绘制方法*/
public:
	// 根据点集生成连续线
	static osg::ref_ptr<osg::Geode> CreateContinuousLine(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors);
	// 根据点集生成线段
	static osg::ref_ptr<osg::Geode> CreateLineSegments(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors);
	// 根据点集生成连续三角面
	static osg::ref_ptr<osg::Geode> CreateContinuousTriangles(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec4Array> colors);
	//根据横截面点集生成三角面
	static osg::ref_ptr<osg::Geode> CreateTriangularMesh(osg::ref_ptr<osg::Vec3Array> coords, osg::ref_ptr<osg::Vec3Array> normals);
	// 创建TextHUD
	osg::ref_ptr<osg::Node> CreateTextHUD(osg::ref_ptr<osgText::Text> updateText);
	//创建坐标轴相机
	osg::ref_ptr<osg::Camera> CreateCoordCamera(osg::Camera* cm);
	//创建光源
	osg::Group*  CreateLight();
	//得到三角法向量
	static osg::Vec3 GetTriangleNormal(osg::Vec3 &point1,osg::Vec3 &point2,osg::Vec3 &point3);
private:
	
	bool		mDone;
	HWND		m_hWnd;
	CWnd*		m_object;
	
	osgViewer::Viewer*				mViewer;
public:
	osg::ref_ptr<osg::Group>		mRoot;
	osg::ref_ptr<osg::Node>			mNode;
};


class cRenderingThread : public OpenThreads::Thread
{
public:
	cRenderingThread(cOSG* ptr);
	virtual ~cRenderingThread();

	virtual void run();

protected:
	cOSG* _ptr;
	bool _done;
};

