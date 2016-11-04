#include "stdafx.h"
#include "m01_05_cal_direction.h"

#include "io_outop.h"

using namespace std;

/*
Calculate the orientation's direction of the subject in the calibiration process and in the dynamic process with respect to the orientation defined by ISB coordinate system
Written by Ziyun Ding
ISB coordinate system's direction:
X positive: point to anterior;
Y positive: point to superior;
Z positive: point to lateral.
*/

void m01_05_cal_direction(Vec_DP &RASIS, Vec_DP &LASIS, Vec_DP &RPSIS, Vec_DP &FAM, int sign[3])
{
	for (int ii=0; ii<3; ii++)
	{
		sign[ii] = 1;
	}

	if (RASIS[0]<RPSIS[0])
	{sign[0] = -1;}

	if (RASIS[1]<FAM[1])
	{sign[1] = -1;}

	if(RASIS[2]<LASIS[2])
	{sign[2] = -1;}


	//cout<<sign[0]<<endl;
	//cout<<sign[1]<<endl;
	//cout<<sign[2]<<endl;



	return ;
}


void m01_05_cal_direction(Vec_DP &RASIS, Vec_DP &LASIS, Vec_DP &RPSIS, int y_sign, int sign[3])
{
	for (int ii=0; ii<3; ii++)
	{
		sign[ii] = 1;
	}

	if (RASIS[0]<RPSIS[0])
	{sign[0] = -1;}

	sign[1] = y_sign;

	if(RASIS[2]<LASIS[2])
	{sign[2] = -1;}


	//cout<<sign[0]<<endl;
	//cout<<sign[1]<<endl;
	//cout<<sign[2]<<endl;

	return ;
}