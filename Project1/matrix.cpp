#include "matrix.h"
#include <sstream>
using namespace std;
matrix::matrix()
{
	
}

void matrix::init(int a2, int b2)
{
	x.resize(a2);
	a = a2;
	b = b2;
	for (int i = 0; i < a2; i++)
	{
		x[i].resize(b2);
		for (int j = 0; j < b2; j++)
		{
			x[i][j] =(double)(rand()%256)/51200;
		}
	}
}

void matrix::initnull(int a2, int b2)
{
	x.resize(a2);
	a = a2;
	b = b2;
	for (int i = 0; i < a2; i++)
	{
		x[i].resize(b2);
		for (int j = 0; j < b2; j++)
		{
			x[i][j] = 0;
		}
	}
}

matrix matrix::expo(matrix t) {
	for (int i = 0; i < a; i++)
	{
		
		for (int j = 0; j < b; j++)
		{
			t.x[i][j] = exp(-t.x[i][j]);
		}
	}

	return t;
}
matrix matrix::operator+ (matrix t) {


	if ((a == t.a) && (b == t.b))
	{
		
		for (int i = 0; i < a; i++)
		{
			
			for (int j = 0; j < b; j++)
			{
				x[i][j]+=t.x[i][j];
			}
		}

	
		return *this;
	}
	else
	{
		cout << "Size matrix don't save";
		exit(1);
	}

}

matrix matrix::operator- (matrix t) {


	if ((a == t.a) && (b == t.b))
	{


		
		for (int i = 0; i < a; i++)
		{
			
			for (int j = 0; j < b; j++)
			{
				x[i][j] -= t.x[i][j];
			}
		}

		
		return *this;
	}
	else
	{
		cout << "Size matrix don't save";
		exit(1);
	}

}
matrix matrix::T()
{
	vector<vector<double>> g;
	g.resize(b);

	for (int i = 0; i < b; i++)
	{
		g[i].resize(a);
		for (int j = 0; j < a; j++)
		{
			g[i][j] = x[j][i];
		}
	}
	matrix t;
	t.init(b, a);
	t.x = g;
	return t;
}
matrix matrix::operator*(matrix t)
{
	if (b == t.a)
	{
		vector<vector<double>> g;
		g.resize(a);
		for (int i = 0; i < a; i++)
		{
			g[i].resize(t.b);
			for (int j = 0; j < t.b; j++)
			{
				double sum = 0;
				for (int k = 0; k < b; k++)
				{
					sum += x[i][k] * t.x[k][j];
				}
				g[i][j] = sum;
			}
		}
		
		
		t.init(a, t.b);
		t.x = g;
		return t;
	}
	else
	{
		if (a == t.a && b == t.b)
		{
			
			for (int i = 0; i < a; i++)
			{
				for (int j = 0; j < b; j++)
				{

					 x[i][j]*=t.x[i][j];
				}
			}
		
			return *this;
		}
		else
		{
			cout << "error ";
			exit(1);
		}
	}
}

matrix matrix::operator* (double t)
{
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			x[i][j] *=t;
		}
	}
	
	return *this;
}

matrix matrix::operator<<(const char *str)
{
	int len = strlen(str);
	int count = 0;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '\n')
		{
			count++;
		}
	}
	int i = 0;
	int width = 0;
	while (str[i] != '\n')
	{
		i++;
		if (str[i] == ' ') width++;
	}
	width++;
	char **C = new char*[count + 1];
	for (int i = 0; i < count + 1; i++) C[i] = new char[100];
	count = 0;
	int k = 0;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '\n')
		{
			C[count][k] = '\0';
			count++;
			k = 0;
		}
		else
		{
			C[count][k] = str[i];
			k++;
		}
	}
	a = count+1;
	b = width;
	x.resize(a);

	for (int i = 0; i < a; i++) x[i].resize(b);
	for (int i = 0; i < a; i++)
	{
		istringstream input(C[i]);
		for (int j = 0; j < b; j++)
		{
			input >> x[i][j];
		}
	}
	return *this;
}

void matrix::print()
{
	cout.precision(prec);
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			cout << fixed<< x[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}
void matrix::R()
{
	int n = x.size()-1;
	for (int i = 0; i < x.size() / 2; i++)
	{
		swap(x[i], x[n - i]);
	}
}
void matrix::precision(int t) {
	prec = t;
}