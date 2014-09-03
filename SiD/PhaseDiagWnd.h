#pragma once
#include "afxwin.h"
#include "CommonData.h"

#include "GraphControl.h"


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
	// контрол, куда рисуется график
	CGraphControl m_stcGraph;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	void InitializeYAxe();
	void UpdateGraph();
	afx_msg void OnBnClickedBtnSavescreen();
	void SetEnableSaveButton(BOOL);
};
