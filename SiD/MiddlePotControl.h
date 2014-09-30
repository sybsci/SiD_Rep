#pragma once
#include "CommonData.h"

// CMiddlePotControl

class CMiddlePotControl : public CStatic
{
	DECLARE_DYNAMIC(CMiddlePotControl)

public:
	CMiddlePotControl();
	virtual ~CMiddlePotControl();

protected:
	DECLARE_MESSAGE_MAP()

private:
	double m_fMinPot, m_fMaxPot;
	int m_nMinPot, m_nMaxPot;

	std::vector<CString> m_vecYAxe;
	BOOL m_bPrintPoincare;

	CString fnGetTickLabelText(int);

public:
	void InitializeYAxe();
	void PaintGraph(Gdiplus::Graphics*, CoordParamStruct*);
	CoordParamStruct GetCoordParams();
	void SetPrintPoincareFlag(BOOL param);
	void PrintFile(FILE*);
	afx_msg void OnPaint();
};


