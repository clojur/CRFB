#pragma once
#include "ViewManager.h"
#include <osgViewer/Viewer>
#include <OpenThreads/Thread>
#include <osgText/Text>

class YJOSG_API ViewerManager : public ViewManager
{
public:
	ViewerManager();
	~ViewerManager();

	void AddView(HWND hWnd) override;
	void RemoveView(HWND hWnd) override;
	void Display(HWND hWnd) override;

private:
	std::map<HWND, OpenThreads::Thread*> _threads;
};

