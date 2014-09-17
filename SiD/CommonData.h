
#pragma once

#include <vector>

static const int maxParticlesNumber = 100000;
static const int Ng = 256;
static const double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;

struct PlotStruct{

	double arrPos[maxParticlesNumber];
	double arrVel[maxParticlesNumber];

	double arrPot[Ng + 1];

	int pNumber;

	double fMaxVel, fMinVel;
	double fMaxPot, fMinPot;

	double fAlphaWithoutPi;

	unsigned int nDotNumber;
	std::vector<double> PMap;
};


struct CoordParamStruct{

	//���� ������ ������� � �������� (!!!)
	int edgingLeft, edgingRight, edgingBottom, edgingTop;

	//������� ������ ������� � ���������� �������� ��������� (�� ������ ������� �� ����� � ��������)
	float logSizeX, logSizeY;

	//������ ��������� � ���������� �������� ���������
	float logOriginX, logOriginY;

	//�������-��������
	int xVE, yVE;

	//������� ���������� ������ � ������� ��������
	float fPixPerX, fPixPerY;

	//������� � ������� �������
	float minPos, maxPos, minVel, maxVel, minPot, maxPot;
	float PosGridStep, VelGridStep;

	int xMin, xMax, yMin, yMax;

	//���������� ����� �� ��� Y
	int nGrades;

	//���� ������� ��������� �������
	CRect GraphRect;

};