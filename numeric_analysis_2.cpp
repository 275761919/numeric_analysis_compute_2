// compute_practice_2.cpp : Defines the entry point for the console application.

//带双步位移的QR方法
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

using namespace std;

const double e = 1e-12;      //迭代的精度水平
const int Max = 1000;   //迭代的最大次数

typedef struct{
	double Re;
	double Im;
}ComplexNumber;

//void initMat(double **a);   //创建题中要求的矩阵
//void hessenbergMat(double **a);  //对矩阵进行拟上三角化
//int muiltiplyMat(double **a, double **b, double **c, int m);   //计算m阶矩阵的乘法a*b =c
//int printMat(double **a);   //打印矩阵
//void QRmethod(double **a);   //带双步位移的QR分解法
//void zeroMat(double **a);  //将满足精度要求可看作零的元素全部置为0
//void output(ComplexNumber *L);  //输出特征值和特征向量
//void iterate(double **M, double **a, int m); 带双步位移的QR分解
//void gauss(double lambda);  //高斯消元
//void maxline(double **ptr, int n, int k);

//初始化矩阵
void initMat(double **& a)
{
	int i, j;
	a = (double **)malloc(10 * sizeof(double *));
	for (i = 0; i<10; i++)
		a[i] = (double *)malloc(11 * sizeof(double));
	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			if (i != j)
				a[i][j] = sin(0.5*(i + 1) + 0.2*(j + 1));
			else
				a[i][j] = 1.52*cos(i + 1 + 1.2*(j + 1));
		}
	}
}


//满足精度要就的元素置0
void zeroMat(double **a)
{
	int i, j;
	for (i = 0; i<10; i++) {
		for (j = 0; j<10; j++) {
			if (a[i][j]<e&&a[i][j]>-e)
				a[i][j] = 0;
		}
	}
}

//将矩阵拟上三角化
void hessenbergMat(double **a)
{
	int r, i, j;
	double c, d, h, t, u[10], p[10], q[10], w[10];
	for (r = 0; r<8; r++)
	{
		zeroMat(a);//太小的数置0
		c = 0; d = 0; h = 0;
		for (i = r + 2; i<10; i++)
			d += a[i][r] * a[i][r];
		if (d == 0)
			continue;
		else {
			d += a[r + 1][r] * a[r + 1][r];
			d = pow(d, 0.5);
			if (a[r + 1][r] != 0)
				c = -fabs(a[r + 1][r]) / a[r + 1][r] * d;
			else
				c = d;
			h = c*c - c*a[r + 1][r];
			//calulate ur
			for (i = 0; i<r + 1; i++)
				u[i] = 0;
			u[r + 1] = a[r + 1][r] - c;
			for (i = r + 2; i<10; i++)
				u[i] = a[i][r];
			for (i = 0; i<10; i++)
			{
				p[i] = 0; q[i] = 0;
				for (j = 0; j<10; j++)
				{
					p[i] += a[j][i] * u[j] / h;
					q[i] += a[i][j] * u[j] / h;
				}
			}
			t = 0;
			for (i = 0; i<10; i++)
				t += p[i] * u[i] / h;
			for (i = 0; i<10; i++)
				w[i] = q[i] - t*u[i];
			for (i = 0; i<10; i++) {
				for (j = 0; j<10; j++)
					a[i][j] -= (w[i] * u[j] + u[i] * p[j]);
			}
		}
	}
}

//计算m阶矩阵乘法 c=a*b
void muiltiplyMat(double **a, double **b, double **c, int m)
{
	for (int i = 0; i<m; i++) {
		for (int j = 0; j<m; j++) {
			c[i][j] = 0;
			for (int k = 0; k<m; k++)
				c[i][j] += a[i][k] * b[k][j];
		}
	}
}

//print matrix element
int printMat(double **a)
{
	int i, j;
	for (i = 0; i<10; i++)
	{
		for (j = 0; j<10; j++)
			printf("%.12e ", a[i][j]);
		printf("\n");
	}
	printf("\n\n\n");
	return 0;
}

//符号函数
int sgn(double a)
{
	if (a >= 0)
		return 1;
	else
		return (-1);
}

//求Q、R和RQ
void QR_Matrix(double **a)
{
	int i , j , r;
	double c , d , h , w[10] , p[10], u[10];
	double **q, **R, **RQ;
	q = (double **)malloc(10 * sizeof(double *));
	for (i = 0; i<10; i++)
		q[i] = (double *)malloc(11 * sizeof(double));
    R = (double **)malloc(10 * sizeof(double *));
	for (i = 0; i<10; i++)
		R[i] = (double *)malloc(11 * sizeof(double));
    RQ = (double **)malloc(10 * sizeof(double *));
	for (i = 0; i<10; i++)
		RQ[i] = (double *)malloc(11 * sizeof(double));
	for(i = 0; i < 10; i++){//初始化Q1
		for(j = 0; j < 10; j++){
			if(i == j)
				q[i][i] = 1;
			else
				q[i][j] = 0;
		}
	}
	//initMat(R);
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++)
			R[i][j] = a[i][j];
	}
	zeroMat(R);
	for(r = 0; r < 9; r++){
		d = 0;
		for(i = r + 1; i < 10; i++)
			d += R[i][r] * R[i][r];
		if(fabs(d) == 0)
			continue;
		else{
			d += R[r][r] * R[r][r];
			d = pow(d, 0.5);
			if (R[r][r] == 0)
				c = d;
			else
				c = -1 * sgn(R[r][r]) * d;
			h = c * c - c * R[r][r];
			for(i = 0; i < r; i++)
				u[i] = 0;
			u[r] = R[r][r] - c;
			for(i = r + 1; i < 10; i++)
				u[i] = R[i][r];
			for(i = 0; i < 10; i++){
				w[i] = 0;
				for(j = 0; j < 10; j++)
					w[i] += q[i][j] * u[j];
			}
			for(i = 0;i < 10; i++){
				for(j = 0; j < 10; j++)
					q[i][j] = q[i][j] - w[i] * u[j] / h;
			}
			for(i = 0; i < 10; i++){
				p[i] = 0;
				for(j = 0; j < 10; j ++ )
					p[i] += R[j][i] * u[j] / h;
			}
			for(i = 0; i < 10; i++){
				for(j = 0; j < 10; j++)
					R[i][j] = R[i][j] - u[i] * p[j];
			}
		}

	}//循环结束
	zeroMat(R);zeroMat(q);zeroMat(RQ);
	cout << endl << "Matrix Q is:" << endl;
	printMat(q);
	cout << endl << "Matrix R is:" << endl;
	printMat(R);
	muiltiplyMat(R, q, RQ, 10);
	cout << endl << "Matrix RQ is :" << endl;
	printMat(RQ);
}
//带双步位移的QR分解的操作
void iterate(double **M, double **a, int m)
{
	int r, i, j;
	double c, d, h, t, u[10], v[10], p[10], q[10], w[10];
	for (r = 0; r<m - 1; r++) {
		zeroMat(M);
		c = 0; d = 0; h = 0;
		for (i = r + 1; i<m; i++)
			d += M[i][r] * M[i][r];
		if (fabs(d) == 0)
			continue;
		else {
			d += M[r][r] * M[r][r];
			d = pow(d, 0.5);
			if (M[r][r] != 0)
				c = -fabs(M[r][r]) / M[r][r] * d;
			else
				c = d;
			h = c*c - c*M[r][r];
			for (i = 0; i<r; i++)
				u[i] = 0;
			u[r] = M[r][r] - c;
			for (i = r + 1; i<m; i++)
				u[i] = M[i][r];
			for (i = 0; i<m; i++) {
				v[i] = 0;
				for (j = 0; j<m; j++)
					v[i] += M[j][i] * u[j] / h;
			}
			for (i = 0; i<m; i++) {
				p[i] = 0; q[i] = 0;
				for (j = 0; j<m; j++) {
					M[i][j] -= u[i] * v[j];
					p[i]    += a[j][i] * u[j] / h;
					q[i]    += a[i][j] * u[j] / h;
				}
			}
			t = 0;
			for (i = 0; i<m; i++)
				t += p[i] * u[i] / h;
			for (i = 0; i<m; i++)
				w[i] = q[i] - t*u[i];
			for (i = 0; i<m; i++) {
				for (j = 0; j<m; j++) {
					a[i][j] -= (w[i] * u[j] + u[i] * p[j]);
				}
			}
		}
	}
}

//选主元
void maxline(double **ptr, int n, int k)
{
	double c;
	int i, M;
	M = k;
	for (i = k; i<n; i++)
	{
		if (fabs(ptr[i][k])>fabs(ptr[M][k]))
			M = i;
	}
	if (M>k)
	{
		for (i = k; i<n + 1; i++) {
			c         = ptr[k][i];
			ptr[k][i] = ptr[M][i];
			ptr[M][i] = c;
		}
	}
}

//高斯消元，求特征向量
void gauss(double lambda)
{
	double **a;
	double *X;//特征向量
	double m, sigma;
	int i, j, k, n = 10;
	X = (double *)malloc(10 * sizeof(double));
	initMat(a);

	for (i = 0; i<n; i++) {//A-lambda*I
		a[i][i]  -= lambda;
		a[i][10] = 0;
	}

	for (k = 0; k<n - 1; k++) {//消元过程
		maxline(a, n, k);//选主元
		for (i = k + 1; i<n; i++) {
			m = a[i][k] / a[k][k];
			for (j = k; j<n + 1; j++)
				a[i][j] = a[i][j] - m*a[k][j];
		}
	}

	X[n - 1] = 1;//系数矩阵非满秩的，求非零解，置最后一个分量为1
	for (k = n - 2; k >= 0; k--) {//回带过程
		sigma = 0;
		for (j = k + 1; j<n; j++)
			sigma += a[k][j] * X[j];
		X[k] = (a[k][n] - sigma) / a[k][k];
	}
	printf("eigenvector: ( ");
	for (i = 0; i<n; i++)
		printf("%lf ", X[i]);
	printf(")\n");
	for (i = 0; i<10; i++)
		free(a[i]);
	free(a);
	free(X);
}


void QRmethod(double **a)
{
	int k, m, i, j, r;
	double s, t, delt;
	double **M;
	ComplexNumber L[10];
	M = (double**)malloc(10 * sizeof(double *));
	for (k = 0; k<10; k++)
		M[k] = (double*)malloc(10 * sizeof(double));

	m = 9; r = 0;
	for (k = 0; k<Max; k++)//QR分解，求解特征值
	{//此时A已经拟上三角化
		if (m == 0) {//m从0开始，相当于书中等于1的情
			L[r].Re = a[m][m];
			L[r].Im = 0; break;
		}
		else if (m < 0) {
			break;
		}
		if (fabs(a[m][m - 1])<e) {//判断能否降1阶
			L[r].Re = a[m][m];
			L[r].Im = 0;
			m--; r++;
		}
		else {
			if (m == 1) //m从0开始，相当于书中等于2的情况
			{
				delt = (a[m][m] + a[m - 1][m - 1])*(a[m][m] + a[m - 1][m - 1]) - 4 * (a[m][m] * a[m - 1][m - 1] - a[m - 1][m] * a[m][m - 1]);
				if (delt >= 0) {
					L[r].Re     = (a[m][m] + a[m - 1][m - 1]) / 2 + sqrt(delt) / 2;//求根公式
					L[r].Im     = 0;
					L[r + 1].Re = (a[m][m] + a[m - 1][m - 1]) / 2 - sqrt(delt) / 2;
					L[r + 1].Im = 0;
				}
				else {
					L[r].Re     = (a[m][m] + a[m - 1][m - 1]) / 2;
					L[r].Im     = sqrt(-delt) / 2;
					L[r + 1].Re = (a[m][m] + a[m - 1][m - 1]) / 2;
					L[r + 1].Im = -sqrt(-delt) / 2;
				}
				m -= 2;//降2阶
				r += 2;
				continue;
			}
			else if (fabs(a[m - 1][m - 2])<e) {
				delt = (a[m][m] + a[m - 1][m - 1])*(a[m][m] + a[m - 1][m - 1]) - 4 * (a[m][m] * a[m - 1][m - 1] - a[m - 1][m] * a[m][m - 1]);
				if (delt>0) {
					L[r].Re     = (a[m][m] + a[m - 1][m - 1]) / 2 + sqrt(delt) / 2;
					L[r].Im     = 0;
					L[r + 1].Re = (a[m][m] + a[m - 1][m - 1]) / 2 - sqrt(delt) / 2;
					L[r + 1].Im = 0;
				}
				else {
					L[r].Re     = (a[m][m] + a[m - 1][m - 1]) / 2;
					L[r].Im     = sqrt(-delt) / 2;
					L[r + 1].Re = (a[m][m] + a[m - 1][m - 1]) / 2;
					L[r + 1].Im = -sqrt(-delt) / 2;
				}
				m -= 2;
				r += 2;
				continue;
			}
			else {
				s = a[m - 1][m - 1] + a[m][m];
				t = a[m - 1][m - 1] * a[m][m] - a[m][m - 1] * a[m - 1][m];
				muiltiplyMat(a, a, M, m + 1);//M=a*a,the power is m+1
				for (i = 0; i<10; i++)
				{
					for (j = 0; j<10; j++)
						M[i][j] -= s*a[i][j];
					M[i][i] += t;
				}
				iterate(M, a, m + 1);//带双步位移的QR分解的操作
				zeroMat(a);
			}
		}
	}
	zeroMat(a);
	printMat(a);

	for (int r = 0; r<10; r++) {
		if (L[r].Im == 0) {
			printf("lambda[%d]=%e\n", r + 1, L[r].Re);
			gauss(L[r].Re);//求特征向量
		}
		else {
			printf("lambda[%d]=%e+i*%e\n", r + 1, L[r].Re, L[r].Im);
		}
	}
}

int main()
{
	double **a;
	int i;
	initMat(a);
	hessenbergMat(a);//将矩阵拟上三角化
	zeroMat(a);//满足精度要就的元素置0
	printMat(a);//A(n-1)
	QR_Matrix(a);//求Q、R和RQ
	QRmethod(a);
	system("pause");
	return 0;
}
