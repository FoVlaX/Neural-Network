#pragma once
#include "matrix.h"
#include <vector>
#include <string>
using namespace std;

struct answer
{
	matrix ans;
	vector<matrix> f;
	vector<matrix> h;
};

class NeNet {
public:
	NeNet(int n,int m, int In, int Out);
	answer feedfoward(matrix a);
	matrix Afun(matrix a);
	matrix d_Afun(matrix a);
	void teach(matrix In, matrix Out, matrix &forSNS);
	void save(string str);
	void load(string str);
private:
	vector<matrix> W;
	vector<matrix> b;
	int deep;
	int width;
};