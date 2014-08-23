// PhaseDiagWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "PhaseDiagWnd.h"
#include "afxdialogex.h"

#include "CommonData.h"
#include "SiDDlg.h"

extern PlotStruct PlotData;
CRITICAL_SECTION crS;

// CPhaseDiagWnd dialog

IMPLEMENT_DYNAMIC(CPhaseDiagWnd, CDialogEx)

CPhaseDiagWnd::CPhaseDiagWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPhaseDiagWnd::IDD, pParent)
{
	InitializeCriticalSection(&crS);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPhaseDiagWnd::~CPhaseDiagWnd()
{
	DestroyWindow();
}

void CPhaseDiagWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, m_stcGraph);
}


BEGIN_MESSAGE_MAP(CPhaseDiagWnd, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPhaseDiagWnd message handlers


BOOL CPhaseDiagWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	CButton *pBtn = (CButton*)this->GetDlgItem(IDC_BTN_SAVESCREEN);
	HANDLE hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVESCREEN), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPhaseDiagWnd::OnOK()
{
	//CDialogEx::OnOK();
}


void CPhaseDiagWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	
}


void CPhaseDiagWnd::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkPhaseDiag.SetCheck(0);
	pMainDlg->DestroyPhaseDiagWnd();
}
