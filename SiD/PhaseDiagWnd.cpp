// PhaseDiagWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "PhaseDiagWnd.h"
#include "afxdialogex.h"

#include "SiDDlg.h"

extern PlotStruct PlotData;
CRITICAL_SECTION crS;

// CPhaseDiagWnd dialog

IMPLEMENT_DYNAMIC(CPhaseDiagWnd, CDialogEx)

CPhaseDiagWnd::CPhaseDiagWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPhaseDiagWnd::IDD, pParent)
{
	InitializeCriticalSection(&crS);

	InitializeYAxe();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPhaseDiagWnd::~CPhaseDiagWnd()
{
	DestroyWindow();
}

void CPhaseDiagWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPhaseDiagWnd, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SIZING()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
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


void CPhaseDiagWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	/////////////////////////////////////////////////////////////////
	
	bool bChangeAxe = false;

	if ((PlotData.fMaxVel > m_fMaxVel) || (PlotData.fMinVel < m_fMinVel)) {
		bChangeAxe = true;
	};

	if (bChangeAxe){

		while (PlotData.fMinVel < m_fMinVel){
			m_fMinVel -= 0.5;
			m_nMinVel -= 5;
			m_vecYAxe.insert(m_vecYAxe.begin(), fnGetTickLabelText(m_nMinVel));
		};

		while (PlotData.fMaxVel > m_fMaxVel){
			m_fMaxVel += 0.5;
			m_nMaxVel += 5;
			m_vecYAxe.push_back(fnGetTickLabelText(m_nMaxVel));
		};

	};

	/////////////////////////////////////////////////////////////////

	CoordParamStruct CPstruct = GetCoordParams();
	
	int width = CPstruct.GraphRect.Width();
	int height = CPstruct.GraphRect.Height();

	Gdiplus::Graphics g(this->GetSafeHwnd());
	Gdiplus::Bitmap memBm(width, height, &g);
	Gdiplus::Graphics memG(&memBm);

	//задний фон области построения
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255));
	memG.FillRectangle(&brush, 0, 0, width - 1, height - 1);
	Gdiplus::Pen pen(Gdiplus::Color(159, 159, 159));
	memG.DrawRectangle(&pen, 0, 0, width - 1, height - 1);

	/////////

	//метки по оси абсцисс
	wchar_t* xMark[] = { L"0", L"0,25", L"0,5", L"0,75", L"1" };

	int x, y;

	brush.SetColor(Gdiplus::Color(0, 0, 0));
	Gdiplus::FontFamily fontFamily(_T("Times New Roman"));
	Gdiplus::Font font(&fontFamily, 16, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	Gdiplus::StringFormat strFormat;
	strFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	strFormat.SetAlignment(Gdiplus::StringAlignmentFar);

	int tmp;

	//сетка и подписи по y
	for (unsigned int i = 0; i<m_vecYAxe.size(); ++i){
		tmp = CPstruct.edgingTop + (int)( (float)i * CPstruct.VelGridStep * CPstruct.fPixPerY );
		y = tmp;
		memG.DrawLine(&pen, CPstruct.edgingLeft, y, width - CPstruct.edgingRight, y);
		memG.DrawString(m_vecYAxe.at(m_vecYAxe.size() - 1 - i), -1, &font,
			Gdiplus::RectF((float)CPstruct.edgingLeft - 40.f, (float)(y - 8), 35.f, 16.f),
			&strFormat, &brush);
	};

	//сетка и подписи по x
	for (int i = 1; i<5; ++i){
		x = CPstruct.edgingLeft + (int)( (float)i * CPstruct.PosGridStep * CPstruct.fPixPerX );
		memG.DrawLine(&pen, x, CPstruct.yMin, x, CPstruct.yMax);
		memG.DrawString(xMark[i], -1, &font, Gdiplus::PointF((float)(x + 4), (float)CPstruct.edgingTop + CPstruct.logOriginY * CPstruct.fPixPerY + 4.f), &brush);
	};


	Gdiplus::Pen axePen(Gdiplus::Color(0, 0, 0), 2.f);
	memG.DrawLine(&axePen, CPstruct.xMin, CPstruct.edgingTop + (int)(CPstruct.logOriginY * CPstruct.fPixPerY), CPstruct.xMax, CPstruct.edgingTop + (int)(CPstruct.logOriginY * CPstruct.fPixPerY));
	memG.DrawLine(&axePen, CPstruct.xMin, CPstruct.yMin, CPstruct.xMin, CPstruct.yMax);

	Gdiplus::FontFamily fontFamilyGeorgia(_T("Georgia"));
	Gdiplus::Font fontItalic(&fontFamilyGeorgia, 24, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);

	strFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
	strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);

	memG.DrawString(_T("v"), -1, &fontItalic,
		Gdiplus::RectF((float)CPstruct.edgingLeft - 20.f, 0.f, 40.f, (float)CPstruct.edgingTop),
		&strFormat, &brush);


	strFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	strFormat.SetAlignment(Gdiplus::StringAlignmentNear);

	memG.DrawString(_T("x"), -1, &fontItalic,
		Gdiplus::RectF(CPstruct.xMax + 20.f, (int)CPstruct.edgingTop + CPstruct.logOriginY * CPstruct.fPixPerY - 15.f, 40.f, 40.f),
		&strFormat, &brush);


	pen.SetColor(Gdiplus::Color(0, 0, 0));
	CPoint point;

	EnterCriticalSection(&crS);
	for (int i = 0; i<PlotData.pNumber; ++i){
		if (PlotData.arrPos[i] >= CPstruct.minPos && PlotData.arrPos[i] <= CPstruct.maxPos && PlotData.arrVel[i] >= CPstruct.minVel && PlotData.arrVel[i] <= CPstruct.maxVel){
			point.x = CPstruct.xMin + (int)(PlotData.arrPos[i] * CPstruct.fPixPerX);
			point.y = CPstruct.edgingTop + (int)((CPstruct.logOriginY - PlotData.arrVel[i])*CPstruct.fPixPerY);

			memG.FillEllipse(&brush, point.x, point.y, 2, 2);

		};
	};
	LeaveCriticalSection(&crS);



	g.DrawImage(&memBm, CPstruct.GraphRect.left, CPstruct.GraphRect.top, 0, 0, width, height, Gdiplus::UnitPixel);
}


void CPhaseDiagWnd::OnClose()
{
	CDialogEx::OnClose();

	CSiDDlg* pMainDlg = (CSiDDlg*)AfxGetMainWnd();
	pMainDlg->m_chkPhaseDiag.SetCheck(0);
	pMainDlg->DestroyPhaseDiagWnd();
}





void CPhaseDiagWnd::InitializeYAxe()
{
	m_fMaxVel = 1.0;
	m_fMinVel = 0.0;
	
	m_nMaxVel = 10;
	m_nMinVel = 0;
	
	m_vecYAxe = { _T("0"), _T("0,5"), _T("1") };
}


CString CPhaseDiagWnd::fnGetTickLabelText(int _val)
{

	CString buf(_T(""));

	buf.Format(_T("%d"), _val);

	if (0 == _val){

	}else if(_val < 0){

		if (buf.GetLength() == 2){

			buf.Insert(1, _T("0,"));

		}
		else{

			if (buf.GetAt(buf.GetLength() - 1) == CString(_T("0"))){

				buf = buf.Left(buf.GetLength() - 1);

			}
			else{

				buf.Insert(buf.GetLength() - 1, _T(","));

			};

		};

	}
	else{

		if (buf.GetLength() == 1){

			buf = _T("0,") + buf;

		}
		else{

			if (buf.GetAt(buf.GetLength() - 1) == CString(_T("0"))){

				buf = buf.Left(buf.GetLength() - 1);

			}
			else{

				buf.Insert(buf.GetLength() - 1, _T(","));

			};

		};

	};

	return buf;
}

CoordParamStruct CPhaseDiagWnd::GetCoordParams()
{
	CoordParamStruct CPstruct;

	CRect rect;
	GetClientRect(&rect);

	//отбивка от краев для размещения элементов диалога - по 7 пикселей
	int nSpace = 11;
	//высота кнопки тулбара - 17 пикселей
	int nToolbarHeight = 28;

	CPstruct.GraphRect.top = 2 * nSpace + nToolbarHeight;
	CPstruct.GraphRect.left = nSpace;
	CPstruct.GraphRect.bottom = rect.bottom - nSpace;
	CPstruct.GraphRect.right = rect.right - nSpace;

	CPstruct.xVE = CPstruct.GraphRect.Width();
	CPstruct.yVE = CPstruct.GraphRect.Height();

	CPstruct.minPos = 0.; 
	CPstruct.maxPos = 1.; 
	CPstruct.minVel = (float)m_fMinVel;	// -1.5; 
	CPstruct.maxVel = (float)m_fMaxVel;	// 2.5;

	double yLength = m_fMaxVel - m_fMinVel;
	CPstruct.nGrades = (int)(yLength/0.5) + 1;
	//int nFirst = (int)(m_fMinVel*10.);

	//в пикселях постоянную отбивку от края
	CPstruct.edgingLeft = 40;	// 0.1f;
	CPstruct.edgingRight = 50;	// 0.1f;
	CPstruct.edgingBottom = 30;	// 0.2f;
	CPstruct.edgingTop = 30;	// 0.f;

	CPstruct.fPixPerX = (float)(CPstruct.xVE - CPstruct.edgingLeft - CPstruct.edgingRight) / (CPstruct.maxPos - CPstruct.minPos);
	CPstruct.fPixPerY = (float)(CPstruct.yVE - CPstruct.edgingTop - CPstruct.edgingBottom) / (CPstruct.maxVel - CPstruct.minVel);

	CPstruct.logOriginX = 0.f;
	CPstruct.logOriginY = (float)m_fMaxVel;
	
	CPstruct.PosGridStep = 0.25; 
	CPstruct.VelGridStep = 0.5;

	CPstruct.xMin = CPstruct.edgingLeft;
	CPstruct.xMax = CPstruct.edgingLeft + (int)((CPstruct.maxPos - CPstruct.minPos) * CPstruct.fPixPerX);

	CPstruct.yMin = CPstruct.edgingTop + (int)((CPstruct.maxVel - CPstruct.minVel) * CPstruct.fPixPerY);
	CPstruct.yMax = CPstruct.edgingTop;


	return CPstruct;
}


void CPhaseDiagWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	//Invalidate();
}


BOOL CPhaseDiagWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialogEx::OnEraseBkgnd(pDC);
	//return 0;
}


void CPhaseDiagWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	Invalidate();
}
