
// SiDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "SiDDlg.h"
#include "afxdialogex.h"

#include "PierceDiode.h"
#include "PhaseDiagWnd.h"
#include "PotDiagWnd.h"
#include "PoincareMap.h"
#include "EFieldDyn.h"
#include "MiddlePot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////
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



/////////////////////////////////////////////
// CSiDDlg dialog


CSiDDlg::CSiDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSiDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bSimIsRun = 0;

	m_nE0RedrawCounter = 0;
	m_nE0RedrawMaxCount = 5;

	fTimeStepPerFrame = 0.1;
	nFrameTime = 50;

	m_fAlphaWPi = 1.1;
	m_strAlpha = CString("1.1");

	thePierceDiode = new CPierceDiode(this);

	pPhaseDiagWnd = 0;
	pPotDiagWnd = 0;
	pPMapDiagWnd = 0;
	pEFieldWnd = 0;
	pMiddlePotWnd = 0;
}

CSiDDlg::~CSiDDlg()
{
	delete thePierceDiode;

	if (pPhaseDiagWnd)
		delete pPhaseDiagWnd;

	if (pPotDiagWnd)
		delete pPotDiagWnd;

	if (pPMapDiagWnd)
		delete pPMapDiagWnd;

	if (pEFieldWnd)
		delete pEFieldWnd;

	if (pMiddlePotWnd)
		delete pMiddlePotWnd;
}

void CSiDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHASEDIAG, m_chkPhaseDiag);
	DDX_Control(pDX, IDC_POTDIAG, m_chkPotDiag);
	DDX_Control(pDX, IDC_PMAP, m_chkPMap);
	DDX_Control(pDX, IDC_EFIELD, m_chkEField);
	DDX_Control(pDX, IDC_MIDDLEPOT, m_chkMiddlePot);
}

BEGIN_MESSAGE_MAP(CSiDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CSiDDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CSiDDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CSiDDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_PHASEDIAG, &CSiDDlg::OnBnClickedPhasediag)
	ON_EN_KILLFOCUS(IDC_ALPHA, &CSiDDlg::OnEnKillfocusAlpha)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_POTDIAG, &CSiDDlg::OnBnClickedPotdiag)
	ON_BN_CLICKED(IDC_PMAP, &CSiDDlg::OnBnClickedPmap)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_EFIELD, &CSiDDlg::OnBnClickedEfield)
	ON_BN_CLICKED(IDC_MIDDLEPOT, &CSiDDlg::OnBnClickedMiddlepot)
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


	SetDlgItemTextW(IDC_ALPHA, (LPCTSTR)m_strAlpha);

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

	if (m_bSimIsRun != 1)
	{	
		SetTimer(ID_DRAWTIMER, nFrameTime, NULL);
		
		if (0 == m_bSimIsRun)
			thePierceDiode->Initialize();
		thePierceDiode->StartCalcThread();
		
		m_bSimIsRun = 1;

		if (pPhaseDiagWnd)
			pPhaseDiagWnd->SetEnableSaveButton(0);
		if (pPotDiagWnd)
			pPotDiagWnd->SetEnableSaveButton(0);
		if (pPMapDiagWnd)
			pPMapDiagWnd->SetEnableSaveButton(0);
		if (pEFieldWnd)
			pEFieldWnd->SetEnableSaveButton(0);
		if (pMiddlePotWnd)
			pMiddlePotWnd->SetEnableSaveButton(0);
	};
}


void CSiDDlg::OnBnClickedBtnPause()
{
	GetDlgItem(IDC_BTN_START)	->EnableWindow(1);
	GetDlgItem(IDC_BTN_PAUSE)	->EnableWindow(0);
	GetDlgItem(IDC_BTN_STOP)	->EnableWindow(1);

	m_bSimIsRun = 2;

	thePierceDiode->SetRunFlag(false);

	eventTimer.SetEvent();
	KillTimer(ID_DRAWTIMER);

	if (pPhaseDiagWnd)
		pPhaseDiagWnd->SetEnableSaveButton(1);
	if (pPotDiagWnd)
		pPotDiagWnd->SetEnableSaveButton(1);
	if (pPMapDiagWnd)
		pPMapDiagWnd->SetEnableSaveButton(1);
	if (pEFieldWnd)
		pEFieldWnd->SetEnableSaveButton(1);
	if (pMiddlePotWnd)
		pMiddlePotWnd->SetEnableSaveButton(1);
}


void CSiDDlg::OnBnClickedBtnStop()
{
	GetDlgItem(IDC_BTN_START)	->EnableWindow(1);
	GetDlgItem(IDC_BTN_PAUSE)	->EnableWindow(0);
	GetDlgItem(IDC_BTN_STOP)	->EnableWindow(0);

	m_bSimIsRun = 0;

	thePierceDiode->SetRunFlag(false);

	eventTimer.SetEvent();
	KillTimer(ID_DRAWTIMER);

	thePierceDiode->ClearData();

	if (pPhaseDiagWnd)
	{
		pPhaseDiagWnd->InitializeYAxe();
		pPhaseDiagWnd->SetEnableSaveButton(1);
	};

	if (pPotDiagWnd)
	{
		pPotDiagWnd->InitializeYAxe();
		pPotDiagWnd->SetEnableSaveButton(1);
	};

	if (pPMapDiagWnd)
	{
		pPMapDiagWnd->InitializeYAxe();
		pPMapDiagWnd->SetEnableSaveButton(1);
	};

	if (pEFieldWnd)
	{
		pEFieldWnd->InitializeYAxe();
		pEFieldWnd->SetEnableSaveButton(1);
	};

	if (pMiddlePotWnd)
	{
		pMiddlePotWnd->InitializeYAxe();
		pMiddlePotWnd->SetEnableSaveButton(1);
	};

	m_nE0RedrawCounter = m_nE0RedrawMaxCount - 1;
	RedrawGraphs();
}


void CSiDDlg::OnBnClickedPhasediag()
{
	if (m_chkPhaseDiag.GetCheck()){
		//отобразить окно фазового портрета
		if (!pPhaseDiagWnd)
		{
			pPhaseDiagWnd = new CPhaseDiagWnd();
			pPhaseDiagWnd->Create(IDD_PHASEDIAGWND, GetDesktopWindow());
			pPhaseDiagWnd->MoveWindow(10, 10, 450, 350);
			if (m_bSimIsRun != 1)
				pPhaseDiagWnd->SetEnableSaveButton(1);
			pPhaseDiagWnd->ShowWindow(SW_SHOW);
		}
	}
	else{
		//закрыть окно фазового портрета, если оно открыто
		if (pPhaseDiagWnd)
		{
			pPhaseDiagWnd->CloseWindow();
			DestroyPhaseDiagWnd();
		}
	}

}


BOOL CSiDDlg::PreTranslateMessage(MSG* pMsg)
{

	//Переход на следующий диалоговый элемент по нажатию "Enter"
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
	    CWnd* pFocus = GetFocus();
		BOOL bKind = pFocus->IsKindOf(RUNTIME_CLASS(CEdit));
		if (pFocus && bKind)
	    {
	        NextDlgCtrl();
	        return TRUE;
	    }
	}
	////

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSiDDlg::OnEnKillfocusAlpha()
{
	CString strBuf;
	CString& refstrBuf = strBuf;
	
	GetDlgItemTextW(IDC_ALPHA, refstrBuf);
	
	double fbuf;
	
	if (fnCheckNumber(&strBuf)){
		fbuf = _wtof(strBuf);
	}
	else{
		AfxMessageBox(_T("Некорректное число (параметр Пирса)"));
		SetDlgItemTextW(IDC_ALPHA, (LPCTSTR)m_strAlpha);
		return void();
	}
	
	if (m_fAlphaWPi != fbuf){
		m_fAlphaWPi = fbuf;
		thePierceDiode->SetAlpha(m_fAlphaWPi);
	};
	m_strAlpha = strBuf;
	SetDlgItemTextW(IDC_ALPHA, (LPCTSTR)m_strAlpha);
}


BOOL fnCheckNumber(CString* pstrNumber)
{
	
	CString bufInt=_T(""), bufFract=_T(""), str = *pstrNumber;
	
	if (0 == str.GetLength()){
		*pstrNumber = _T("0");
		return TRUE;
	};
	
	CString alphabet = _T("0123456789");
		
	TCHAR symbol;
	
	bool bHaveSignificantSymbol = false;
	BOOL bSignificant = FALSE;
	
	str.Replace(_T(","), _T("."));
	
	int nInd, nInd2;
		
	
	nInd = str.Find(_T("."));
	if (-1 == nInd){
		//целое число
		for (int i=0; i<str.GetLength(); i++){
	
			symbol = str.GetAt(i);
			bSignificant = fnItIsSignificantNumber(symbol);
	
	
			if (bHaveSignificantSymbol){
	
				if (!bSignificant)
					return FALSE;	//введены лишние символы
	
			}
			else{
	
				if (bSignificant){
					bHaveSignificantSymbol = true;
				}
				else{
					continue;
				};
	
			};
	
			bufInt += symbol;
	
		}
	
		if (0 == bufInt.GetLength())
			bufInt = _T("0");
	
		*pstrNumber = bufInt;
		return TRUE;
	
	}
	else{
	
		nInd2 = str.Find(_T("."), nInd+1);
		if (-1 == nInd2){
			//целая и дробная части
				
			for (int i=0; i<nInd; i++){
	
				symbol = str.GetAt(i);
				bSignificant = fnItIsSignificantNumber(symbol);
	
				if (bHaveSignificantSymbol){
	
					if (!bSignificant)
						return FALSE;	//введены лишние символы
	
				}
				else{
	
					if (bSignificant){
						bHaveSignificantSymbol = true;
					}
					else{
						continue;
					};
	
				};
	
				bufInt += symbol;
	
			};
	
			if (0 == bufInt.GetLength())
				bufInt = _T("0");
	
			bHaveSignificantSymbol = false;
	
			for (int i=str.GetLength()-1; i>nInd; i--){
	
				symbol = str.GetAt(i);
				bSignificant = fnItIsSignificantNumber(symbol);
	
				if (bHaveSignificantSymbol){
	
					if (!bSignificant)
						return FALSE;	//введены лишние символы
	
				}
				else{
	
					if (bSignificant){
						bHaveSignificantSymbol = true;
					}
					else{
						continue;
					};
	
				};
	
				bufFract = symbol + bufFract;
	
			};
	
			if (0 == bufFract.GetLength())
				bufFract = _T("0");
	
			*pstrNumber = bufInt + _T(".") + bufFract;
			return TRUE;
	
		}
		else{
			//несколько разделителей дробной и целой частей - ошибка
			return FALSE;
		};
	
	};
	
	
		
	return TRUE;
}


BOOL fnItIsSignificantNumber(TCHAR symbol)
{
	CString significantAlphabet = _T("123456789");
	
	if (significantAlphabet.Find(symbol) > -1)
		return TRUE;
	else
		return FALSE;
}


void CSiDDlg::DestroyPhaseDiagWnd()
{
	delete pPhaseDiagWnd;
	pPhaseDiagWnd = 0;
}


void CSiDDlg::RedrawGraphs()
{
	if (pPhaseDiagWnd)
		pPhaseDiagWnd->UpdateGraph();

	if (pPotDiagWnd)
		pPotDiagWnd->UpdateGraph();

	if (pPMapDiagWnd)
		pPMapDiagWnd->UpdateGraph();

	++m_nE0RedrawCounter;
	if (m_nE0RedrawCounter == m_nE0RedrawMaxCount)
	{
		m_nE0RedrawCounter = 0;
		if (pEFieldWnd)
			pEFieldWnd->UpdateGraph();
		if (pMiddlePotWnd)
			pMiddlePotWnd->UpdateGraph();
	};
}


void CSiDDlg::OnTimer(UINT_PTR nIDEvent)
{
	eventTimer.SetEvent();

	CDialogEx::OnTimer(nIDEvent);
}


void CSiDDlg::OnBnClickedPotdiag()
{
	if (m_chkPotDiag.GetCheck()){
		//отобразить окно распределения потенциала
		if (!pPotDiagWnd)
		{
			pPotDiagWnd = new CPotDiagWnd();
			pPotDiagWnd->Create(IDD_POTDIAGWND, GetDesktopWindow());
			pPotDiagWnd->MoveWindow(10, 30, 450, 350);
			if (m_bSimIsRun != 1)
				pPotDiagWnd->SetEnableSaveButton(1);
			pPotDiagWnd->ShowWindow(SW_SHOW);
		}
	}
	else{
		//закрыть окно фазового портрета, если оно открыто
		if (pPotDiagWnd)
		{
			pPotDiagWnd->CloseWindow();
			DestroyPotDiagWnd();
		}
	}
}


void CSiDDlg::DestroyPotDiagWnd()
{
	delete pPotDiagWnd;
	pPotDiagWnd = 0;
}


void CSiDDlg::DestroyPMapDiagWnd()
{
	delete pPMapDiagWnd;
	pPMapDiagWnd = 0;
}

void CSiDDlg::OnBnClickedPmap()
{
	if (m_chkPMap.GetCheck()){
		//отобразить окно фазового портрета
		if (!pPMapDiagWnd)
		{
			pPMapDiagWnd = new CPoincareMap();
			pPMapDiagWnd->Create(IDD_POINCAREMAP, GetDesktopWindow());
			pPMapDiagWnd->MoveWindow(10, 50, 350, 350);
			if (m_bSimIsRun != 1)
				pPMapDiagWnd->SetEnableSaveButton(1);
			pPMapDiagWnd->ShowWindow(SW_SHOW);
		}
	}
	else{
		//закрыть окно фазового портрета, если оно открыто
		if (pPMapDiagWnd)
		{
			pPMapDiagWnd->CloseWindow();
			DestroyPMapDiagWnd();
		}
	}
}


void CSiDDlg::OnClose()
{
	thePierceDiode->SetRunFlag(false);
	eventTimer.SetEvent();

	CDialogEx::OnClose();
}


void CSiDDlg::DestroyEFieldWnd()
{
	delete pEFieldWnd;
	pEFieldWnd = 0;
}

void CSiDDlg::OnBnClickedEfield()
{
	if (m_chkEField.GetCheck()){
		//отобразить окно графика эл. поля
		if (!pEFieldWnd)
		{
			pEFieldWnd = new CEFieldDyn();
			pEFieldWnd->Create(IDD_EFIELDDYN, GetDesktopWindow());
			pEFieldWnd->MoveWindow(10, 70, 550, 350);
			if (m_bSimIsRun != 1)
				pEFieldWnd->SetEnableSaveButton(1);
			pEFieldWnd->ShowWindow(SW_SHOW);
		}
	}
	else{
		//закрыть окно графика эл. поля, если оно открыто
		if (pEFieldWnd)
		{
			pEFieldWnd->CloseWindow();
			DestroyEFieldWnd();
		}
	}
}


void CSiDDlg::OnBnClickedMiddlepot()
{
	if (m_chkMiddlePot.GetCheck()){
		if (!pMiddlePotWnd)
		{
			pMiddlePotWnd = new CMiddlePot();
			pMiddlePotWnd->Create(IDD_MIDDLEPOT, GetDesktopWindow());
			pMiddlePotWnd->MoveWindow(10, 90, 550, 350);
			if (m_bSimIsRun != 1)
				pMiddlePotWnd->SetEnableSaveButton(1);
			pMiddlePotWnd->ShowWindow(SW_SHOW);
		}
	}
	else{
		if (pMiddlePotWnd)
		{
			pMiddlePotWnd->CloseWindow();
			DestroyMiddlePotWnd();
		}
	}
}


void CSiDDlg::DestroyMiddlePotWnd()
{
	delete pMiddlePotWnd;
	pMiddlePotWnd = 0;
}