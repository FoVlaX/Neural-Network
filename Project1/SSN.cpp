#include "SSN.h"
#include <fstream>
SSN::SSN(int n,  vector<pair<int,int>> sizes,int a1, int b1,int out,int vn, int vm, int plng)
{
	polng = plng;
	W.resize(n);
	W1.resize(n);
	W2.resize(n);
	NET = obj;
	deep = n;
	in = a1;
	im = b1;
	d_W.resize(deep);
	d_W1.resize(deep);
	d_W2.resize(deep);
	pool.resize(deep);
	pool1.resize(deep);
	pool2.resize(deep);
	P_index.resize(deep);
	P_index1.resize(deep);
	P_index2.resize(deep);

	C_index.resize(deep);
	C_index1.resize(deep);
	C_index2.resize(deep);
	for (int i = 0; i < n; i++)
	{
		int size = sizes[i].first *sizes[i].second;
		W[i].init(size,1);
		W1[i].init(size, 1);
		W2[i].init(size, 1);
	}
	dW = W;
	dW1 = W1;
	dW2 = W2;
	Wsizes = sizes;
	int ss = getsize(a1, b1, polng);
	obj = new NeNet(vn,vm,ss,out);
	objrgb = new NeNet(vn, vm, 3 * ss,out);
}

matrix SSN::convolution(matrix &a, matrix &w, int &an, int &am, int &wn, int &wm, matrix &dW, int &step, pair <int,int> &Ci)
{
	matrix ans;
	int nn = (int)ceil(double(an - wn + 1) / double(step));
	int nm = (int)ceil(double(am - wm + 1) / double(step));

	Ci.first = an;
	Ci.second = am;

	int nsize = (nn)*(nm);
	dW.init(nsize,wn*wm);
	ans.init(nsize, 1);

	int nc = (int)ceil(double(wn) / 2.0);
	int mc = (int)ceil(double(wm) / 2.0);
	   
	int num = 0;

	for (int i = nc; i < an; i+=step)
	{
		for (int j = mc; j < am; j+=step)
		{
			if (i + wn < an && j + wm < am)
			{
				double sum = 0;
				double max = 0;
				for (int k = 0; k < wn; k++)
				{
					for (int l = 0; l < wm; l++)
					{
						sum += a.x[gix(i + k-nc, j + l-mc, am)][0] * w.x[gix(k,l,wm)][0];
						dW.x[num][gix(k, l, wm)] = a.x[gix(i + k-nc, j + l-mc, am)][0];
 					}
				}
				ans.x[num][0] = sum/(wm*wn);
			}
			num += 1;
		}
	}
	an = nn;
	am = nm;
	return ans;

}
matrix SSN::pooling(matrix &a, int &n,int &m,int &z,matrix &dInd)
{
	int nn = n/z;
	int nm = m/z;
	matrix b;
	b.init(nn*nm, 1);
	dInd.init(nn*nm, 1);
	int ind = 0;
	for (int i = 0, k = 0; i < a.a; i += z,k+=1)
	{
		for (int j = 0, l = 0; j < a.b; j += z, l += 1)
		{
			
			if (i + z < a.a && j + z < a.b)
			{
				double max = 0;
				for (int o = 0; o < z; o++)
				{
					for (int p = 0; p < z; p++)
					{
						if (a.x[gix(i + o, j + p, m)][0] > max)
						{
							max = a.x[gix(i + o, j + p, m)][0];
							ind = gix(i + o, j + p, m);
						}
					}
				}
				b.x[gix(k, l, nm)][0] = max;
				dInd.x[gix(k, l, nm)][0] = ind;
				
			}
		}
	}

	n = nn;
	m = nm;

	return b;
}
int SSN::gix(int i, int j, int m)
{
	return i * m + j;
}

matrix SSN::vectorization(matrix a)
{
	matrix b;
	b.init(a.a*a.b , 1);
	int h = 0;
	for (int i = 0; i < a.a; i++)
	{
		for (int j = 0; j < a.b; j++)
		{
			b.x[h][0] = a.x[i][j];
			h++;
		}
	}
	return b; 
}
matrix SSN::matrixization(matrix a, int n, int m) {
	matrix b;
	b.init(n, m);
	if (n*m == a.a)
	{
		int h = 0;
		for (int i = 0; i < b.a; i++)
		{
			for (int j = 0; j < b.b; j++)
			{
				b.x[i][j] = a.x[h][0];
				h++;
			}
		}
	}
	return b;
}


matrix SSN::feedfoward(matrix a)
{
	int in1 = in;
	int im1 = im;
	for (int i = 0; i < deep; i++)
	{
		a = convolution(a, W[i], in1, im1, Wsizes[i].first, Wsizes[i].second, d_W[i],stp,C_index[i]);
		pool[i].first = in1;
		pool[i].second = im1;
		a = pooling(a, in1, im1, polng, P_index[i]);
	}
	a = obj->feedfoward(a).ans;
	return a;
}
matrix SSN::feedfoward_ft(matrix a) {
	int in1 = in;
	int im1 = im;
	for (int i = 0; i < deep; i++)
	{
		a = convolution(a, W[i], in1, im1, Wsizes[i].first, Wsizes[i].second, d_W[i],stp, C_index[i]);
		pool[i].first = in1;
		pool[i].second = im1;
		a = pooling(a, in1, im1, polng, P_index[i]);
	}
	return a;
}

int SSN::getsize(int a, int b, int pooling)
{
	int n = a;
	int m = b;
	for (int i = 0; i < deep; i++)
	{
		n = n - Wsizes[i].first + 1;
		m = m - Wsizes[i].second + 1;
		n /= pooling;
		m /= pooling;
	}
 	return n * m;
}

matrix SSN::antconv(matrix a, matrix w, int &an, int &am, int wn, int wm, int step, pair <int, int> &Ci)
{
	w.R();
	matrix ans;
	int nn = Ci.first;
	int nm = Ci.second;
	int nsize = (nn)*(nm);
	int nc = ceil(double(wn) / 2.0);
	int mc = ceil(double(wm) / 2.0);

	ans.init(nsize, 1);

	for (int i = 0; i < nn; i++)
	{
		for (int j = 0; j < nm; j++)
		{

				double sum = 0;
				for (int k = -nc; k < wn-nc; k++)
				{
					for (int l = -mc; l < wm-mc; l++)
					{
						if (i + k < an && j + l < am && i+k>=0 && j+l>=0)
						{
							sum += a.x[gix(i + k, j + l, am)][0] * w.x[gix(k+nc, l+mc, wm)][0];
						}
					}
				}
				ans.x[gix(i, j, nm)][0] = sum / (wn*wm);
			
		}
	}
	an = nn;
	am = nm;
	return ans;
}

void SSN::Teach(matrix a, matrix out)
{
	matrix g;
	int in1 = in, im1 = im;
	
	for (int i = 0; i < deep; i++)
	{
		in1 = in1 - Wsizes[i].first + 1;
		im1 = im1 - Wsizes[i].second + 1;
		in1 /= polng;
		im1 /= polng;
	}

 	a = feedfoward_ft(a);
 	obj->teach(a, out, g);
	
	for (int i = deep - 1; i >= 0; i--)
	{
		g = antpooling(g,in1,im1,i, pool);
		dW[i] = (g.T()*d_W[i]).T();
		g = antconv(g, W[i], in1, im1, Wsizes[i].first, Wsizes[i].second);
		W[i] = W[i] + dW[i];
	}

}

matrix SSN::antpooling(matrix &a, int &n, int &m, int &h,vector<pair<int,int>> &pool)
{
	int nn = pool[h].first;
	int nm = pool[h].second;
	matrix b;
	b.initnull(nm*nn, 1);
	int ss = n * m;
	for (int i = 0; i < ss; i++)
	{
		b.x[P_index[h].x[i][0]][0] = a.x[i][0];
	}
	n = nn;
	m = nm;
	return b;
}

void SSN::save(string str)
{
	obj->save("N"+str);
	objrgb->save("Nrgb" + str);
	ofstream out(str);
	out << deep << endl;
	for (int i = 0; i < deep; i++)
	{
		out << Wsizes[i].first << " " << Wsizes[i].second << endl;
		for (int k = 0; k < Wsizes[i].first; k++)
		{
			for (int h = 0; h < Wsizes[i].second; h++)
			{
				out << W[i].x[gix(k, h, Wsizes[i].second)][0] << " ";
				out << W1[i].x[gix(k, h, Wsizes[i].second)][0] << " ";
				out << W2[i].x[gix(k, h, Wsizes[i].second)][0] << " ";
			}
			out << endl;
		}
	}
}

void SSN::load(string str)
{
	obj->load("N" + str);
	objrgb->load("Nrgb" + str);
	ifstream out(str);
	out >> deep;
	for (int i = 0; i < deep; i++)
	{
		out >> Wsizes[i].first >> Wsizes[i].second;
		for (int k = 0; k < Wsizes[i].first; k++)
		{
			for (int h = 0; h < Wsizes[i].second; h++)
			{
				out >> W[i].x[gix(k, h, Wsizes[i].second)][0];
				out >> W1[i].x[gix(k, h, Wsizes[i].second)][0];
				out >> W2[i].x[gix(k, h, Wsizes[i].second)][0];
			}
			
		}
	}
}

matrix SSN::ff3canel(matrix r, matrix g, matrix b) {
	int in1 = in;
	int im1 = im;
	int in2 = in;
	int im2 = im;
	int in3 = in;
	int im3 = im;
	for (int i = 0; i < deep; i++)
	{
		r = convolution(r, W[i], in1, im1, Wsizes[i].first, Wsizes[i].second, d_W[i],stp, C_index[i]);
		pool[i].first = in1;
		pool[i].second = im1;
		r = pooling(r, in1, im1, polng, P_index[i]);

		g = convolution(g, W1[i], in2, im2, Wsizes[i].first, Wsizes[i].second, d_W1[i], stp, C_index1[i]);
		pool1[i].first = in2;
		pool1[i].second = im2;
		g = pooling(g, in2, im2, polng, P_index1[i]);

		b = convolution(b, W2[i], in3, im3, Wsizes[i].first, Wsizes[i].second, d_W2[i], stp, C_index2[i]);
		pool2[i].first = in3;
		pool2[i].second = im3;
		b = pooling(b, in3, im3, polng, P_index2[i]);
	}
	matrix c;

	c = scepka(r,g,b);

	c = objrgb->feedfoward(c).ans;
	return c;
}


matrix SSN::ff3canel_r(matrix r, matrix g, matrix b) {
	int in1 = in;
	int im1 = im;
	int in2 = in;
	int im2 = im;
	int in3 = in;
	int im3 = im;
	for (int i = 0; i < deep; i++)
	{
		r = convolution(r, W[i], in1, im1, Wsizes[i].first, Wsizes[i].second, d_W[i], stp, C_index[i]);
		pool[i].first = in1;
		pool[i].second = im1;
		r = pooling(r, in1, im1, polng, P_index[i]);

		g = convolution(g, W1[i], in2, im2, Wsizes[i].first, Wsizes[i].second, d_W1[i], stp, C_index1[i]);
		pool1[i].first = in2;
		pool1[i].second = im2;
		g = pooling(g, in2, im2, polng, P_index1[i]);

		b = convolution(b, W2[i], in3, im3, Wsizes[i].first, Wsizes[i].second, d_W2[i], stp, C_index2[i]);
		pool2[i].first = in3;
		pool2[i].second = im3;
		b = pooling(b, in3, im3, polng, P_index2[i]);
	}
	matrix c;

	c = scepka(r, g, b);

	return c;
}


matrix SSN::scepka(matrix r, matrix g, matrix b)
{
	matrix c;
	c.init(r.a + g.a + b.a, 1);
	for (int i = 0; i < r.a; i++)
	{
		c.x[i][0] = r.x[i][0];
		c.x[i + r.a][0] = g.x[i][0];
		c.x[i + 2 * r.a][0] = b.x[i][0];
	}
	return c;
}

void SSN::rasscepka(matrix &r, matrix &g, matrix &b, matrix c) {
	
	int ss = c.a / 3;

	r.init(ss, 1);
	g.init(ss, 1);
	b.init(ss, 1);
	
	for (int i = 0; i < ss; i++)
	{
		r.x[i][0] = c.x[i][0];
		g.x[i][0] = c.x[i + r.a][0];
		b.x[i][0] = c.x[i + 2 * r.a][0];
	}

}

void SSN::Teach3(matrix r, matrix g, matrix b, matrix out)
{
	matrix j;
	int in1 = in, im1 = im;

	for (int i = 0; i < deep; i++)
	{
		in1 = in1 - Wsizes[i].first + 1;
		im1 = im1 - Wsizes[i].second + 1;
		in1 /= polng;
		im1 /= polng;
	}
	int im2 = im1;
	int in2 = in1;
	int in3 = in1;
	int im3 = im1;

	matrix a;
	a = ff3canel_r(r,g,b);
	objrgb->teach(a, out, j);


	rasscepka(r, g, b, j);

	for (int i = deep - 1; i >= 0; i--)
	{
		r = antpooling(r, in1, im1, i, pool);
		dW[i] = (r.T()*d_W[i]).T();
		r = antconv(r, W[i], in1, im1, Wsizes[i].first, Wsizes[i].second);
		W[i] = W[i] + dW[i];

		g = antpooling(g, in2, im2, i, pool1);
		dW1[i] = (g.T()*d_W1[i]).T();
		g = antconv(g, W1[i], in2, im2, Wsizes[i].first, Wsizes[i].second);
		W1[i] = W1[i] + dW1[i];

		b = antpooling(b, in3, im3, i, pool2);
		dW2[i] = (b.T()*d_W2[i]).T();
		b = antconv(b, W2[i], in3, im3, Wsizes[i].first, Wsizes[i].second);
		W2[i] = W2[i] + dW2[i];
	}
}