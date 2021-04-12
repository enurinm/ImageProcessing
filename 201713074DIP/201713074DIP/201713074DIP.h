
// 201713074DIP.h : main header file for the 201713074DIP application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy201713074DIPApp:
// See 201713074DIP.cpp for the implementation of this class
//

class CMy201713074DIPApp : public CWinAppEx
{
public:
	CMy201713074DIPApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy201713074DIPApp theApp;
