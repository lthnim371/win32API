#include <windows.h>
#include <tchar.h> // for unicode
#include "utility.h"
#include <math.h> //¶Ç´Â <cmath>

using namespace std;

int main(void)
{
/*
sin(theta_rad) = value
theta_rad = asin(theta_rad)

sin(pi/2) = 1

pi/2 = asin(1)
pi = 2*asin(1)

*/

	float pi = 2.f*asin(1.f);
	float D2R = pi/180.f; //degree_to_radian
	
	
	
	for(int i =0; i <= 360; ++i)
	{
		//cos
		cout << "cos(" << i << ") = " << cos(i*1.0f*3.14f/180.f) << endl;
	}

	_getch();
	return 0;
}