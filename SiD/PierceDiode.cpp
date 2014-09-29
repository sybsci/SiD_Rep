
#include "stdafx.h"
#include "Resource.h"
#include "PierceDiode.h"
#include "SiDDlg.h"


PlotStruct PlotData;

UINT fnCalcThread(LPVOID Param);

CRITICAL_SECTION critS;


CPierceDiode::CPierceDiode(CSiDDlg* _pParentWnd)
{
	pParentWnd = _pParentWnd;

	a0 = 0.001;

	nTimeCount = 0;

	alpha = 1.1 * PI;	//by default
	bSimIsRun = false;

	den = new double[Ng + 1];
	pot = new double[Ng + 1];
	E = new double[Ng + 1];

	vel = new double[maxParticlesNumber];
	pos = new double[maxParticlesNumber];

	InitializeCriticalSection(&critS);

	PlotData.pNumber = 0;
	PlotData.fAlphaWithoutPi = 1.1;

	PlotData.nDotNumber = 100;
	PlotData.PMap = std::vector<double>(PlotData.nDotNumber, 0.);

	PlotData.fMaxE0 = 1.0;
	PlotData.fMinE0 = 0.0;

	DotStruct dotE0;
	dotE0.time = 0;
	dotE0.value = 0.;
	PlotData.E0.push_back(dotE0);
}


CPierceDiode::~CPierceDiode()
{
	delete[] den;
	delete[] pot;
	delete[] E;

	delete[] vel;
	delete[] pos;
}


void CPierceDiode::PoissonSolve(double *pot, int Ng, double dx, double alpha, const double *den){

	pot[0] = 0.;
	pot[1] = 0.;
	for (int i = 2; i <= Ng; ++i)
		pot[i] = dx * dx * alpha * alpha * (den[i - 1] - 1.) + 2. * pot[i - 1] - pot[i - 2];
	for (int i = 1; i <= Ng - 1; ++i)
		pot[i] = pot[i] - (double)i * pot[Ng] / (double)Ng;
	pot[Ng] = 0.;

}

void CPierceDiode::Initialize(){

	dx = 1. / (double)Ng;
	Npc = (int)(dx * alpha / 0.005) + 1;
	dt = dx / ((double)Npc);
	Np = Npc * Ng;

	for (int i = 0; i < Np; ++i){
		vel[i] = 1.;
		pos[i] = (double)i / (double)Np + a0 * sin(2. * PI * (double)i / (double)Np) + dt / 2.;
	};

}

void CPierceDiode::ResumeSimulation(){

	bSimIsRun = true;

	double rel, shift, force;
	int ind;
	

	double fMaxVel, fMinVel;
	double fMaxPot, fMinPot;

	double sumTimePerFrame = 0.;

	double z1 = 0., z2 = 0., z3 = 0.;
	double E0prev = 0., E0prevprev = 0.;

	DotStruct dotE0, dotE0_Poin;

	size_t denSize = (Ng + 1) * sizeof(double);

	while (bSimIsRun){

		nTimeCount++;

		//раздача заряда методом CIC (линейное взвешивание)
		ZeroMemory(den, denSize);
		for (int i = 0; i < Np; ++i){
			rel = (pos[i] / dx);
			ind = (int)rel;
			shift = rel - (double)ind;

			den[ind] += (1. - shift) / (double)Npc;
			den[ind + 1] += shift / (double)Npc;
		};

		//решение уравнения Пуассона
		PoissonSolve(pot, Ng, dx, alpha, den);

		//вычисление эл. поля
		E[0] = (pot[0] - pot[1]) / dx;
		E[Ng] = (pot[Ng - 1] - pot[Ng]) / dx;
		for (int i = 1; i <= Ng - 1; ++i)
			E[i] = (pot[i - 1] - pot[i + 1]) * .5 / dx;

		//решение уравнения движения
		for (int i = 0; i < Np; ++i){

			rel = pos[i] / dx;
			ind = (int)rel;
			shift = rel - (double)ind;
			force = E[ind] * (1. - shift) + shift * E[ind + 1];

			vel[i] -= dt * force;
			pos[i] += dt * vel[i];

			if (pos[i] < 0. || pos[i] >= 1.){
				pos[i] = pos[Np - 1];
				vel[i] = vel[Np - 1];
				Np--;
				i--;
			};

		};


		pos[Np] = 0.5*dt;	//because leapfrog
		vel[Np] = 1.;

		Np++;


		//работа с отображение Пуанкаре
		z1 = z2;
		z2 = z3;
		z3 = pot[Ng / 2];

		if ((z1 < z2) && (z2 > z3))
		{
			EnterCriticalSection(&critS);
				PlotData.PMap.insert(PlotData.PMap.begin(), z2);
				if (PlotData.PMap.size() > PlotData.nDotNumber)
					PlotData.PMap.erase(PlotData.PMap.begin() + PlotData.nDotNumber);
			LeaveCriticalSection(&critS);
		};


		//работа с напряженностью эл. поля на входном электроде
		if (nTimeCount % AnalizeEvery == 0)
		{
			EnterCriticalSection(&critS);

			dotE0.time = nTimeCount;
			dotE0.value = E[0];
			PlotData.E0.insert(PlotData.E0.begin(), dotE0);

			if (PlotData.E0.size() > MaxSizeOfE0)
				PlotData.E0.erase(PlotData.E0.begin() + MaxSizeOfE0);

			if (E[0] > PlotData.fMaxE0)
				PlotData.fMaxE0 = E[0];

			if (E[0] < PlotData.fMinE0)
				PlotData.fMinE0 = E[0];

			LeaveCriticalSection(&critS);
		};

		if (E0prevprev < E0prev && E0prev > E[0])
		{
			EnterCriticalSection(&critS);

			dotE0_Poin.time = nTimeCount - 1;
			dotE0_Poin.value = E0prev;
			PlotData.E0_Poin.insert(PlotData.E0_Poin.begin(), dotE0_Poin);

			if (PlotData.E0_Poin.size() > MaxSizeOfE0_Poin)
				PlotData.E0_Poin.erase(PlotData.E0_Poin.begin() + MaxSizeOfE0_Poin);

			LeaveCriticalSection(&critS);
		};

		E0prevprev = E0prev;
		E0prev = E[0];


		//взаимодействие с окнами

		sumTimePerFrame += dt;
		if (sumTimePerFrame > pParentWnd->fTimeStepPerFrame){
			sumTimePerFrame = 0.;

			WaitForSingleObject(pParentWnd->eventTimer.m_hObject, INFINITE);


			EnterCriticalSection(&critS);

			PlotData.pNumber = Np;
			PlotData.fAlphaWithoutPi = alpha / PI;

			fMaxVel = 0.;
			fMinVel = 0.;

			fMaxPot = 0.;
			fMinPot = 0.;

			for (int i = 0; i<Np; ++i){

				PlotData.arrPos[i] = pos[i];
				PlotData.arrVel[i] = vel[i];

				if (vel[i] > fMaxVel)
					fMaxVel = vel[i];

				if (vel[i] < fMinVel)
					fMinVel = vel[i];

			};

			for (int i = 0; i <= Ng; ++i)
			{
				PlotData.arrPot[i] = pot[i];

				if (pot[i] > fMaxPot)
					fMaxPot = pot[i];

				if (pot[i] < fMinPot)
					fMinPot = pot[i];
			};


			PlotData.fMaxVel = fMaxVel;
			PlotData.fMinVel = fMinVel;

			PlotData.fMaxPot = fMaxPot;
			PlotData.fMinPot = fMinPot;


			LeaveCriticalSection(&critS);


			pParentWnd->RedrawGraphs();

			pParentWnd->eventTimer.ResetEvent();

		};


	};

}


void CPierceDiode::SetRunFlag(bool flag)
{
	bSimIsRun = flag;
}


void CPierceDiode::ClearData()
{
	ZeroMemory(PlotData.arrPos, maxParticlesNumber * sizeof(double));
	ZeroMemory(PlotData.arrVel, maxParticlesNumber * sizeof(double));
	ZeroMemory(PlotData.arrPot, (Ng + 1) * sizeof(double));

	PlotData.pNumber = 0;
	PlotData.fMaxVel = 1.f;
	PlotData.fMinVel = 0.f;
	PlotData.fMaxPot = 1.f;
	PlotData.fMinPot = 0.f;
	PlotData.fMaxE0 = 1.f;
	PlotData.fMinE0 = 0.f;

	PlotData.E0.clear();
	PlotData.E0_Poin.clear();

	DotStruct dotE0;
	dotE0.time = 0;
	dotE0.value = 0.;

	PlotData.E0.push_back(dotE0);
	PlotData.E0_Poin.push_back(dotE0);

	PlotData.PMap = std::vector<double>(PlotData.nDotNumber, 0.);

	nTimeCount = 0;
}


void CPierceDiode::StartCalcThread()
{
	AfxBeginThread(fnCalcThread, this);
}


UINT fnCalcThread(LPVOID Param)
{
	CPierceDiode* pDiode = (CPierceDiode*)Param;

	pDiode->ResumeSimulation();

	AfxEndThread(0);
	return 0;
}


void CPierceDiode::SetAlpha(double AlphaWithoutPi)
{
	alpha = AlphaWithoutPi * PI;
}
