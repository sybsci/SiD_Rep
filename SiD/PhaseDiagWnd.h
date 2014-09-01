#pragma once
#include "afxwin.h"
#include <vector>
#include "CommonData.h"


// CPhaseDiagWnd dialog

class CPhaseDiagWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CPhaseDiagWnd)

public:
	CPhaseDiagWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPhaseDiagWnd();

// Dialog Data
	enum { IDD = IDD_PHASEDIAGWND };

private:
	double m_fMinVel, m_fMaxVel;
	int m_nMinVel, m_nMaxVel;
	std::vector<CString> m_vecYAxe;

	CString fnGetTickLabelText(int);
	CoordParamStruct GetCoordParams();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnClose();

	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void InitializeYAxe();
};
