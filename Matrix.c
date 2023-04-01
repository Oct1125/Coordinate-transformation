#include "main.h"

void free_Matrix(Matrix arr)
{
	int i;
	for (i = 0; i < arr.row; i++)
	{
		free(arr.data[i]);
	}
}

Matrix MakeMatrix(int row, int col)
{
	int i = 0;
	Matrix arr = {0};
	arr.row = row;
	arr.col = col;
	arr.data = (double **)malloc(sizeof(double *) * arr.row);
	if (arr.data == NULL)
		exit(-1);
	for (i = 0; i < arr.row; i++)
	{
		arr.data[i] = (double *)malloc(sizeof(double) * arr.col);
		memset(arr.data[i], 0, sizeof(double) * arr.col);
	}
	return arr;
}

void PrintMatrix(Matrix arr)
{
	int i, j;
	for (i = 0; i < arr.row; i++)
	{
		for (j = 0; j < arr.col; j++)
		{
			printf("%lf\t", arr.data[i][j]);
		}
		putchar('\n');
	}
}

Matrix Matrix_Mul(const Matrix left, const Matrix right)
{
	if (left.col != right.row)
		exit(-1); // 判断左列是否等于右行
	Matrix res = {0};
	res.data = (double **)malloc(sizeof(double *) * left.row);
	if (res.data == NULL)
		exit(-1);
	int i, j, k;
	for (i = 0; i < left.row; i++)
	{
		res.data[i] = (double *)malloc(sizeof(double) * right.col); // 创建新矩阵
		memset(res.data[i], 0, sizeof(double) * right.col);			// 初始化
	}
	for (i = 0; i < left.row; i++)
	{
		for (j = 0; j < right.col; j++)
		{
			for (k = 0; k < left.col; k++)
			{
				res.data[i][j] += left.data[i][k] * right.data[k][j]; // 该部分的计算与前文一致
			}
		}
	}
	return res;
}

double Det(Matrix src)
{
	// step 1
	// 判断指针是否为空
	if (src.data == NULL)
		exit(-1);
	int i = 0, j = 0, k = 0, flag = 0, count = 0;
	double sum = 1, tmp;
	Matrix res = {0};
	// 判断矩阵维数
	if (src.row != src.col)
		exit(-1);
	// step 2
	res.row = res.col = src.row;
	res.data = (double **)malloc(sizeof(double *) * res.row);
	for (i = 0; i < res.row; i++)
	{
		res.data[i] = (double *)malloc(sizeof(double) * res.row);
		memset(res.data[i], 0, sizeof(double) * res.row); // 初始化
	}
	// step 3
	// 进行数据拷贝
	for (i = 0; i < res.row; i++)
	{
		memcpy(res.data[i], src.data[i], sizeof(double) * res.row);
	}
	// step 4
	// 找主元，绝对值最大的那一行，与主元行互换
	for (j = 0; j < res.col; j++)
	{
		flag = j;
		double Max = fabs(res.data[flag][j]); // 用绝对值比较
		// 默认当前主元行的数最大,从主对角线下方选择主元
		for (i = j; i < res.row; i++)
		{
			if (fabs(res.data[i][j]) > Max)
			{
				flag = i;
				Max = fabs(res.data[i][j]);
			}
		}
		if (i == j && i != flag)
		{
			count++; // 记录互换次数
			for (k = 0; k < res.col; k++)
			{
				tmp = res.data[flag][k];
				res.data[flag][k] = res.data[i][k];
				res.data[i][k] = tmp;
			}
		}
		// 将主对角下方元素消成0
		for (i = j + 1; i < res.row; i++)
		{
			double b = res.data[i][j] / res.data[j][j];
			for (k = 0; k < res.col; k++)
			{
				res.data[i][k] += b * res.data[j][k] * (-1); // 初等行变换
			}
		}
	}
	// 计算主对角线元素乘积
	for (i = 0; i < res.row; i++)
	{
		for (j = 0; j < res.col; j++)
		{
			if (i == j)
				sum *= res.data[i][j];
		}
	}
	// 必须释放res内存！
	free_Matrix(res);
	return pow(-1, count) * sum;
}

extern void matmul2(const char *tr, const Matrix A, const Matrix B, const Matrix C, double alpha, double beta)
{
	assert(A.col == B.row);
	assert(tr[0] == 'N' || 'T');
	assert(tr[1] == 'N' || 'T');
	if (tr[0] == 'T')
		assert(A.col == A.row);
	if (tr[1] == 'T')
		assert(B.col == B.row);
	double d;
	int i, j, x, f;
	f = tr[0] == 'N' ? (tr[1] == 'N' ? 1 : 2) : (tr[1] == 'N' ? 3 : 4);
	for (i = 0; i < A.row; i++)
	{
		for (j = 0; j < B.col; j++)
		{
			d = 0.0;
			switch (f)
			{
			case 1:
				for (x = 0; x < A.col; x++)
				{
					d += A.data[i][x] * B.data[x][j];
				}
				break;
			case 2:
				for (x = 0; x < A.col; x++)
				{
					d += A.data[i][x] * B.data[j][x];
				}
				break;
			case 3:
				for (x = 0; x < A.col; x++)
				{
					d += A.data[x][i] * B.data[x][j];
				}
				break;
			case 4:
				for (x = 0; x < A.col; x++)
				{
					d += A.data[x][i] * B.data[j][x];
				}
				break;
			}
			if (beta == 0.0)
				C.data[i][j] = alpha * d;
			else
				C.data[i][j] = alpha * d + beta * C.data[i][j];
		}
	}
}

extern void matmul(const char *tr, int n, int k, int m, double alpha,
				   const double *A, const double *B, double beta, double *C)
{
	double d;
	int i, j, x, f = tr[0] == 'N' ? (tr[1] == 'N' ? 1 : 2) : (tr[1] == 'N' ? 3 : 4);

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < k; j++)
		{
			d = 0.0;
			switch (f)
			{
			case 1:
				for (x = 0; x < m; x++)
				{
					d += A[i + x * n] * B[x + j * m];
				}
				break;
			case 2:
				for (x = 0; x < m; x++)
				{
					d += A[i + x * n] * B[j + x * k];
				}
				break;
			case 3:
				for (x = 0; x < m; x++)
				{
					d += A[x + i * m] * B[x + j * m];
				}
				break;
			case 4:
				for (x = 0; x < m; x++)
				{
					d += A[x + i * m] * B[j + x * k];
				}
				break;
			}
			if (beta == 0.0)
				C[i + j * n] = alpha * d;
			else
				C[i + j * n] = alpha * d + beta * C[i + j * n];
		}
	}
}