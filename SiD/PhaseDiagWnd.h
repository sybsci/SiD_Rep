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
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	// ������� ������ �������
	CStatic m_stcGraph;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnClose();
};
