
// SiDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "SiDDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSiDDlg dialog



CSiDDlg::CSiDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSiDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSiDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHASEDIAG, m_chkPhaseDiag);
}

BEGIN_MESSAGE_MAP(CSiDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CSiDDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CSiDDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CSiDDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_PHASEDIAG, &CSiDDlg::OnBnClickedPhasediag)
END_MESSAGE_MAP()


// CSiDDlg message handlers

BOOL CSiDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	///////////////////////////////////////////////////////////////////////////

	//установим иконки кнопкам

	CButton *pBtn = (CButton*)this->GetDlgItem(IDC_BTN_START);
	HANDLE hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_START), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	pBtn = 0;

	pBtn = (CButton*)this->GetDlgItem(IDC_BTN_PAUSE);
	hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_PAUSE), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	pBtn = 0;

	pBtn = (CButton*)this->GetDlgItem(IDC_BTN_STOP);
	hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_STOP), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);


	///////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSiDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CSiDDlg::OnPaint()
{
	// If you add a minimize button to your dialog, you will need the code below
	//  to draw the icon.  For MFC applications using the document/view model,
	//  this is automatically done for you by the framework.
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSiDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSiDDlg::OnOK(){
	//чтобы по нажатию "Enter" окно не закрывалось, нужно оставить пустой эту процедуру
}


void CSiDDlg::OnBnClickedBtnStart()
{
	GetDlgItem(IDC_BTN_START)	->EnableWindow(0);
	GetDlgItem(IDC_BTN_PAUSE)	->EnableWindow(1);
	GetDlgItem(IDC_BTN_STOP)	->EnableWindow(1);
}


void CSiDDlg::OnBnClickedBtnPause()
{
	GetDlgItem(IDC_BTN_START)	->EnableWindow(1);
	GetDlgItem(IDC_BTN_PAUSE)	->EnableWindow(0);
	GetDlgItem(IDC_BTN_STOP)	->EnableWindow(1);
}


void CSiDDlg::OnBnClickedBtnStop()
{
	GetDlgItem(IDC_BTN_START)	->EnableWindow(1);
	GetDlgItem(IDC_BTN_PAUSE)	->EnableWindow(0);
	GetDlgItem(IDC_BTN_STOP)	->EnableWindow(0);
}


void CSiDDlg::OnBnClickedPhasediag()
{
	
	if (m_chkPhaseDiag.GetCheck()){
		//отобразить окно фазового портрета
		
	}
	else{
		//закрыть окно фазового портрета, если оно открыто
	
	}

}
