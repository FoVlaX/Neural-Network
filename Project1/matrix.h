#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;

class matrix
{
private:
	int prec = 4;
	
public:
	int a, b;
	void R();
	vector<vector<double>> x;
	matrix();
	void precision(int t);
	void init(int a2, int b2);
	void initnull(int a2, int b2);
	matrix T();
	matrix operator+ (matrix t);
	matrix operator- (matrix t);
	matrix operator* (matrix t);
	matrix operator* (double t);
	friend matrix operator+(double y, matrix t)
	{
		for (int i = 0; i < t.a; i++)
		{
			for (int j = 0; j < t.b; j++)
			{
				t.x[i][j] += y;
			}
		}
		return t;
	}

	friend matrix operator+(matrix t,double y)
	{
		for (int i = 0; i < t.a; i++)
		{
			for (int j = 0; j < t.b; j++)
			{
				t.x[i][j] += y;
			}
		}
		return t;
	}

	friend matrix operator-(double y, matrix t)
	{
		for (int i = 0; i < t.a; i++)
		{
			for (int j = 0; j < t.b; j++)
			{
				t.x[i][j] = y - t.x[i][j];
			}
		}
		return t;
	}

	friend matrix operator-( matrix t, double y)
	{
		for (int i = 0; i < t.a; i++)
		{
			for (int j = 0; j < t.b; j++)
			{
				t.x[i][j] = t.x[i][j] - y ;
			}
		}
		return t;
	}

	friend matrix operator*(double y,matrix t)
	{
		for (int i = 0; i < t.a; i++)
		{
			for (int j = 0; j <  t.b; j++)
			{
				t.x[i][j] *= y;
			}
		}
		return t;
	}


	friend matrix operator/(double left, matrix t)
	{
		for (int i = 0; i <  t.a; i++)
		{
			for (int j = 0; j < t.b; j++)
			{
				t.x[i][j] = left/ t.x[i][j];
			}
		}
		return t;
	}

	friend matrix operator/(matrix t, double left)
	{
		for (int i = 0; i < t.a; i++)
		{
			for (int j = 0; j < t.b; j++)
			{
				t.x[i][j] =  t.x[i][j]/left;
			}
		}
		return t;
	}

	void print();
	matrix operator<<(const char *str);
	matrix expo(matrix t);
};

