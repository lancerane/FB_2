#include "stdafx.h"
#include "io_outop.h"
#include "m01_01_mapcoord.h"

/*

Map lab coordinate system to Klein Horsman system

*/


void m01_01_mapcoord(Mat_DP &data, int orientation[3]) 
{

	double z[3];
	int sagittal, longitudinal, transverse;
	int X=0, Y=1, Z=2;
	sagittal = orientation[0];
	longitudinal = orientation[1];
	transverse = orientation[2];

	

	for (int i=0; i<data.nrows(); i++) 
	{
		for (int j=0; j<data.ncols(); j=j+3) 
		{
			z[0]=data[i][j];
			z[1]=data[i][j+1];
			z[2]=data[i][j+2];
			data[i][j+X]=z[sagittal];		
			data[i][j+Y]=z[longitudinal];
			data[i][j+Z]=z[transverse];
		}
		
	}
	

	return;
}


void m01_01_mapcoord(Mat_DP &data, int sign[3], double scale) 
{

	double z[3];	
	int X=0, Y=1, Z=2;

	for (int i=0; i<data.nrows(); i++) 
	{
		for (int j=0; j<data.ncols(); j=j+3) 
		{
			z[0]=data[i][j]*scale;
			z[1]=data[i][j+1]*scale;
			z[2]=data[i][j+2]*scale;
			data[i][j+X]=z[0]*sign[0];		
			data[i][j+Y]=z[1]*sign[1];
			data[i][j+Z]=z[2]*sign[2];
		}		
	}
	

	return;
}