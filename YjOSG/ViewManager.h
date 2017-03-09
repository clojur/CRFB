#pragma once
#include <windows.h>
#include "Component.h"
#include <osgText/Text>
#include "common.h"

class ComponentCallback;

class  YJOSG_API ViewManager
{
protected:
	ViewManager() {}

public:
	virtual ~ViewManager() {}
	virtual void AddView(HWND hWnd) {}
	virtual void RemoveView(HWND hWnd) {}
	virtual void SetSceneData(HWND hWnd, ComponentCallback* cb);
	virtual void SetSceneData(HWND hWnd, osg::Node* node);
	virtual osg::Node* GetSceneData(HWND hWnd);
	virtual void Display(HWND hWnd) {}
	//通过节点名删除节点
	void RemoveNodeByName(const char *name, HWND hWnd);
	//删除全部默认节点
	void RemoveAllNode(HWND hWnd);
	//返回view
	osg::ref_ptr<osgViewer::View> getView(HWND hWnd){ return _viewMap[hWnd]; }

protected:
	void InitView(HWND hWnd, osgViewer::View* view);
	void InitCameraConfig(HWND hWnd, osgViewer::View* view);
	// 创建TextHUD
	osg::ref_ptr<osg::Node> CreateTextHUD(osg::ref_ptr<osgText::Text> updateText);
	//创建坐标轴相机
	osg::ref_ptr<osg::Camera> CreateCoordCamera(osg::Camera* cm);
	//创建光源
	osg::Group* CreateLight(osg::Group* root);
protected:
	std::map<HWND, osg::ref_ptr<osgViewer::View>> _viewMap;
	std::map<HWND, unsigned int> _rootIndex;
};

