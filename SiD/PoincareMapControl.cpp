// PoincareMapControl.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "PoincareMapControl.h"

extern PlotStruct PlotData;
CRITICAL_SECTION critSect;

// CPoincareMapControl

IMPLEMENT_DYNAMIC(CPoincareMapControl, CStatic)

CPoincareMapControl::CPoincareMapControl()
{
	InitializeCriticalSection(&critSect);

	InitializeAxes();
}

CPoincareMapControl::~CPoincareMapControl()
{
}


BEGIN_MESSAGE_MAP(CPoincareMapControl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPoincareMapControl message handlers


void CPoincareMapControl::OnPaint()
{
	CPaintDC dc(this); 

	/////////////////////////////////////////////////////////////////

	bool bChangeAxe = false;

	if ((PlotData.fMaxPot > m_fMaxPot) /*|| (PlotData.fMinPot < m_fMinPot)*/) {
		bChangeAxe = true;
	};

	if (bChangeAxe){

		while (PlotData.fMaxPot > m_fMaxPot){
			m_fMaxPot += 0.5;
			m_nMaxPot += 5;
			m_vecYAxe.push_back(fnGetTickLabelText(m_nMaxPot));
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


CoordParamStruct CPoincareMapControl::GetCoordParams()
{
	CoordParamStruct CPstruct;

	CRect rect;
	GetClientRect(&rect);

	CPstruct.xVE = rect.Width();
	CPstruct.yVE = rect.Height();

	CPstruct.minPos = (float)m_fMinPot;
	CPstruct.maxPos = (float)m_fMaxPot;
	CPstruct.minPot = (float)m_fMinPot;	// -1.5; 
	CPstruct.maxPot = (float)m_fMaxPot;	// 2.5;

	double yLength = m_fMaxPot - m_fMinPot;
	CPstruct.nGrades = (int)(yLength / 0.5) + 1;

	//в пикселях постоянную отбивку от края
	CPstruct.edgingLeft = 40;	// 0.1f;
	CPstruct.edgingRight = 50;	// 0.1f;
	CPstruct.edgingBottom = 30;	// 0.2f;
	CPstruct.edgingTop = 30;	// 0.f;

	CPstruct.fPixPerX = (float)(CPstruct.xVE - CPstruct.edgingLeft - CPstruct.edgingRight) / (CPstruct.maxPos - CPstruct.minPos);
	CPstruct.fPixPerY = (float)(CPstruct.yVE - CPstruct.edgingTop - CPstruct.edgingBottom) / (CPstruct.maxPot - CPstruct.minPot);

	CPstruct.logOriginX = 0.f;
	CPstruct.logOriginY = (float)m_fMaxPot;

	CPstruct.PosGridStep = 0.5;
	CPstruct.VelGridStep = 0.5;

	CPstruct.xMin = CPstruct.edgingLeft;
	CPstruct.xMax = CPstruct.edgingLeft + (int)((CPstruct.maxPos - CPstruct.minPos) * CPstruct.fPixPerX);

	CPstruct.yMin = CPstruct.edgingTop + (int)((CPstruct.maxPot - CPstruct.minPot) * CPstruct.fPixPerY);
	CPstruct.yMax = CPstruct.edgingTop;


	return CPstruct;
}


void CPoincareMapControl::PaintGraph(Gdiplus::Graphics* pMemG, CoordParamStruct* pCPstruct)
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
	for (unsigned int i = 0; i<m_vecYAxe.size(); ++i){
		tmp = pCPstruct->edgingLeft + (int)((float)i * pCPstruct->VelGridStep * pCPstruct->fPixPerX);
		x = tmp;
		pMemG->DrawLine(&pen, x, pCPstruct->yMin, x, pCPstruct->yMax);
		pMemG->DrawString(m_vecYAxe.at(i), -1, &font,
			Gdiplus::PointF((float)(x - 10), (float)pCPstruct->edgingTop + pCPstruct->logOriginY * pCPstruct->fPixPerY + 4.f),
			&brush);
	};

	//биссектриса
	pMemG->DrawLine(&pen, 
		pCPstruct->edgingLeft, 
		pCPstruct->edgingTop + (int)(pCPstruct->logOriginY * pCPstruct->fPixPerY), 
		width - pCPstruct->edgingRight, 
		pCPstruct->edgingTop);


	Gdiplus::Pen axePen(Gdiplus::Color(0, 0, 0), 2.f);
	pMemG->DrawLine(&axePen, pCPstruct->xMin, pCPstruct->edgingTop + (int)(pCPstruct->logOriginY * pCPstruct->fPixPerY), pCPstruct->xMax, pCPstruct->edgingTop + (int)(pCPstruct->logOriginY * pCPstruct->fPixPerY));
	pMemG->DrawLine(&axePen, pCPstruct->xMin, pCPstruct->yMin, pCPstruct->xMin, pCPstruct->yMax);

	Gdiplus::FontFamily fontFamilySymbol(_T("Symbol"));
	Gdiplus::Font fontItalic(&fontFamilySymbol, 24, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);

	strFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
	strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);

	pMemG->DrawString(_T("f"), -1, &fontItalic,
		Gdiplus::RectF((float)pCPstruct->edgingLeft - 20.f, 0.f, 40.f, (float)pCPstruct->edgingTop),
		&strFormat, &brush);


	/*Gdiplus::FontFamily fontFamilyGeorgia(_T("Georgia"));
	Gdiplus::Font fontItalicX(&fontFamilyGeorgia, 24, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);*/

	strFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	strFormat.SetAlignment(Gdiplus::StringAlignmentNear);

	pMemG->DrawString(_T("f"), -1, &fontItalic,
		Gdiplus::RectF(pCPstruct->xMax + 25.f, (int)pCPstruct->edgingTop + pCPstruct->logOriginY * pCPstruct->fPixPerY - 15.f, 40.f, 40.f),
		&strFormat, &brush);

	pen.SetColor(Gdiplus::Color(0, 0, 0));
	CPoint point;


	EnterCriticalSection(&critSect);
	for (unsigned int i = 1; i < PlotData.nDotNumber; ++i)
	{
		if (PlotData.PMap[i] <= pCPstruct->maxPot && PlotData.PMap[i] >= pCPstruct->minPot)
		{
			point.x = pCPstruct->xMin + (int)(PlotData.PMap[i] * pCPstruct->fPixPerX);
			point.y = pCPstruct->edgingTop + (int)((pCPstruct->logOriginY - PlotData.PMap[i-1])*pCPstruct->fPixPerY) - 3;
			pMemG->FillEllipse(&brush, point.x, point.y, 4, 4);
		};
	};
	LeaveCriticalSection(&critSect);
}


void CPoincareMapControl::InitializeAxes()
{
	m_fMaxPot = 0.5;
	m_fMinPot = 0.0;

	m_nMaxPot = 5;
	m_nMinPot = 0;

	m_vecYAxe = { _T("0"), _T("0,5") };
}


CString CPoincareMapControl::fnGetTickLabelText(int _val)
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
