
// SiDDlg.h : header file
//

#pragma once
#include "afxwin.h"

#define ID_DRAWTIMER 191

class CPierceDiode;		//объ€влен в PierceDiode.h
class CPhaseDiagWnd;	//объ€влен в PhaseDiagWnd.h


// CSiDDlg dialog
class CSiDDlg : public CDialogEx
{
// Construction
public:
	CSiDDlg(CWnd* pParent = NULL);	// standard constructor
	~CSiDDlg();

// Dialog Data
	enum { IDD = IDD_SID_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
private:
	CString m_strAlpha;

	//update every nFrameTime ms
	DWORD nFrameTime;

	//—татус симул€ции: 0 - не запущена, 1 - идет, 2 - была запущена, но приостановлена
	BOOL m_bSimIsRun;

	void OnTimer(UINT);

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//«начение параметра ѕирса, деленного на пи
	double m_fAlphaWPi;

	//числ. симул€ци€ динамики “ј ќ… длительности должна выполн€тьс€ не ранее, чем за nFrameTime мс
	double fTimeStepPerFrame;

	//таймер, сигнализирующий о том, что пора перерисовать графики по новым данным симул€ции
	CEvent eventTimer;

	//главный объект программы
	CPierceDiode* thePierceDiode;

	//окно фазового портрета
	CPhaseDiagWnd* pPhaseDiagWnd;

	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	// ‘лажок "‘азовый портрет"
	CButton m_chkPhaseDiag;
	afx_msg void OnBnClickedPhasediag();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusAlpha();
//	afx_msg void OnDestroy();
	void DestroyPhaseDiagWnd();
};


BOOL fnCheckNumber(CString *);


BOOL fnItIsSignificantNumber(TCHAR);


