#pragma once
#include "CommonData.h"
#include "MiddlePotControl.h"
#include "afxwin.h"

// CMiddlePot dialog

class CMiddlePot : public CDialogEx
{
	DECLARE_DYNAMIC(CMiddlePot)

public:
	CMiddlePot(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMiddlePot();

// Dialog Data
	enum { IDD = IDD_MIDDLEPOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()

public:
	void SetEnableSaveButton(BOOL);
	void InitializeYAxe();
	void UpdateGraph();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMiddlePotControl m_stcGraph;
	virtual void OnOK();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CButton m_chkPrintMPPMap;
	afx_msg void OnBnClickedBtnSavemiddlepot();
	afx_msg void OnBnClickedBtnSavetxtmiddlepot();
	afx_msg void OnBnClickedPrintmiddlepotpmap();
};
