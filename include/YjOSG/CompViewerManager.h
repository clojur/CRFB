#pragma once
#include "ViewManager.h"
#include <osgViewer/View>
#include <osgViewer/CompositeViewer>
#include "Component.h"

class YJOSG_API CompViewerManager : public ViewManager
{
public:
	CompViewerManager();
	~CompViewerManager();

	osgViewer::CompositeViewer* getViewer() const { return _viewer; }

	void AddView(HWND hWnd) override;
	void RemoveView(HWND hWnd) override;
	void Display(HWND hWnd) override;

	void PreFrameUpdate(void);
	void PostFrameUpdate(void);

private:
	osg::ref_ptr<osgViewer::CompositeViewer> _viewer;
	OpenThreads::Thread* _threadHandle;
	std::deque<osg::ref_ptr<osgViewer::View>> _addViewQueue;
	std::deque<osg::ref_ptr<osgViewer::View>> _delViewQueue;
	bool _added;
	bool _removed;
};

