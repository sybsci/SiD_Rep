#pragma once
#include <vector>
#include "CommonData.h"

// CGraphControl

class CGraphControl : public CStatic
{
	DECLARE_DYNAMIC(CGraphControl)

public:
	CGraphControl();
	virtual ~CGraphControl();
private:
	double m_fMinVel, m_fMaxVel;
	int m_nMinVel, m_nMaxVel;
	std::vector<CString> m_vecYAxe;

	CString fnGetTickLabelText(int);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void InitializeYAxe();
	void PaintGraph(Gdiplus::Graphics* pMemG, CoordParamStruct*);
	CoordParamStruct GetCoordParams();
	void PrintFile(FILE* pFile);
};


