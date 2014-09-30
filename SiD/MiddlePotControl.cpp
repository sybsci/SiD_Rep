// MiddlePotControl.cpp : implementation file
//

#include "stdafx.h"
#include "SiD.h"
#include "MiddlePotControl.h"

extern PlotStruct PlotData;
CRITICAL_SECTION criSect;

// CMiddlePotControl

IMPLEMENT_DYNAMIC(CMiddlePotControl, CStatic)

CMiddlePotControl::CMiddlePotControl()
{
	m_bPrintPoincare = false;
		
	InitializeCriticalSection(&criSect);

	InitializeYAxe();
}

CMiddlePotControl::~CMiddlePotControl()
{
}


BEGIN_MESSAGE_MAP(CMiddlePotControl, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CMiddlePotControl::InitializeYAxe()
{
	m_fMaxPot = 0.5;
	m_fMinPot = 0.0;

	m_nMaxPot = 5;
	m_nMinPot = 0;

	m_vecYAxe = { _T("0"), _T("0,5") };
}


CString CMiddlePotControl::fnGetTickLabelText(int _val)
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


void CMiddlePotControl::OnPaint()
{
	CPaintDC dc(this);
	
	/////////////////////////////////////////////////////////////////

	bool bChangeAxe = false;

	double fmax, fmin;

	if (m_bPrintPoincare)
	{
		fmax = PlotData.fMaxMP_Poin;
		fmin = PlotData.fMinMP_Poin;	
	}
	else
	{
		fmax = PlotData.fMaxMP;
		fmin = PlotData.fMinMP;	
	};

	if ((fmax > m_fMaxPot) || (fmin < m_fMinPot)) {
		bChangeAxe = true;
	};

	if (bChangeAxe)
	{
		while (fmin < m_fMinPot)
		{
			m_fMinPot -= 0.5;
			m_nMinPot -= 5;
			m_vecYAxe.insert(m_vecYAxe.begin(), fnGetTickLabelText(m_nMinPot));
		};

		while (fmax > m_fMaxPot)
		{
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


CoordParamStruct CMiddlePotControl::GetCoordParams()
{
	CoordParamStruct CPstruct;

	CRect rect;
	GetClientRect(&rect);

	CPstruct.xVE = rect.Width();
	CPstruct.yVE = rect.Height();

	CPstruct.minPos = 0.;
	CPstruct.maxPos = 10.;
	CPstruct.minPot = (float)m_fMinPot;
	CPstruct.maxPot = (float)m_fMaxPot;

	CPstruct.PosGridStep = 1.0;
	CPstruct.VelGridStep = 0.5;

	double yLength = m_fMaxPot - m_fMinPot;
	CPstruct.nGrades = (int)(yLength / (double)CPstruct.VelGridStep) + 1;

	//в пиксел€х посто€нную отбивку от кра€
	CPstruct.edgingLeft = 40;
	CPstruct.edgingRight = 50;
	CPstruct.edgingBottom = 30;
	CPstruct.edgingTop = 42;

	CPstruct.fPixPerX = (float)(CPstruct.xVE - CPstruct.edgingLeft - CPstruct.edgingRight) / (CPstruct.maxPos - CPstruct.minPos);
	CPstruct.fPixPerY = (float)(CPstruct.yVE - CPstruct.edgingTop - CPstruct.edgingBottom) / (CPstruct.maxPot - CPstruct.minPot);

	CPstruct.logOriginX = 0.f;
	CPstruct.logOriginY = (float)m_fMaxPot;

	CPstruct.xMin = CPstruct.edgingLeft;
	CPstruct.xMax = CPstruct.edgingLeft + (int)((CPstruct.maxPos - CPstruct.minPos) * CPstruct.fPixPerX);

	CPstruct.yMin = CPstruct.edgingTop + (int)((CPstruct.maxPot - CPstruct.minPot) * CPstruct.fPixPerY);
	CPstruct.yMax = CPstruct.edgingTop;


	return CPstruct;
}


void CMiddlePotControl::PaintGraph(Gdiplus::Graphics* pMemG, CoordParamStruct* pCPstruct)
{
	pMemG->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	pMemG->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	int width = pCPstruct->xVE;
	int height = pCPstruct->yVE;

	pMemG->Clear(Gdiplus::Color(255, 255, 255));
	Gdiplus::Pen pen(Gdiplus::Color(159, 159, 159));
	pMemG->DrawRectangle(&pen, 0, 0, width - 1, height - 1);

	/////////


	int nxGrades = (int)(pCPstruct->maxPos / pCPstruct->PosGridStep);
	int x, y;

	Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0));
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

	strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);

	//сетка и подписи по x
	for (int i = 1; i <= nxGrades; ++i)
	{
		x = pCPstruct->edgingLeft + (int)((float)i * pCPstruct->PosGridStep * pCPstruct->fPixPerX);
		pMemG->DrawLine(&pen, x, pCPstruct->yMin, x, pCPstruct->yMax);
	};


	Gdiplus::Pen axePen(Gdiplus::Color(0, 0, 0), 2.f);
	pMemG->DrawLine(&axePen, pCPstruct->xMin, pCPstruct->edgingTop + (int)(pCPstruct->logOriginY * pCPstruct->fPixPerY), pCPstruct->xMax, pCPstruct->edgingTop + (int)(pCPstruct->logOriginY * pCPstruct->fPixPerY));
	pMemG->DrawLine(&axePen, pCPstruct->xMin, pCPstruct->yMin, pCPstruct->xMin, pCPstruct->yMax);


	Gdiplus::Bitmap bmpE0(theApp.m_hInstance, MAKEINTRESOURCE(IDB_PHI));
	pMemG->DrawImage(&bmpE0, 35, 10);


	Gdiplus::FontFamily fontFamilyGeorgia(_T("Times New Roman"));
	Gdiplus::Font fontItalicX(&fontFamilyGeorgia, 24, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);

	strFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	strFormat.SetAlignment(Gdiplus::StringAlignmentNear);

	pMemG->DrawString(_T("t"), -1, &fontItalicX,
		Gdiplus::RectF(pCPstruct->xMax + 20.f, (int)pCPstruct->edgingTop + pCPstruct->logOriginY * pCPstruct->fPixPerY - 15.f, 40.f, 40.f),
		&strFormat, &brush);


	pen.SetColor(Gdiplus::Color(0, 0, 0));
	CPoint point;

	Gdiplus::Point point1, point2;
	long long int nLastT, nFirstT;

	axePen.SetWidth(1.f);


	EnterCriticalSection(&criSect);

	if (m_bPrintPoincare)
	{
		nLastT = PlotData.MidPot_Poin.at(0).time;
		nFirstT = nLastT - MaxSizeOfE0 * AnalizeEvery;

		for (unsigned int i = 1; i < PlotData.MidPot_Poin.size(); ++i)	// беретс€ пара точек i, i+1
		{
			if (PlotData.MidPot_Poin[i].value <= pCPstruct->maxPot && PlotData.MidPot_Poin[i].value >= pCPstruct->minPot)
			{
				if (PlotData.MidPot_Poin[i].time < nFirstT)
					break;

				point1.X = pCPstruct->xMin + (int)(((pCPstruct->maxPos - pCPstruct->minPos) * ((float)(PlotData.MidPot_Poin[i - 1].time - nFirstT)) / ((float)(nLastT - nFirstT))) * pCPstruct->fPixPerX);
				point1.Y = pCPstruct->edgingTop + (int)((pCPstruct->logOriginY - PlotData.MidPot_Poin[i - 1].value) * pCPstruct->fPixPerY);
				point2.X = pCPstruct->xMin + (int)(((pCPstruct->maxPos - pCPstruct->minPos) * ((float)(PlotData.MidPot_Poin[i].time - nFirstT)) / ((float)(nLastT - nFirstT))) * pCPstruct->fPixPerX);
				point2.Y = pCPstruct->edgingTop + (int)((pCPstruct->logOriginY - PlotData.MidPot_Poin[i].value) * pCPstruct->fPixPerY);

				pMemG->DrawLine(&axePen, point1, point2);

				pMemG->FillEllipse(&brush, point2.X - 3, point2.Y - 3, 5, 5);
			};
		};
	}
	else
	{
		nLastT = PlotData.MidPot.at(0).time;
		nFirstT = nLastT - MaxSizeOfE0 * AnalizeEvery;

		for (unsigned int i = 1; i < PlotData.MidPot.size(); ++i)	// беретс€ пара точек i, i+1
		{
			if (PlotData.MidPot[i].value <= pCPstruct->maxPot && PlotData.MidPot[i].value >= pCPstruct->minPot)
			{
				

				point1.X = pCPstruct->xMin + (int)(((pCPstruct->maxPos - pCPstruct->minPos) * ((float)(PlotData.MidPot[i - 1].time - nFirstT)) / ((float)(nLastT - nFirstT))) * pCPstruct->fPixPerX);
				point1.Y = pCPstruct->edgingTop + (int)((pCPstruct->logOriginY - PlotData.MidPot[i - 1].value) * pCPstruct->fPixPerY);
				point2.X = pCPstruct->xMin + (int)(((pCPstruct->maxPos - pCPstruct->minPos) * ((float)(PlotData.MidPot[i].time - nFirstT)) / ((float)(nLastT - nFirstT))) * pCPstruct->fPixPerX);
				point2.Y = pCPstruct->edgingTop + (int)((pCPstruct->logOriginY - PlotData.MidPot[i].value) * pCPstruct->fPixPerY);

				pMemG->DrawLine(&axePen, point1, point2);
			};
		};
	};

	LeaveCriticalSection(&criSect);

}

void CMiddlePotControl::SetPrintPoincareFlag(BOOL param)
{
	m_bPrintPoincare = param;
}


void CMiddlePotControl::PrintFile(FILE* pFile)
{
	CoordParamStruct CPstruct = GetCoordParams();

	EnterCriticalSection(&criSect);

	if (m_bPrintPoincare)
	{
		for (unsigned int i = 1; i < PlotData.MidPot_Poin.size(); ++i)	// беретс€ пара точек i, i+1
		{
			if (PlotData.MidPot_Poin[i].value <= CPstruct.maxPot && PlotData.MidPot_Poin[i].value >= CPstruct.minPot)
			{
				fprintf_s(pFile, "%20.15f\n", PlotData.MidPot_Poin[i].value);
			};
		};
	}
	else
	{
		for (unsigned int i = 1; i < PlotData.E0.size(); ++i)	// беретс€ пара точек i, i+1
		{
			if (PlotData.MidPot[i].value <= CPstruct.maxPot && PlotData.MidPot[i].value >= CPstruct.minPot)
			{
				fprintf_s(pFile, "%20.15f\n", PlotData.MidPot[i].value);
			};
		};
	};

	LeaveCriticalSection(&criSect);
}