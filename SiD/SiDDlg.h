
// SiDDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "PotDiagWnd.h"

#define ID_DRAWTIMER 191

class CPierceDiode;		//объ€влен в PierceDiode.h
class CPhaseDiagWnd;	//объ€влен в PhaseDiagWnd.h
class CPotDiagWnd;		//объ€влен в PotDiagWnd.h
class CPoincareMap;		//объ€влен в PotDiagWnd.h
class CEFieldDyn;		//объ€влен в EFieldDyn.h
class CMiddlePot;		//объ€влен в MiddlePot.h


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
	UINT nFrameTime;

	//—татус симул€ции: 0 - не запущена, 1 - идет, 2 - была запущена, но приостановлена
	BOOL m_bSimIsRun;

	int m_nE0RedrawCounter, m_nE0RedrawMaxCount;

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

	// ‘лажок "‘азовый портрет"
	CButton m_chkPhaseDiag;

	//окно фазового портрета
	CPhaseDiagWnd* pPhaseDiagWnd;

	// ‘лажок "–аспределение потенциала"
	CButton m_chkPotDiag;

	//окно распределени€ потенциала
	CPotDiagWnd* pPotDiagWnd;

	CPoincareMap* pPMapDiagWnd;

	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	
	afx_msg void OnBnClickedPhasediag();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusAlpha();
	void DestroyPhaseDiagWnd();
	void DestroyPotDiagWnd();
	void DestroyPMapDiagWnd();
	void DestroyEFieldWnd();
	void DestroyMiddlePotWnd();
	void RedrawGraphs();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPotdiag();
	
	CButton m_chkPMap;
	afx_msg void OnBnClickedPmap();
	afx_msg void OnClose();
	CButton m_chkEField;
	CEFieldDyn* pEFieldWnd;
	afx_msg void OnBnClickedEfield();
	afx_msg void OnBnClickedMiddlepot();
	CButton m_chkMiddlePot;
	CMiddlePot* pMiddlePotWnd;
};


BOOL fnCheckNumber(CString *);


BOOL fnItIsSignificantNumber(TCHAR);


