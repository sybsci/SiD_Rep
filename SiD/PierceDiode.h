
#pragma once

#include "CommonData.h"

class CSiDDlg;

class CPierceDiode
{
private:
	
	double a0;

	int Np, Npc;

	double *vel, *pos;
	double *den, *pot, *E;

	double dx, dt;

	double alpha;
	bool bSimIsRun;

	long long int nTimeCount;

	void PoissonSolve(double*, int, double, double, const double*);

public:
	CPierceDiode(CSiDDlg*);
	~CPierceDiode();

	CSiDDlg* pParentWnd;

	void Initialize();
	void ResumeSimulation();
	void SetRunFlag(bool flag);
	void StartCalcThread();
	void ClearData();
	void SetAlpha(double AlphaWithoutPi);
};

