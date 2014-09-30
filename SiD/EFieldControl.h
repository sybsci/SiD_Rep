#pragma once
#include "CommonData.h"

// CEFieldControl

class CEFieldControl : public CStatic
{
	DECLARE_DYNAMIC(CEFieldControl)

public:
	CEFieldControl();
	virtual ~CEFieldControl();

protected:
	DECLARE_MESSAGE_MAP()

private:
	double m_fMinE, m_fMaxE;
	int m_nMinE, m_nMaxE;
	std::vector<CString> m_vecYAxe;

	CString fnGetTickLabelText(int);

public:
	void InitializeYAxe();
	void PaintGraph(Gdiplus::Graphics*, CoordParamStruct*);
	CoordParamStruct GetCoordParams();
	afx_msg void OnPaint();
private:
	BOOL m_bPrintPoincare;
public:
	void SetPrintPoincareFlag(BOOL param);
	void PrintFile(FILE*);
};


