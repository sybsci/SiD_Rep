
// SiDDlg.h : header file
//

#pragma once
#include "afxwin.h"

#define ID_DRAWTIMER 191

class CPierceDiode;		//�������� � PierceDiode.h
class CPhaseDiagWnd;	//�������� � PhaseDiagWnd.h


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

	//������ ���������: 0 - �� ��������, 1 - ����, 2 - ���� ��������, �� ��������������
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
	//�������� ��������� �����, ��������� �� ��
	double m_fAlphaWPi;

	//����. ��������� �������� ����� ������������ ������ ����������� �� �����, ��� �� nFrameTime ��
	double fTimeStepPerFrame;

	//������, ��������������� � ���, ��� ���� ������������ ������� �� ����� ������ ���������
	CEvent eventTimer;

	//������� ������ ���������
	CPierceDiode* thePierceDiode;

	//���� �������� ��������
	CPhaseDiagWnd* pPhaseDiagWnd;

	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	// ������ "������� �������"
	CButton m_chkPhaseDiag;
	afx_msg void OnBnClickedPhasediag();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusAlpha();
//	afx_msg void OnDestroy();
	void DestroyPhaseDiagWnd();
};


BOOL fnCheckNumber(CString *);


BOOL fnItIsSignificantNumber(TCHAR);


