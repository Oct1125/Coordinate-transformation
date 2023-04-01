#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/malloc.h>
#include <assert.h>

#define PI 3.141592653589793

#define RE_WGS84 6378137.0			   /* earth semimajor axis (WGS84) (m) */
#define FE_WGS84 (1.0 / 298.257223563) /* earth flattening (WGS84) */

#define MAX 150

typedef struct CFG
{
	char data_address[MAX]; // 原始pos数据地址
	char dop_address[MAX];	// 原始dop数据地址
	char save_address[MAX]; // 保存数据地址
	char FILE_NAME[MAX];	// 数据文件名
	double PARAMATERS[7];	
	double REFENCE_X;
	double REFENCE_Y;
	double REFENCE_Z;
	int I_SYS;
	int I_XYZ;
	int I_BLH;
	int I_ENU;
	int O_SYS;
	int O_XYZ;
	int O_BLH;
	int O_ENU;
} CFG, *pCFG;

typedef struct BLH
{
	double B;
	double L;
	double H;
} BLH, *pBLH;

typedef struct ENU
{
	double E;
	double N;
	double U;
} ENU, *pENU;

typedef struct XYZ
{
	double X;
	double Y;
	double Z;
} XYZ, *pXYZ;

typedef struct Matrix
{
	int row;
	int col;
	double **data;
} Matrix, *pMatrix;

typedef struct POS
{
	char name[4];
	double xyz[3];
	double blh[3];
	double enu[3];
} POS, *pPOS;

void free_Matrix(Matrix arr);
void PrintMatrix(Matrix arr);
Matrix MakeMatrix(int row, int col);
Matrix Matrix_Mul(const Matrix left, const Matrix right);
double Det(Matrix src);

extern void matmul(const char *tr, int n, int k, int m, double alpha,
				   const double *A, const double *B, double beta, double *C);
extern void matmul2(const char *tr, const Matrix A, const Matrix B, const Matrix C, double alpha, double beta);

extern BLH xyz2blh(double X, double Y, double Z);
extern ENU xyz2enu(double Xr, double Yr, double Zr, double Xs, double Ys, double Zs);
extern ENU xyz2enu2(double Xr, double Yr, double Zr, double Xs, double Ys, double Zs);
extern void xyz2enu3(const double *pos, double *E);
extern void ecef2enu3(const double *pos, const double *r, double *e);
extern void xyz2enu4(double lat, double lon, Matrix E);
extern void ecef2enu4(double lat, double lon, Matrix r, Matrix e);

extern XYZ enu2xyz(double Xr, double Yr, double Zr, double Xs, double Ys, double Zs);
extern void enu2xzy2(const double *pos, const double *e, double *r); // rtklib

extern void ecef2pos(const double *r, double *pos);

extern void pos2ecef(const double *pos, double *r);
extern XYZ blh2xyz(double B, double L, double H);

XYZ WGS84toCGCS2000(CFG cfg,double x, double y, double z);
XYZ CGCS2000toWGS84(CFG cfg,double x, double y, double z);

/*---------------------------------Fuction--------------------------------------*/
CFG readCFG(char *cfgname);
int getrow(FILE *fp);
void setstr(char *des, const char *src, int n);
double str2num(const char *buff, int i, int n);
void readdata(CFG cfg, FILE *fp, int row, pPOS pos);

void coortran();
