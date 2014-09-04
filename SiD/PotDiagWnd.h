#pragma once
#include "afxwin.h"
#include "CommonData.h"

#include "PotGraphControl.h"

// CPotDiagWnd dialog

class CPotDiagWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CPotDiagWnd)

private:
	CPotGraphControl m_stcGraph;

public:
	CPotDiagWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPotDiagWnd();

// Dialog Data
	enum { IDD = IDD_POTDIAGWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	

	void SetEnableSaveButton(BOOL);
	afx_msg void OnBnClickedBtnSavepotscreen();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void InitializeYAxe();
	void UpdateGraph();
};
