// EFieldDyn.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "EFieldDyn.h"
#include "afxdialogex.h"

#include "SiDDlg.h"


// CEFieldDyn dialog

IMPLEMENT_DYNAMIC(CEFieldDyn, CDialogEx)

CEFieldDyn::CEFieldDyn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEFieldDyn::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEFieldDyn::~CEFieldDyn()
{
	DestroyWindow();
}

void CEFieldDyn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFIELDGRAPH, m_stcGraph);
	DDX_Control(pDX, IDC_PRINTE0PMAP, m_chkPrintE0PMap);
}


BEGIN_MESSAGE_MAP(CEFieldDyn, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SAVEEFIELD, &CEFieldDyn::OnBnClickedBtnSaveefield)
	ON_BN_CLICKED(IDC_BTN_SAVETXTEFIELD, &CEFieldDyn::OnBnClickedBtnSavetxtefield)
	ON_BN_CLICKED(IDC_PRINTE0PMAP, &CEFieldDyn::OnBnClickedPrinte0pmap)
END_MESSAGE_MAP()


// CEFieldDyn message handlers


void CEFieldDyn::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);

	//������� �� ����� ��� ���������� ��������� ������� - �� 7 ��������
	int nSpace = 11;
	//������ ������ ������� - 17 ��������
	int nToolbarHeight = 28;

	CRect newRect;
	newRect.top = 2 * nSpace + nToolbarHeight;
	newRect.left = nSpace;
	newRect.bottom = rect.bottom - nSpace;
	newRect.right = rect.right - nSpace;

	m_stcGraph.MoveWindow(&newRect);
	m_stcGraph.Invalidate();
}


void CEFieldDyn::OnOK()
{
	//CDialogEx::OnOK();
}


void CEFieldDyn::SetEnableSaveButton(BOOL set)
{
	GetDlgItem(IDC_BTN_SAVEEFIELD)->EnableWindow(set);
	GetDlgItem(IDC_BTN_SAVETXTEFIELD)->EnableWindow(set);
}


void CEFieldDyn::InitializeYAxe()
{
	m_stcGraph.InitializeYAxe();
}


void CEFieldDyn::UpdateGraph()
{
	m_stcGraph.Invalidate();
}



void CEFieldDyn::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkEField.SetCheck(0);
	pMainDlg->DestroyEFieldWnd();
}


BOOL CEFieldDyn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CButton *pBtn = (CButton*)this->GetDlgItem(IDC_BTN_SAVEEFIELD);
	HANDLE hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVESCREEN), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	pBtn = (CButton*)this->GetDlgItem(IDC_BTN_SAVETXTEFIELD);
	hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVETXTDATA), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	return TRUE;
}


void CEFieldDyn::OnBnClickedBtnSaveefield()
{
	CFileDialog dialog(FALSE,
		CString(_T("png")),
		CString(_T("��. ����")),
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


void CEFieldDyn::OnBnClickedBtnSavetxtefield()
{
	CFileDialog dialog(FALSE,
		CString(_T("txt")),
		CString(_T("��. ����")),
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
			AfxMessageBox(_T("������ ��� �������� �����"));
		};

		fclose(pFile);
	};
}


void CEFieldDyn::OnBnClickedPrinte0pmap()
{
	m_stcGraph.SetPrintPoincareFlag(m_chkPrintE0PMap.GetCheck());
	m_stcGraph.InitializeYAxe();
	m_stcGraph.Invalidate();
}
