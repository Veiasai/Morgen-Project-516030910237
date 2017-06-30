#include <iostream>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <string>
#include <sstream>


const int hours_num = 7;
const int minutes_num = 60;

using namespace std;
int main()
{
	fstream f("iceberg.csv");
	double num[hours_num][minutes_num] = {};
	int h, m;
	double num_int1 = 0;
	double num_int2 = 0;
	double all_num = 0;
	while (!f.eof())
	{
		string s;
		getline(f, s);
		if (s[0] != '2')
		{
			all_num += num_int1;
			num_int2 = 0;
			num_int1 = 0;
			continue;
		}
		h = (s[11] - '0') * 10 + (s[12] - '0');
		m = (s[14] - '0') * 10 + (s[15] - '0');
		string num_s = s.substr(26);
		stringstream ss;
		ss << num_s;
		ss >> num_int1;
		num[h-9][m] += (num_int1 - num_int2);
		num_int2 = num_int1;
	}

	/*for (int i = 0; i < 7; i++)
		for (int j = 0; j < 20; j++)
		{
			double now_num = 0;
			for (int k = 0; k < 3; k++)
				now_num += num[i][j * 3 + k];

			for (int k = 0; k < 3; k++)
				num[i][j * 3 + k] = now_num * (k + 1) / 3;
		}*/
			
	int now_num = 0;
	FILE* fout;
	fopen_s(&fout, "icebergout.txt", "w+");
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 60; j++)
		{
			fprintf(fout,"%2d",i+9);
			fprintf(fout,":");
			fprintf(fout,"%02d", j);
			fprintf(fout, ":00, ");
			now_num += num[i][j];
			double x = num[i][j] / all_num;
			double y = now_num / all_num;
			fprintf(fout,"%6f", x);
			fprintf(fout, "--");
			fprintf(fout,"%6f", y);
			fprintf(fout, "\n");
		}
	cout << "End" << endl;
}