#include "Equations.h"


float mGaussCurve(float x, float curveHeight, float posCenter, float standDev)
{
	float ee = 2.71828f;
	float result = 0.0f;

	result = pow((curveHeight * ee), (-((pow((x - posCenter), 2)) / (2 * pow(standDev, 2)))));  // Gaussian Equation

	return result;
}

float mcalcPointDist(float x1, float y1, float x2, float y2)
{
	float result = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	return abs(result);
}


int mrandom(int a, int b) //Returns a random number between a and b
{
	if (a == b) return a;
	return (a < b) ? (rand() % (b - a + 1) + a) : (rand() % (a - b + 1) + b);
}
