#pragma once
#include "afxwin.h"


// CPhaseDiagWnd dialog

class CPhaseDiagWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CPhaseDiagWnd)

public:
	CPhaseDiagWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPhaseDiagWnd();

// Dialog Data
	enum { IDD = IDD_PHASEDIAGWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Область вывода графика
	CStatic m_stcGraph;
};
