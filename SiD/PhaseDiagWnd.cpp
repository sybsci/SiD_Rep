// PhaseDiagWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "PhaseDiagWnd.h"
#include "afxdialogex.h"

#include "SiDDlg.h"


// CPhaseDiagWnd dialog

IMPLEMENT_DYNAMIC(CPhaseDiagWnd, CDialogEx)

CPhaseDiagWnd::CPhaseDiagWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPhaseDiagWnd::IDD, pParent)
{
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
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SAVESCREEN, &CPhaseDiagWnd::OnBnClickedBtnSavescreen)
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

	return TRUE;
}


void CPhaseDiagWnd::OnOK()
{
	//CDialogEx::OnOK();
}


void CPhaseDiagWnd::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkPhaseDiag.SetCheck(0);
	pMainDlg->DestroyPhaseDiagWnd();
}


void CPhaseDiagWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//изменим размер график контрола 

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


void CPhaseDiagWnd::InitializeYAxe()
{
	m_stcGraph.InitializeYAxe();
}


void CPhaseDiagWnd::UpdateGraph()
{
	m_stcGraph.Invalidate();
}


void CPhaseDiagWnd::OnBnClickedBtnSavescreen()
{
	CFileDialog dialog(FALSE,
					CString(_T("png")),
					CString(_T("Фазовый портрет")),
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


void CPhaseDiagWnd::SetEnableSaveButton(BOOL set)
{
	GetDlgItem(IDC_BTN_SAVESCREEN)->EnableWindow(set);
}
