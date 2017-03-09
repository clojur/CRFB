// RigidFrame.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RigidFrame.h"
#include "MainFrm.h"
#include "Dialog objects\DlgNewBridge.h"

#include "ChildFrm.h"
#include "RigidFrameDoc.h"
#include "RigidFrameView.h"
#include <RigidFrameBridge/RigidFrameBridge.h>
#include <YjOSG/YjOSG.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CRigidFrameBridge* Get_Brg()
{
	CBCGPMDIFrameWnd* pWnd = (CBCGPMDIFrameWnd*)AfxGetMainWnd();
	CBCGPMDIChildWnd* pChildWnd = (CBCGPMDIChildWnd*)pWnd->MDIGetActive();
	CRigidFrameDoc* pDoc = (CRigidFrameDoc*)pChildWnd->GetActiveDocument();
	CRigidFrameBridge* brg = pDoc->RigidFrameBridgeExample;
	return brg;
}



// CRigidFrameApp

BEGIN_MESSAGE_MAP(CRigidFrameApp, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CRigidFrameApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CBCGPWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CBCGPWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_WINDOW_CLOSEALL, &CRigidFrameApp::OnWindowCloseall)
END_MESSAGE_MAP()


// CRigidFrameApp construction

CRigidFrameApp::CRigidFrameApp() :hasProject(false), projectModified(false)
{
	//检测内存泄漏
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	pDocTemplate_Ger = nullptr;

	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLUE, ID_VIEW_APPLOOK_2007);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLACK, ID_VIEW_APPLOOK_2007_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_SILVER, ID_VIEW_APPLOOK_2007_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_AQUA, ID_VIEW_APPLOOK_2007_3);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_BLUE, ID_VIEW_APPLOOK_2010_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_BLACK, ID_VIEW_APPLOOK_2010_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2010_SILVER, ID_VIEW_APPLOOK_2010_3);
	AddVisualTheme(BCGP_VISUAL_THEME_VS_2010, ID_VIEW_APPLOOK_SCENIC);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2013_WHITE, ID_VIEW_APPLOOK_2013_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2013_GRAY, ID_VIEW_APPLOOK_2013_2);
	AddVisualTheme(BCGP_VISUAL_THEME_VS_2013_BLUE, ID_VIEW_APPLOOK_2013_3);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_COLORFUL, ID_VIEW_APPLOOK_2016_1);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_DARK_GRAY, ID_VIEW_APPLOOK_2016_2);
	AddVisualTheme(BCGP_VISUAL_THEME_OFFICE_2016_WHITE, ID_VIEW_APPLOOK_2016_3);

	SetVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLACK);


	// 创建自定义字体:
	LOGFONT lf;
	globalData.fontRegular.GetLogFont(&lf);
	lf.lfHeight = (int)(1.1 * lf.lfHeight);
	wcscpy(lf.lfFaceName, _T("微软雅黑"));
	m_fontRegular.CreateFontIndirect(&lf);

}


// The one and only CRigidFrameApp object

CRigidFrameApp theApp;


// CRigidFrameApp initialization

BOOL CRigidFrameApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBCGPWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("CDIP RigidFrame Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	SetRegistryBase(_T("Settings"));


	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	
	pDocTemplate_Ger = new CMultiDocTemplate(IDR_RigidFrameTYPE,
		RUNTIME_CLASS(CRigidFrameDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CRigidFrameView));
	if (!pDocTemplate_Ger)
		return FALSE;
	AddDocTemplate(pDocTemplate_Ger);

	//_viewMgr = new ViewerManager;
	_viewMgr = new CompViewerManager;

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);


	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		if (!pMainFrame->LoadMDIState(GetRegSectionPath()) || 
			DYNAMIC_DOWNCAST(CMDIChildWnd, pMainFrame->GetActiveFrame()) == NULL)
		{
			if (!ProcessShellCommand(cmdInfo))
				return FALSE;
		}
	}
	else
	{
		// Dispatch commands specified on the command line
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

// CRigidFrameApp message handlers

int CRigidFrameApp::ExitInstance() 
{
	delete _viewMgr;
	return CBCGPWinApp::ExitInstance();
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	CBCGPURLLinkButton m_btnURL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBrgRename();
//	afx_msg void OnBrgDel();
};


BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
	
//	ON_COMMAND(ID_BRG_RENAME, &CAboutDlg::OnBrgRename)
//	ON_COMMAND(ID_BRG_DEL, &CAboutDlg::OnBrgDel)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CBCGPDialog(CAboutDlg::IDD)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
}


// App command to run the dialog
void CRigidFrameApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CRigidFrameApp message handlers


void CRigidFrameApp::PreLoadState()
{
}

BOOL CRigidFrameApp::SaveAllModified()
{
	if (!CBCGPWinApp::SaveAllModified())
	{
		return FALSE;
	}
	
	CBCGPMDIFrameWnd* pMainFrame = DYNAMIC_DOWNCAST(CBCGPMDIFrameWnd, m_pMainWnd);
	if (pMainFrame != NULL)
	{
		pMainFrame->SaveMDIState(GetRegSectionPath());
	}
	
	return TRUE;
}



void CRigidFrameApp::OnFileNew()
{
	CProject *proj = ((CMainFrame*)AfxGetMainWnd())->Proj;
	CDlgNewBridge dlg(proj->GetNextIndex());

	if (dlg.DoModal() == IDOK)
	{
		proj->AddBridge(dlg.m_Newbridge);
		CBCGPWinApp::OnFileNew();
	}
}




void CRigidFrameApp::OnWindowCloseall()
{
	if (m_pDocManager != NULL)
	{
		if (m_pDocManager->SaveAllModified())
		{
			m_pDocManager->CloseAllDocuments(FALSE);
		}
	}
}
