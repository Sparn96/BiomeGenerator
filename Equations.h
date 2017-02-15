#ifndef __GENEQUAITONS_H
#define __GENEQUAITONS_H

#include <cmath>


//const float pi = 3.14159265359f;

float mGaussCurve(float x, float curveHeight, float posCenter, float standDev);
float mcalcPointDist(float x1, float y1, float x2, float y2);
int mrandom(int a, int b);
struct cartPair
{
	int x;
	int y;
};

#endif