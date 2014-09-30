// MiddlePot.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "MiddlePot.h"
#include "afxdialogex.h"

#include "SiDDlg.h"

// CMiddlePot dialog

IMPLEMENT_DYNAMIC(CMiddlePot, CDialogEx)

CMiddlePot::CMiddlePot(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMiddlePot::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMiddlePot::~CMiddlePot()
{
	DestroyWindow();
}

void CMiddlePot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MIDDLEPOTGRAPH, m_stcGraph);
	DDX_Control(pDX, IDC_PRINTMIDDLEPOTPMAP, m_chkPrintMPPMap);
}


BEGIN_MESSAGE_MAP(CMiddlePot, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SAVEMIDDLEPOT, &CMiddlePot::OnBnClickedBtnSavemiddlepot)
	ON_BN_CLICKED(IDC_BTN_SAVETXTMIDDLEPOT, &CMiddlePot::OnBnClickedBtnSavetxtmiddlepot)
	ON_BN_CLICKED(IDC_PRINTMIDDLEPOTPMAP, &CMiddlePot::OnBnClickedPrintmiddlepotpmap)
END_MESSAGE_MAP()


// CMiddlePot message handlers


void CMiddlePot::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);

	//отбивка от краев для размещения элементов диалога - по 7 пикселей
	int nSpace = 11;
	//высота кнопки тулбара - 17 пикселей
	int nToolbarHeight = 28;

	CRect newRect;
	newRect.top = 2 * nSpace + nToolbarHeight;
	newRect.left = nSpace;
	newRect.bottom = rect.bottom - nSpace;
	newRect.right = rect.right - nSpace;

	m_stcGraph.MoveWindow(&newRect);
	m_stcGraph.Invalidate();
}


void CMiddlePot::OnOK()
{
	//CDialogEx::OnOK();
}


void CMiddlePot::SetEnableSaveButton(BOOL set)
{
	GetDlgItem(IDC_BTN_SAVEMIDDLEPOT)->EnableWindow(set);
	GetDlgItem(IDC_BTN_SAVETXTMIDDLEPOT)->EnableWindow(set);
}


void CMiddlePot::InitializeYAxe()
{
	m_stcGraph.InitializeYAxe();
}


void CMiddlePot::UpdateGraph()
{
	m_stcGraph.Invalidate();
}


void CMiddlePot::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkMiddlePot.SetCheck(0);
	pMainDlg->DestroyMiddlePotWnd();
}


BOOL CMiddlePot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CButton *pBtn = (CButton*)this->GetDlgItem(IDC_BTN_SAVEMIDDLEPOT);
	HANDLE hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVESCREEN), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	pBtn = (CButton*)this->GetDlgItem(IDC_BTN_SAVETXTMIDDLEPOT);
	hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVETXTDATA), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	return TRUE;
}


void CMiddlePot::OnBnClickedBtnSavemiddlepot()
{
	CFileDialog dialog(FALSE,
		CString(_T("png")),
		CString(_T("Потенциал в середине диода")),
		6UL,
		_T("PNG (*.png)|*.png|JPG (*.jpg)|*.jpg; *.jpeg|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
		this);

	if (dialog.DoModal() == IDOK)
	{

		CoordParamStruct CPstruct = m_stcGraph.GetCoordParams();

		int width = CPstruct.xVE;
		int height = CPstruct.yVE;

		Gdiplus::Graphics g(this->GetSafeHwnd());
		Gdiplus::Bitmap memBm(width, height, &g);
		Gdiplus::Graphics memG(&memBm);

		//////////////////////////////////////////////////////////////////
		m_stcGraph.PaintGraph(&memG, &CPstruct);

		CLSID clsid;

		//dialog.m_ofn.lpstrFile
		if (dialog.m_ofn.nFilterIndex == 2){
			//jpg
			GetEncoderClsid(L"image/jpeg", &clsid);
			memBm.Save(dialog.m_ofn.lpstrFile, &clsid, NULL);
		}
		else if (dialog.m_ofn.nFilterIndex == 3){
			//bmp
			GetEncoderClsid(L"image/bmp", &clsid);
			memBm.Save(dialog.m_ofn.lpstrFile, &clsid, NULL);
		}
		else{
			GetEncoderClsid(L"image/png", &clsid);
			memBm.Save(dialog.m_ofn.lpstrFile, &clsid, NULL);
		};

	};
}


void CMiddlePot::OnBnClickedBtnSavetxtmiddlepot()
{
	CFileDialog dialog(FALSE,
		CString(_T("txt")),
		CString(_T("Потенциал в середине диода")),
		6UL,
		_T("txt (*.txt)|*.txt|All Files (*.*)|*.*||"),
		this);

	if (dialog.DoModal() == IDOK)
	{
		errno_t err;
		FILE *pFile;
		err = _wfopen_s(&pFile, (LPCTSTR)dialog.m_ofn.lpstrFile, _T("w"));
		if (0 == err)
		{
			m_stcGraph.PrintFile(pFile);
		}
		else
		{
			AfxMessageBox(_T("Ошибка при открытии файла"));
		};

		fclose(pFile);
	};
}


void CMiddlePot::OnBnClickedPrintmiddlepotpmap()
{
	m_stcGraph.SetPrintPoincareFlag(m_chkPrintMPPMap.GetCheck());
	m_stcGraph.InitializeYAxe();
	m_stcGraph.Invalidate();
}
