#pragma once
#include "NeNet.h"

class SSN {
public:
	SSN(int n, vector<pair<int, int>>  sizes, int a1,int b1,int out, int vn, int vm, int plng);
	matrix convolution(matrix &a, matrix &w,int &an, int &am,int &wn,int &wm, matrix &dW, int &step, pair<int,int> &Ci); //операция свертки
	int gix(int i, int j, int m); //получение индекса в векторе соответсующему двумерный координатам
	matrix pooling(matrix &a, int &n, int &m, int &z, matrix &dInd);//выделение максимумов
	matrix antpooling(matrix &a, int &n, int &m,  int &h, vector<pair<int, int>> &pool);
	matrix feedfoward(matrix a);
	matrix feedfoward_ft(matrix a);
	matrix ff3canel(matrix r, matrix g, matrix b);
	matrix vectorization(matrix a);
	matrix matrixization(matrix a, int n, int m);
	int getsize(int a, int b, int pooling);
	matrix antconv(matrix a, matrix w, int &an, int &am, int wn, int wm, int step, pair <int, int> &Ci);
	void Teach(matrix a, matrix out);
	void Teach3(matrix r,matrix g,matrix b, matrix out);
	matrix ff3canel_r(matrix r, matrix g, matrix b);
	void save(string str);
	void load(string str);
private:
	int polng;
	NeNet *obj;
	NeNet *objrgb;
	int in, im;
	NeNet *NET;
	int deep;
	matrix scepka(matrix r, matrix g, matrix b);
	void rasscepka(matrix &r, matrix &g, matrix &b,matrix c);
	vector<pair<int, int>> pool;
	vector<pair<int, int>> pool1;
	vector<pair<int, int>> pool2;
	int stp = 1;
	vector<matrix> W;
	vector<matrix> d_W;
	vector<matrix> dW;

	vector<matrix> W1;
	vector<matrix> d_W1;
	vector<matrix> dW1;

	vector<matrix> W2;
	vector<matrix> d_W2;
	vector<matrix> dW2;

	vector<matrix> P_index;
	vector<matrix> P_index1;
	vector<matrix> P_index2;

	vector<pair<int, int>> C_index;
	vector<pair<int, int>> C_index1;
	vector<pair<int, int>> C_index2;

	vector<pair<int, int>> Wsizes;
};