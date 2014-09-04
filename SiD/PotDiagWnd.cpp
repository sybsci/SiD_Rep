// PotDiagWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "PotDiagWnd.h"
#include "afxdialogex.h"

#include "SiDDlg.h"



// CPotDiagWnd dialog

IMPLEMENT_DYNAMIC(CPotDiagWnd, CDialogEx)

CPotDiagWnd::CPotDiagWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPotDiagWnd::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPotDiagWnd::~CPotDiagWnd()
{
	DestroyWindow();
}


BOOL CPotDiagWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	CButton *pBtn = (CButton*)this->GetDlgItem(IDC_BTN_SAVEPOTSCREEN);
	HANDLE hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVESCREEN), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	return TRUE;
}


void CPotDiagWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POTGRAPH, m_stcGraph);
}


BEGIN_MESSAGE_MAP(CPotDiagWnd, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVEPOTSCREEN, &CPotDiagWnd::OnBnClickedBtnSavepotscreen)
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CPotDiagWnd::SetEnableSaveButton(BOOL set)
{
	GetDlgItem(IDC_BTN_SAVEPOTSCREEN)->EnableWindow(set);
}




void CPotDiagWnd::OnBnClickedBtnSavepotscreen()
{
	CFileDialog dialog(FALSE,
		CString(_T("png")),
		CString(_T("������������� ����������")),
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


void CPotDiagWnd::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkPotDiag.SetCheck(0);
	pMainDlg->DestroyPotDiagWnd();
}


void CPotDiagWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//������� ������ ������ �������� 

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





void CPotDiagWnd::OnOK()
{
	//CDialogEx::OnOK();
}


void CPotDiagWnd::InitializeYAxe()
{
	m_stcGraph.InitializeYAxe();
}


void CPotDiagWnd::UpdateGraph()
{
	m_stcGraph.Invalidate();
}