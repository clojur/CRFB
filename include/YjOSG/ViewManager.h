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
	//ͨ���ڵ���ɾ���ڵ�
	void RemoveNodeByName(const char *name, HWND hWnd);
	//ɾ��ȫ��Ĭ�Ͻڵ�
	void RemoveAllNode(HWND hWnd);
	//����view
	osg::ref_ptr<osgViewer::View> getView(HWND hWnd){ return _viewMap[hWnd]; }

protected:
	void InitView(HWND hWnd, osgViewer::View* view);
	void InitCameraConfig(HWND hWnd, osgViewer::View* view);
	// ����TextHUD
	osg::ref_ptr<osg::Node> CreateTextHUD(osg::ref_ptr<osgText::Text> updateText);
	//�������������
	osg::ref_ptr<osg::Camera> CreateCoordCamera(osg::Camera* cm);
	//������Դ
	osg::Group* CreateLight(osg::Group* root);
protected:
	std::map<HWND, osg::ref_ptr<osgViewer::View>> _viewMap;
	std::map<HWND, unsigned int> _rootIndex;
};

