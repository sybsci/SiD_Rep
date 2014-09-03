
#pragma once

#include "CommonData.h"

class CSiDDlg;

class CPierceDiode
{
private:
	int Ng;
	double a0;

	int Np, Npc;

	double *vel, *pos;
	double *den, *pot, *E;

	double dx, dt;

	double alpha;
	bool bSimIsRun;

	void zerosArr(double*, int);
	void PoissonSolve(double*, int, double, double, const double*);

public:
	CPierceDiode(CSiDDlg*);
	~CPierceDiode();

	CSiDDlg* pParentWnd;

	void Initialize();
	void ResumeSimulation();
	//void SetAlpha(double AlphaWithoutPi);
	void SetRunFlag(bool flag);
	void StartCalcThread();
	//void Test();
	void ClearData();

	void SetAlpha(double AlphaWithoutPi);
};

