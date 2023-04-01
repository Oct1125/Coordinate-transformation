#include"main.h"

//获取行数
int getrow(FILE* fp)
{
	int n = 0;
	char buff[MAX] = { 0 };//用来存放读取到的字符串
	//gets函数，读取一行，当读取结束后返回NULL指针，格式如下：
	//char * fgets ( char * str, int num, FILE * stream );
	while (fgets(buff, MAX, fp))
	{
		n+=1;
	}
	return n;
}

//将字符串转换为浮点数,i起始位置，n输入多少个字符
double str2num(const char* buff, int i, int n)
{
	double value = 0.0;
	char str[256] = { 0 };
	char* p = str;
	/************************************
	* 当出现以下三种情况报错，返回0.0
	* 1.起始位置<0
	* 2.读取字符串个数<i
	* 3.str里面存放的字节数<n
	*************************************/
	if (i < 0 || (int)strlen(buff) < i || (int)sizeof(str) - 1 < n)
	{
		return 0.0;
	}
	for (buff += i; *buff && --n >= 0; buff++)
	{
		//三目操作符：D和d为文件中科学计数法部分，将其转换成二进制能读懂的e
		*p++ = ((*buff == 'D' || *buff == 'd') ? 'e' : *buff);
	}
	*p = '\0';
	//三目操作符，将str中存放的数以格式化读取到value中。
	return sscanf(str, "%lf", &value) == 1 ? value : 0.0;
}

void setstr(char* des, const char* src, int n)
{
	char* p = des;
	const char* q = src;
	while (*q && q < src+n)
	{
		*p++ = *q++ ;
	}
	*p-- = '\0';
//去掉尾部空格
	while (p >= des && *p == ' ')
	{
		*p-- = '\0';
	}
}



//经纬度转换(弧度)
BLH XYZtoLB(double X, double Y, double Z)
{
	double a =   6378137.0;                                   //长半轴
	double f = (1 / 298.257222101);                           //扁率
	double b = (a - a * f);                                   //短半轴
	double e2 = (f*(2-f));                                    //第一偏心率平方
	BLH res = { 0 };
	double B = 0.0, N = 0.0, H = 0.0, R0, R1, deltaH, deltaB;
	R0 = sqrt(pow(X, 2) + pow(Y, 2));
	R1 = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
	//经度直接求解
	res.L = atan2(Y,X);
	//迭代求大地维度和大地高
	N = a;
	H = R1 - sqrt(a * b);
	B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
	do
	{
		deltaH = N;//判断收敛所用
		deltaB = B;
		N = a / sqrt(1 - e2 * pow(sin(B), 2));
		H = R0 / cos(B) - N;
		B = atan2(Z * (N + H), R0 * (N * (1 - e2) + H));
	} while (fabs(deltaH - H) > 0.001 && fabs(deltaB - B) > 1.0e-9);
	res.B = B;
	res.H = H;
	return res;
}


           
