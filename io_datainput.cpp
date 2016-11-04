#include "stdafx.h"
#include "io_inop.h"
#include "io_datainput.h"

/*

Input all data

*/
Vec_DP io_datainput(string filename, int k) 
{

	int n;
	ifstream file(filename.c_str());

	if( !file.is_open() )
	{
		cout<< "\nError in file named ' "
		<< filename
		<< "' failed to open successfully. Aborting." << endl;
		getchar();
		exit(0);
	}



	file >> n;
	Vec_DP data(n);
	for (int i=0; i<n; i++) {
		file >> data[i];
	}
	file.close();

	return data;
}


Mat_DP io_datainput(string filename) 
{
	
	int x, y;

	ifstream file(filename.c_str());

	if( !file.is_open() )
	{
		cout<< "\nError in file named ' "
		<< filename
		<< "' failed to open successfully. Aborting." << endl;
		getchar();
		exit(0);
	}




	file >> x;
	file >> y;
	
	
	Mat_DP data(x,y);

	for (int i=0; i<x; i++) {
		for (int j=0; j<y; j++) {

			file >> data[i][j];

		}
	}

	file.close();

	return data;
}

void io_datainput(string filename, int muscles, Muscle *m[]) 
{

	ifstream infile(filename.c_str());

	for (int i=0; i<muscles; i++) 
	{
		m[i]=new Muscle;
		infile >> m[i]->name;
		infile >> m[i]->pnts;
		infile >> m[i]->segments_spanned;

			
		for (int k=0; k<m[i]->pnts; k++) 
		{
			for (int j=0; j<6; j++) 
			{
				infile >> m[i]->gblpnts[k][j];
			}

			
		}	
		infile >> m[i]->pcsa;
		infile >> m[i]->pcsa_multiple;
		infile >> m[i]->max_tension;
		infile >> m[i]->pennation_angle;
	}
	
	infile.close();
	
}

void io_datainput(Vec_DP &data, int index, string filename) 
{
	
	int num;
	string text;

	ifstream file(filename.c_str());


	if( !file.is_open() )
	{
		cout<< "\nError in file named ' "
		<< filename
		<< "' failed to open successfully. Aborting." << endl;
		getchar();
		exit(0);
	}

	do {
		do {
		file >> text;
		} while (text!="n");
		file >> num;
	} while (num!=index);
	file >> data;
	file.close();

	return;
}

void io_datainput(Mat_DP &data, Vec_DP markers, int segments, int index, string filename) 
{
	
	int num;
	string text;

	ifstream file(filename.c_str());

	if( !file.is_open() )
	{
		cout<< "\nError in file named ' "
		<< filename
		<< "' failed to open successfully. Aborting." << endl;
		getchar();
		exit(0);
	}

	do {
		do {
		file >> text;
		} while (text!="n");
		file >> num;
	} while (num!=index);
	for (int i=0; i<segments; i++) {
		for (int j=0; j<3*markers[i]; j++) {
			file >> data[i][j];
		}
	}
	file.close();

	return;
}

Mat_DP io_datainput(int frames, int num, string filename) 
{
	Mat_DP data(frames,num);

	ifstream file(filename.c_str());

	if( !file.is_open() )
	{
		cout<< "\nError in file named ' "
		<< filename
		<< "' failed to open successfully. Aborting." << endl;
		getchar();
		exit(0);
	}

	for (int i=0; i<frames; i++) 
	{
		for (int j=0; j<num; j++) 
		{
			file >> data[i][j];
		}
	}
	file.close();

	return data;
}

