
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

	Ng = 256;
	a0 = 0.001;

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
}


CPierceDiode::~CPierceDiode()
{
	delete[] den;
	delete[] pot;
	delete[] E;

	delete[] vel;
	delete[] pos;
}


void CPierceDiode::zerosArr(double* pArr, int numEl){
	for (int i = 0; i < numEl; ++i)
		pArr[i] = 0.;
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

	double sumTimePerFrame = 0.;

	while (bSimIsRun){

		//раздача заряда методом CIC (линейное взвешивание)
		zerosArr(den, Ng + 1);
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

			/*if (0 == kk)
			vel[i] += 0.5 * dt * force;*/

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

			for (int i = 0; i<Np; ++i){

				PlotData.arrPos[i] = pos[i];
				PlotData.arrVel[i] = vel[i];

				if (vel[i] > fMaxVel)
					fMaxVel = vel[i];

				if (vel[i] < fMinVel)
					fMinVel = vel[i];

			};

			PlotData.fMaxVel = fMaxVel;
			PlotData.fMinVel = fMinVel;

			LeaveCriticalSection(&critS);


			//pParentWnd->RedrawGraphs();

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
	ZeroMemory(PlotData.arrPos, maxParticlesNumber*sizeof(double));
	ZeroMemory(PlotData.arrVel, maxParticlesNumber*sizeof(double));

	PlotData.pNumber = 0;
	PlotData.fMaxVel = 1.f;
	PlotData.fMinVel = 0.f;
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


//void CPierceDiode::SetAlpha(double AlphaWithoutPi){
//
//	alpha = AlphaWithoutPi * PI;
//
//}



//void CPierceDiode::Test(){
//
//	pParentWnd->Test();
//
//}
//





