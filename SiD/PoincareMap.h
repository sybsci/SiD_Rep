#pragma once
#include "afxwin.h"
#include "CommonData.h"

#include "PoincareMapControl.h"

// CPoincareMap dialog

class CPoincareMap : public CDialogEx
{
	DECLARE_DYNAMIC(CPoincareMap)

public:
	CPoincareMap(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPoincareMap();

// Dialog Data
	enum { IDD = IDD_POINCAREMAP };

private:
	CPoincareMapControl m_stcGraph;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();

	void InitializeYAxe();
	void UpdateGraph();
	void SetEnableSaveButton(BOOL);

};
