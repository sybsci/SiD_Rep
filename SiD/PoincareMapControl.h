#pragma once

#include <vector>
#include "CommonData.h"


// CPoincareMapControl

class CPoincareMapControl : public CStatic
{
	DECLARE_DYNAMIC(CPoincareMapControl)

public:
	CPoincareMapControl();
	virtual ~CPoincareMapControl();

private:
	double m_fMinPot, m_fMaxPot;
	int m_nMinPot, m_nMaxPot;
	std::vector<CString> m_vecYAxe;

	CString fnGetTickLabelText(int);

protected:
	DECLARE_MESSAGE_MAP()

public:
	void PaintGraph(Gdiplus::Graphics* pMemG, CoordParamStruct*);
	CoordParamStruct GetCoordParams();
	afx_msg void OnPaint();

	void InitializeAxes();
};


