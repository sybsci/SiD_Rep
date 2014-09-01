
#pragma once

static const int maxParticlesNumber = 100000;
static const double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;

struct PlotStruct{

	double arrPos[maxParticlesNumber];
	double arrVel[maxParticlesNumber];
	int pNumber;

	double fMaxVel, fMinVel;

	double fAlphaWithoutPi;

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
	float minPos, maxPos, minVel, maxVel;
	float PosGridStep, VelGridStep;

	int xMin, xMax, yMin, yMax;

	//���������� ����� �� ��� Y
	int nGrades;

	//���� ������� ��������� �������
	CRect GraphRect;

};