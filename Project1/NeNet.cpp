#include "NeNet.h"
#include <fstream>
using namespace std;
NeNet::NeNet(int n, int m, int In, int Out)
{
	W.resize(n + 1);
	b.resize(n + 1);
	deep = n;
	width = m;
	W[0].init(m, In);
	b[0].init(m, 1);
	for (int i = 1; i < n ; i++)
	{
		W[i].init(m, m);
		b[i].init(m, 1);
	}
	W[n].init(Out, m);
	b[n].init(Out, 1);
}

answer NeNet::feedfoward(matrix a)
{
	answer f;
	f.h.push_back(a);
	f.f.push_back(a);
	for (int i = 0; i < deep + 1; i++)
	{
		matrix s;
		s = W[i] * f.f[i] + b[i];

		
		f.h.push_back(s);
		s = Afun(s);
		
		f.f.push_back(s);
	}
	f.ans = f.f[f.f.size() - 1];
	return f;
}

void NeNet::teach(matrix In, matrix Out,matrix &forSNS)
{
	answer ans;
	ans = feedfoward(In);
	matrix y;
	y = ans.ans;
	vector<matrix> f;
	vector<matrix> h;
	vector<matrix> d_W;
	vector<matrix> d_b;
	f = ans.f;
	h = ans.h;
	d_W = W;
	d_b = b;
	int l = 1;
	matrix Y;
	Y = 2 * (Out - y)* d_Afun(h[deep + 1]);
	while (deep + 2 - l > 0)
	{
		d_b[deep + 1 - l] = Y;
		d_W[deep + 1 - l] = Y * f[deep + 1 - l].T();
		if (deep + 2 - l > 1) {
			Y = (Y.T()*W[deep + 1 - l]).T()*d_Afun(h[deep + 1 - l]);
		}

		l++;
	}
	for (int i = 0; i < deep + 1; i++)
	{
		b[i] = b[i] + d_b[i];
		W[i] = W[i] + d_W[i];
	}
	
	forSNS = (Y.T()*W[0]).T();

}

matrix NeNet::Afun(matrix a) {
	
	matrix b;
	b = 1 / (1 + a.expo(a));
	return b;
}
matrix NeNet::d_Afun(matrix a) {
	matrix b;
	matrix c;
	c = Afun(a);
	b = c * (1 - c);
	return b;
}

void NeNet::save(string str) {
	
	ofstream out(str);
	if (out.is_open())
	{
		out << deep << " " << width << "\n";
		out << W[0].x[0].size() << " " << W[W.size() - 1].x.size() << "\n";
		for (int k = 0; k < deep + 1; k++)
		{
			for (int i = 0; i < W[k].x.size(); i++)
			{
				for (int j = 0; j < W[k].x[i].size(); j++)
				{
					out << W[k].x[i][j] << "\n";
				}
				out << "\n";
			}

			for (int i = 0; i < b[k].x.size(); i++)
			{
				for (int j = 0; j < b[k].x[i].size(); j++)
				{
					out << b[k].x[i][j] << "\n";
				}
				out << "\n";
			}
		}
	}
	else
	{
		cout << "don't create file";
	}
	out.close();

}
void NeNet::load(string str) {
	ifstream in(str);
	if (in.is_open())
	{
		in >> deep >> width;
		int In, Out;
		in >> In >> Out;
		W.resize(deep + 1);
		b.resize(deep + 1);

		W[0].init(width, In);
		b[0].init(width, 1);
		for (int i = 1; i < deep; i++)
		{
			W[i].init(width, width);
			b[i].init(width, 1);
		}
		W[deep].init(Out, width);
		b[deep].init(Out, 1);
		for (int k = 0; k < deep + 1; k++)
		{
			for (int i = 0; i < W[k].x.size(); i++)
			{
				for (int j = 0; j < W[k].x[i].size(); j++)
				{
					in >> W[k].x[i][j];
				}

			}

			for (int i = 0; i < b[k].x.size(); i++)
			{
				for (int j = 0; j < b[k].x[i].size(); j++)
				{
					in >> b[k].x[i][j];
				}

			}
		}
	}
	else
	{
		cout << "don't open file";
	}
	in.close();
}