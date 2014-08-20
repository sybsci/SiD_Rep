
// SiD.h : main header file for the SiD application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSiDApp:
// See SiD.cpp for the implementation of this class
//

class CSiDApp : public CWinApp
{
public:
	CSiDApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSiDApp theApp;