#include "stdafx.h"
#include "nr.h"
#include "mathematics.h"
#include "kinematics.h"
using namespace std;



Vec_DP KINEMATICS::kin_vecrotate(Mat_DP axes, Vec_DP v0) 
{
	Vec_DP v1(3);

	for (int ii=0; ii<3; ii++)
	{
		for (int jj=0; jj<3; jj++)
		{

			v1[ii] = v1[ii]+axes[ii][jj]*v0[jj];
		}
	}

	return v1;
}