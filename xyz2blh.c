#include"main.h"

//xzy->blh(rad)
BLH xyz2blh(double X, double Y, double Z)
{
	double a,f,e2;
	a=6378137.0;
 	f=(1.0 / 298.257223563);
	e2=f*(2-f);
	BLH res = { 0 };
	double B = 0.0, N = 0.0, H = 0.0, R0, R1, deltaH, deltaB;
	R0 = sqrt(pow(X, 2) + pow(Y, 2));
	R1 = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
	//经度直接求解
	res.L = atan2(Y, X);
	//迭代求大地维度和大地高
	N = a;
	H = R1 - a;
	B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
	do
	{
		deltaH = N;//判断收敛所用
		deltaB = B;
		N = a / sqrt(1 - e2 * pow(sin(B), 2));
		H = R0 / cos(B) - N;
		B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
	} while (fabs(deltaH - H) > 1.0e-3 && fabs(deltaB - B) > 1.0e-9);
	res.B = B;
	res.H = H;
	return res;
}

/* inner product ---------------------------------------------------------------
 * inner product of vectors
 * args   : double *a,*b     I   vector a,b (n x 1)
 *          int    n         I   size of vector a,b
 * return : a'*b
 *-----------------------------------------------------------------------------*/
extern double dot(const double* a, const double* b, int n)
{
	double c = 0.0;
	while (--n >= 0)
		c += a[n] * b[n];
	return c;
}
/* transform ecef to geodetic postion ------------------------------------------
*transform ecef position to geodetic position
* args   : 
* double* r        I   ecef position{ x,y,z } (m)
* double* pos      O   geodetic position{ lat,lon,h } (rad, m)
* return : none
* notes : WGS84, ellipsoidal height
* ---------------------------------------------------------------------------- - */
void ecef2pos(const double* r, double* pos)
{
	double e2 = FE_WGS84 * (2.0 - FE_WGS84), r2 = dot(r, r, 2), z, zk, v = RE_WGS84, sinp;
	for (z = r[2], zk = 0.0; fabs(z - zk) >= 1E-4;)
	{
		zk = z;
		sinp = z / sqrt(r2 + z * z);
		v = RE_WGS84 / sqrt(1.0 - e2 * sinp * sinp);
		z = r[2] + v * e2 * sinp;
	}
	pos[0] = r2 > 1E-12 ? atan(z / sqrt(r2)) : (r[2] > 0.0 ? PI / 2.0 : -PI / 2.0);
	pos[1] = r2 > 1E-12 ? atan2(r[1], r[0]) : 0.0;
	pos[2] = sqrt(r2 + z * z) - v;
}