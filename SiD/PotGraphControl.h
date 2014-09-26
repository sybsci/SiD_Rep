#pragma once
#include <vector>
#include "CommonData.h"

// PotGraphControl

class CPotGraphControl : public CStatic
{
	DECLARE_DYNAMIC(CPotGraphControl)

public:
	CPotGraphControl();
	virtual ~CPotGraphControl();

private:
	double m_fMinPot, m_fMaxPot;
	int m_nMinPot, m_nMaxPot;
	std::vector<CString> m_vecYAxe;

	CString fnGetTickLabelText(int);

protected:
	DECLARE_MESSAGE_MAP()

public:
	void InitializeYAxe();
	void PaintGraph(Gdiplus::Graphics* pMemG, CoordParamStruct*);
	CoordParamStruct GetCoordParams();

	afx_msg void OnPaint();
	void GetPotArray(double* arr);
};


