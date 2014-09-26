// PoincareMap.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "PoincareMap.h"
#include "afxdialogex.h"

#include "SiDDlg.h"

// CPoincareMap dialog

IMPLEMENT_DYNAMIC(CPoincareMap, CDialogEx)

CPoincareMap::CPoincareMap(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPoincareMap::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPoincareMap::~CPoincareMap()
{
	DestroyWindow();
}

void CPoincareMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PMAPGRAPH, m_stcGraph);
}


BEGIN_MESSAGE_MAP(CPoincareMap, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SAVEPMAPSCREEN, &CPoincareMap::OnBnClickedBtnSavepmapscreen)
	ON_BN_CLICKED(IDC_BTN_SAVEPMAPTXT, &CPoincareMap::OnBnClickedBtnSavepmaptxt)
END_MESSAGE_MAP()


// CPoincareMap message handlers


BOOL CPoincareMap::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CButton *pBtn = (CButton*)this->GetDlgItem(IDC_BTN_SAVEPMAPSCREEN);
	HANDLE hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVESCREEN), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	pBtn = (CButton*)this->GetDlgItem(IDC_BTN_SAVEPMAPTXT);
	hBtnIcon = LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_SAVETXTDATA), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS);
	pBtn->SetIcon((HICON)hBtnIcon);

	return TRUE;
}


void CPoincareMap::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkPMap.SetCheck(0);
	pMainDlg->DestroyPMapDiagWnd();
}


void CPoincareMap::OnSize(UINT nType, int cx, int cy)
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


void CPoincareMap::OnOK()
{
}


void CPoincareMap::InitializeYAxe()
{
	m_stcGraph.InitializeAxes();
}


void CPoincareMap::UpdateGraph()
{
	m_stcGraph.Invalidate();
}


void CPoincareMap::SetEnableSaveButton(BOOL set)
{
	GetDlgItem(IDC_BTN_SAVEPMAPSCREEN)->EnableWindow(set);
	GetDlgItem(IDC_BTN_SAVEPMAPTXT)->EnableWindow(set);
}

void CPoincareMap::OnBnClickedBtnSavepmapscreen()
{
	CFileDialog dialog(FALSE,
		CString(_T("png")),
		CString(_T("Отображение Пуанкаре")),
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


void CPoincareMap::OnBnClickedBtnSavepmaptxt()
{
	CFileDialog dialog(FALSE,
		CString(_T("txt")),
		CString(_T("Отображение Пуанкаре")),
		6UL,
		_T("txt (*.txt)|*.txt|All Files (*.*)|*.*||"),
		this);

	if (dialog.DoModal() == IDOK)
	{
		errno_t err;
		FILE *pFile;
		std::vector<double> pmap;
		err = _wfopen_s(&pFile, (LPCTSTR)dialog.m_ofn.lpstrFile, _T("w"));
		if (0 == err)
		{
			pmap = m_stcGraph.GetPMapArray();
			for (unsigned int i = 0; i < pmap.size(); ++i)
				fprintf_s(pFile, "%20.15f\n", pmap[i]);	
		}
		else
		{
			AfxMessageBox(_T("Ошибка при открытии файла"));
		};

		fclose(pFile);

	};

}
