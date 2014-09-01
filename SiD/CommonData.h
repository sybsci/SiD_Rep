
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

	//края вокруг графика в пикселях (!!!)
	int edgingLeft, edgingRight, edgingBottom, edgingTop;

	//размеры холста графика в логических единицах измерения (не считая отбивки по краям в пикселях)
	float logSizeX, logSizeY;

	//начало координат в логических единицах измерения
	float logOriginX, logOriginY;

	//вьюпорт-экстенты
	int xVE, yVE;

	//перевод логических единиц в единицы экранные
	float fPixPerX, fPixPerY;

	//размеры и границы графика
	float minPos, maxPos, minVel, maxVel;
	float PosGridStep, VelGridStep;

	int xMin, xMax, yMin, yMax;

	//количество меток по оси Y
	int nGrades;

	//сама область рисования графика
	CRect GraphRect;

};