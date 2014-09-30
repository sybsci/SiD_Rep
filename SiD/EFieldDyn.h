#pragma once
#include "CommonData.h"
#include "EFieldControl.h"
#include "afxwin.h"



// CEFieldDyn dialog

class CEFieldDyn : public CDialogEx
{
	DECLARE_DYNAMIC(CEFieldDyn)

public:
	CEFieldDyn(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEFieldDyn();

// Dialog Data
	enum { IDD = IDD_EFIELDDYN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEFieldControl m_stcGraph;
	virtual void OnOK();

	void SetEnableSaveButton(BOOL);
	void InitializeYAxe();
	void UpdateGraph();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSaveefield();
	afx_msg void OnBnClickedBtnSavetxtefield();
	CButton m_chkPrintE0PMap;
	afx_msg void OnBnClickedPrinte0pmap();
};
