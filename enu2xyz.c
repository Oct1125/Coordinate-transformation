#include"main.h"
XYZ enu2xyz(double Xr, double Yr, double Zr, double Es, double Ns, double Us)
{
    double sinA,cosA,sinB,cosB;
	XYZ xyz = { 0 };
	BLH ref ={0};
	Matrix R,X,res;
	R=MakeMatrix(3,3);
	X=MakeMatrix(3,1);
	res=MakeMatrix(3,3);
	ref=xyz2blh(Xr,Yr,Zr);
    sinA=sin(ref.L);
    cosA=cos(ref.L);
    sinB=sin(ref.B);
    cosB=cos(ref.B);
	//构建旋转矩阵R

	R.data[0][0] = -sin(ref.L);
	R.data[1][0] = cos(ref.L);
	R.data[2][0] = 0;
	R.data[0][1] = -sin(ref.B) * cos(ref.L);
	R.data[1][1] = -sin(ref.B) * sin(ref.L);
	R.data[2][1] = cos(ref.B);
	R.data[0][2] = cos(ref.B) * cos(ref.L);
	R.data[1][2] = cos(ref.B) * sin(ref.L);
	R.data[2][2] = sin(ref.B);

	//构建X向量
	X.data[0][0] = Es;
    X.data[1][0] = Ns;
    X.data[2][0] = Us;
	res = Matrix_Mul(R, X);
	xyz.X=res.data[0][0];
	xyz.Y=res.data[1][0];
	xyz.Z=res.data[2][0];
	free_Matrix(R),free_Matrix(X),free_Matrix(res);
	return xyz;
}


extern void enu2xzy2(const double *pos, const double *e, double *r)
{
	//pos reference BLH
	//e   E、N、U
    double E[9];
    
    xyz2enu3(pos,E);
    matmul("TN",3,1,3,1.0,E,e,0.0,r);
}