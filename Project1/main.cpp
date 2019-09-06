#include "matrix.h"
#include "NeNet.h"
#include "SSN.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, char **argv)
{
	Mat img = imread("ops.jpeg");
	int n = img.rows;
	int m = img.cols;
	matrix r;
	r.init(n*m, 1);
	matrix g;
	g.init(n*m, 1);
	matrix b;
	b.init(n*m, 1);

	int h = 0;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			r.x[h][0] = (double)img.at<Vec3b>(i, j)[0] / 255;
			g.x[h][0] = (double)img.at<Vec3b>(i, j)[1] / 255;
			b.x[h][0] = (double)img.at<Vec3b>(i, j)[2] / 255;
			h++;
		}
	}

	vector<pair<int, int>> sizes(4);

	sizes[0].first = 2;
	sizes[0].second = 2;
	sizes[1].first = 2;
	sizes[1].second = 2;
	sizes[2].first = 2;
	sizes[2].second = 2;
	sizes[3].first = 2;
	sizes[3].second = 2;

	SSN network(4,sizes,n,m,4,4,60,2);
	matrix gras;
	


	matrix answer;
	answer.init(4, 1);
	answer.x[0][0] = 0.26;
	answer.x[1][0] = 0.44;
	answer.x[2][0] = 0.22;
	answer.x[3][0] = 0.33;

	/*
	for (int i = 0; i < 10; i++)
	{
		network.Teach3(r,g,b, answer);
		cout << i << endl;
	}
	network.save("network.txt");*/
	network.load("network.txt");
	gras = network.ff3canel(r,g,b);
	gras.print();
	cin >> h;
	return 0;
}