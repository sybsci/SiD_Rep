// GraphControl.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "GraphControl.h"



extern PlotStruct PlotData;
CRITICAL_SECTION crS;

// CGraphControl

IMPLEMENT_DYNAMIC(CGraphControl, CStatic)

CGraphControl::CGraphControl()
{
	InitializeCriticalSection(&crS);

	InitializeYAxe();
}

CGraphControl::~CGraphControl()
{
}


BEGIN_MESSAGE_MAP(CGraphControl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CGraphControl message handlers




void CGraphControl::OnPaint()
{
	CPaintDC dc(this);

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
	
	int width = CPstruct.xVE;
	int height = CPstruct.yVE;

	Gdiplus::Graphics g(this->GetSafeHwnd());
	Gdiplus::Bitmap memBm(width, height, &g);
	Gdiplus::Graphics memG(&memBm);

	//////////////////////////////////////////////////////////////////
	PaintGraph(&memG, &CPstruct);
	//////////////////////////////////////////////////////////////////
	
	g.DrawImage(&memBm, 0, 0, 0, 0, width, height, Gdiplus::UnitPixel);

}


void CGraphControl::InitializeYAxe()
{
	m_fMaxVel = 1.0;
	m_fMinVel = 0.0;

	m_nMaxVel = 10;
	m_nMinVel = 0;

	m_vecYAxe = { _T("0"), _T("0,5"), _T("1") };
}


CString CGraphControl::fnGetTickLabelText(int _val)
{

	CString buf(_T(""));

	buf.Format(_T("%d"), _val);

	if (0 == _val){

	}
	else if (_val < 0){

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


CoordParamStruct CGraphControl::GetCoordParams()
{
	CoordParamStruct CPstruct;

	CRect rect;
	GetClientRect(&rect);

	CPstruct.xVE = rect.Width();
	CPstruct.yVE = rect.Height();

	CPstruct.minPos = 0.;
	CPstruct.maxPos = 1.;
	CPstruct.minVel = (float)m_fMinVel;	// -1.5; 
	CPstruct.maxVel = (float)m_fMaxVel;	// 2.5;

	double yLength = m_fMaxVel - m_fMinVel;
	CPstruct.nGrades = (int)(yLength / 0.5) + 1;
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



void CGraphControl::PaintGraph(Gdiplus::Graphics* pMemG, CoordParamStruct* pCPstruct)
{

	pMemG->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	pMemG->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	int width = pCPstruct->xVE;
	int height = pCPstruct->yVE;

	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255));
	pMemG->Clear(Gdiplus::Color(255, 255, 255));
	Gdiplus::Pen pen(Gdiplus::Color(159, 159, 159));
	pMemG->DrawRectangle(&pen, 0, 0, width - 1, height - 1);

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
		tmp = pCPstruct->edgingTop + (int)((float)i * pCPstruct->VelGridStep * pCPstruct->fPixPerY);
		y = tmp;
		pMemG->DrawLine(&pen, pCPstruct->edgingLeft, y, width - pCPstruct->edgingRight, y);
		pMemG->DrawString(m_vecYAxe.at(m_vecYAxe.size() - 1 - i), -1, &font,
			Gdiplus::RectF((float)pCPstruct->edgingLeft - 40.f, (float)(y - 8), 35.f, 16.f),
			&strFormat, &brush);
	};

	//сетка и подписи по x
	for (int i = 1; i<5; ++i){
		x = pCPstruct->edgingLeft + (int)((float)i * pCPstruct->PosGridStep * pCPstruct->fPixPerX);
		pMemG->DrawLine(&pen, x, pCPstruct->yMin, x, pCPstruct->yMax);
		pMemG->DrawString(xMark[i], -1, &font, Gdiplus::PointF((float)(x + 4), (float)pCPstruct->edgingTop + pCPstruct->logOriginY * pCPstruct->fPixPerY + 4.f), &brush);
	};


	Gdiplus::Pen axePen(Gdiplus::Color(0, 0, 0), 2.f);
	pMemG->DrawLine(&axePen, pCPstruct->xMin, pCPstruct->edgingTop + (int)(pCPstruct->logOriginY * pCPstruct->fPixPerY), pCPstruct->xMax, pCPstruct->edgingTop + (int)(pCPstruct->logOriginY * pCPstruct->fPixPerY));
	pMemG->DrawLine(&axePen, pCPstruct->xMin, pCPstruct->yMin, pCPstruct->xMin, pCPstruct->yMax);

	Gdiplus::FontFamily fontFamilyGeorgia(_T("Georgia"));
	Gdiplus::Font fontItalic(&fontFamilyGeorgia, 24, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);

	strFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
	strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);

	pMemG->DrawString(_T("v"), -1, &fontItalic,
		Gdiplus::RectF((float)pCPstruct->edgingLeft - 20.f, 0.f, 40.f, (float)pCPstruct->edgingTop),
		&strFormat, &brush);


	strFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	strFormat.SetAlignment(Gdiplus::StringAlignmentNear);

	pMemG->DrawString(_T("x"), -1, &fontItalic,
		Gdiplus::RectF(pCPstruct->xMax + 20.f, (int)pCPstruct->edgingTop + pCPstruct->logOriginY * pCPstruct->fPixPerY - 15.f, 40.f, 40.f),
		&strFormat, &brush);


	pen.SetColor(Gdiplus::Color(0, 0, 0));
	CPoint point;

	//Gdiplus::SolidBrush alphabrush(Gdiplus::Color(30, 80, 80, 80));

	EnterCriticalSection(&crS);
	for (int i = 0; i<PlotData.pNumber; ++i){
		if (PlotData.arrPos[i] >= pCPstruct->minPos && PlotData.arrPos[i] <= pCPstruct->maxPos && PlotData.arrVel[i] >= pCPstruct->minVel && PlotData.arrVel[i] <= pCPstruct->maxVel){
			point.x = pCPstruct->xMin + (int)(PlotData.arrPos[i] * pCPstruct->fPixPerX);
			point.y = pCPstruct->edgingTop + (int)((pCPstruct->logOriginY - PlotData.arrVel[i])*pCPstruct->fPixPerY);

			//pMemG->FillEllipse(&alphabrush, point.x - 1, point.y - 1 , 5, 5);
			pMemG->FillEllipse(&brush, point.x, point.y, 3, 3);

		};
	};
	LeaveCriticalSection(&crS);

}
