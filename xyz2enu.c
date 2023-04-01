#include"main.h"
//参考坐标xr，yr，zr
//代求坐标Xs，Ys，Zs
ENU xyz2enu(double Xr, double Yr, double Zr, double Xs, double Ys, double Zs)
{
	ENU enu = { 0 };
	BLH ref ={0};
	Matrix R,X,res;
	R=MakeMatrix(3,3);
	X=MakeMatrix(3,1);
	res=MakeMatrix(3,3);
	ref=xyz2blh(Xr,Yr,Zr);
	//构建旋转矩阵R
	R.data[0][0] = -sin(ref.L);
	R.data[0][1] = cos(ref.L);
	R.data[0][2] = 0;
	R.data[1][0] = -sin(ref.B) * cos(ref.L);
	R.data[1][1] = -sin(ref.B) * sin(ref.L);
	R.data[1][2] = cos(ref.B);
	R.data[2][0] = cos(ref.B) * cos(ref.L);
	R.data[2][1] = cos(ref.B) * sin(ref.L);
	R.data[2][2] = sin(ref.B);
	//构建X向量
	X.data[0][0] = Xs - Xr;
	X.data[1][0] = Ys - Yr;
	X.data[2][0] = Zs - Zr;
	res = Matrix_Mul(R, X);
	enu.E=res.data[0][0];
	enu.N=res.data[1][0];
	enu.U=res.data[2][0];
	free_Matrix(R),free_Matrix(X),free_Matrix(res);
	return enu;
}

//fortran code
ENU xyz2enu2(double Xr, double Yr, double Zr, double Xs, double Ys, double Zs)
{
	double dx,dy,dz,sinA,cosA,sinB,cosB,lat,lon;
	ENU enu = { 0 };
	BLH ref ={0};
	ref=xyz2blh(Xr,Yr,Zr);
	//进行补角
	lat=ref.B-PI/2.0;
	lon=ref.L-PI;
	//计算代求点与参考点坐标差
	dx=Xs-Xr;
	dy=Ys-Yr;
	dz=Zs-Zr;
	//计算旋转因子
	sinA=sin(lat);
	cosA=cos(lat);
	sinB=sin(lon);
	cosB=cos(lon);
	//直接求解
	enu.E=sinB*dx-cosB*dy;
	enu.N=cosA*cosB*dx+cosA*sinB*dy-sinA*dz;
	enu.U=sinA*cosB*dx+sinA*sinB*dy+cosA*dz;
	return enu;
}

//rtklib code
/* ecef to local coordinate transfromation matrix ------------------------------
* compute ecef to local coordinate transfromation matrix
* args   : double *pos      I   geodetic position {lat,lon} (rad)
*          double *E        O   ecef to local coord transformation matrix (3x3)
* return : none
* notes  : matirix stored by column-major order (fortran convention)
*-----------------------------------------------------------------------------*/
extern void xyz2enu3(const double *pos, double *E)
{
    double sinp=sin(pos[0]),cosp=cos(pos[0]),sinl=sin(pos[1]),cosl=cos(pos[1]);
    
    E[0]=-sinl;      E[3]=cosl;       E[6]=0.0;
    E[1]=-sinp*cosl; E[4]=-sinp*sinl; E[7]=cosp;
    E[2]=cosp*cosl;  E[5]=cosp*sinl;  E[8]=sinp;
}
/* transform ecef vector to local tangental coordinate -------------------------
* transform ecef vector to local tangental coordinate
* args   : double *pos      I   geodetic position {lat,lon} (rad)
*          double *r        I   vector in ecef coordinate {x,y,z}
*          double *e        O   vector in local tangental coordinate {e,n,u}
* return : none
*-----------------------------------------------------------------------------*/
extern void ecef2enu3(const double *pos, const double *r, double *e)
{
    double E[9];
    
    xyz2enu3(pos,E);
    matmul("NN",3,1,3,1.0,E,r,0.0,e);
}

//my rtklib 
extern void xyz2enu4(double lat,double lon,Matrix E)
{
    double sinp=sin(lat),cosp=cos(lat),sinl=sin(lon),cosl=cos(lon);
    
    E.data[0][0]=-sinl;      E.data[0][1]=cosl;       E.data[0][2]=0.0;
    E.data[1][0]=-sinp*cosl; E.data[1][1]=-sinp*sinl; E.data[1][2]=cosp;
    E.data[2][0]=cosp*cosl;  E.data[2][1]=cosp*sinl;  E.data[2][2]=sinp;
}

extern void ecef2enu4(double lat,double lon, Matrix r, Matrix e)
{
	Matrix E = MakeMatrix(3, 3);
	xyz2enu4(lat,lon,E);
	matmul2("NN",E,r,e,1.0,0.0);
}