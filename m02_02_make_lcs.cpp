#include "stdafx.h"
#include "m02_02_make_lcs.h"
#include "io_datainput.h"

using namespace std;

/*
	Create the coordinate system for each segment by specifying the transformation from the LCS to the calibration position
	Re-written by Ziyun Ding, Imperial College (c) 2014,
	based on research and code by Dan Cleather.
*/

void m02_02_make_lcs(Structure *calibrate_pos[], int segments)
{
	
	Mat_DP axes(3,3);

	int X, Y, Z;
	X=0; Y=1; Z=2;
   
	Vec_DP temp_axis_x(3), temp_axis_y(3), temp_axis_z(3), axis_x(3), axis_y(3), axis_z(3), q(4);
	
	

	/* 
	Foot: LCS to GCS using calibration data
	*/
	for (int ii=0; ii<3; ii++)
	{
		axis_y[ii] = calibrate_pos[0]->landmarks[1][ii]-calibrate_pos[0]->landmarks[1][ii+3];
		temp_axis_z[ii] = calibrate_pos[0]->landmarks[2][ii]-calibrate_pos[0]->landmarks[2][ii+3];
	}
	axis_y = MATHEMATICS::math_vecnorm(axis_y);
	temp_axis_z = MATHEMATICS::math_vecnorm(temp_axis_z);
	axis_x = MATHEMATICS::math_crossprd(axis_y,temp_axis_z);
	axis_x = MATHEMATICS::math_vecnorm(axis_x);
	axis_z = MATHEMATICS::math_crossprd(axis_x,axis_y);
	axis_z = MATHEMATICS::math_vecnorm(axis_z);

	MATHEMATICS::math_zeromatrix(axes);
	for (int ii=0; ii<3; ii++)
	{
		axes[X][ii] = axis_x[ii];
		axes[Y][ii] = axis_y[ii];
		axes[Z][ii] = axis_z[ii];
	}

	axes=MATHEMATICS::math_transpose(axes);

	KINEMATICS::kin_qcalc(axes,q);

	for (int kk=0; kk<4; kk++) 
	{	
		calibrate_pos[0]->rot_lcs_to_gcs[kk]=q[kk];
	}

	
	/*
	Shank: LCS to GCS using calibration data
	*/
	for (int ii=0; ii<3; ii++)
	{
		axis_y[ii] = calibrate_pos[1]->landmarks[1][ii]-calibrate_pos[1]->landmarks[1][ii+3];
		temp_axis_z[ii] = calibrate_pos[1]->landmarks[2][ii]-calibrate_pos[1]->landmarks[2][ii+3];
	}
	axis_y = MATHEMATICS::math_vecnorm(axis_y);
	temp_axis_z = MATHEMATICS::math_vecnorm(temp_axis_z);
	axis_x = MATHEMATICS::math_crossprd(axis_y,temp_axis_z);
	axis_x = MATHEMATICS::math_vecnorm(axis_x);
	axis_z = MATHEMATICS::math_crossprd(axis_x,axis_y);
	axis_z = MATHEMATICS::math_vecnorm(axis_z);

	MATHEMATICS::math_zeromatrix(axes);
	for (int ii=0; ii<3; ii++)
	{
		axes[X][ii] = axis_x[ii];
		axes[Y][ii] = axis_y[ii];
		axes[Z][ii] = axis_z[ii];
	}

	axes=MATHEMATICS::math_transpose(axes);

	KINEMATICS::kin_qcalc(axes,q);

	for (int kk=0; kk<4; kk++) 
	{	
		calibrate_pos[1]->rot_lcs_to_gcs[kk]=q[kk];
	}
	

	/* 
	Thigh: LCS to GCS using calibration data
	*/
	for (int ii=0; ii<3; ii++)
	{
		axis_y[ii] = calibrate_pos[2]->landmarks[1][ii]-calibrate_pos[2]->landmarks[1][ii+3];
		temp_axis_z[ii] = calibrate_pos[2]->landmarks[2][ii]-calibrate_pos[2]->landmarks[2][ii+3];
	}
	axis_y = MATHEMATICS::math_vecnorm(axis_y);
	temp_axis_z = MATHEMATICS::math_vecnorm(temp_axis_z);
	axis_x = MATHEMATICS::math_crossprd(axis_y,temp_axis_z);
	axis_x = MATHEMATICS::math_vecnorm(axis_x);
	axis_z = MATHEMATICS::math_crossprd(axis_x,axis_y);
	axis_z = MATHEMATICS::math_vecnorm(axis_z);

	MATHEMATICS::math_zeromatrix(axes);
	for (int ii=0; ii<3; ii++)
	{
		axes[X][ii] = axis_x[ii];
		axes[Y][ii] = axis_y[ii];
		axes[Z][ii] = axis_z[ii];
	}

	axes=MATHEMATICS::math_transpose(axes);

	KINEMATICS::kin_qcalc(axes,q);

	for (int kk=0; kk<4; kk++) 
	{	
		calibrate_pos[2]->rot_lcs_to_gcs[kk]=q[kk];
	}
	

	/* 
	Pelvis: LCS to GCS using calibration data
	*/
	for (int ii=0; ii<3; ii++)
	{
		temp_axis_x[ii] = calibrate_pos[3]->landmarks[0][ii]-calibrate_pos[3]->landmarks[0][ii+3];
		axis_z[ii] = calibrate_pos[3]->landmarks[2][ii]-calibrate_pos[3]->landmarks[2][ii+3];
	}
	temp_axis_x = MATHEMATICS::math_vecnorm(temp_axis_x);
	axis_z = MATHEMATICS::math_vecnorm(axis_z);
	axis_y = MATHEMATICS::math_crossprd(axis_z,temp_axis_x);
	axis_y = MATHEMATICS::math_vecnorm(axis_y);
    axis_x = MATHEMATICS::math_crossprd(axis_y,axis_z);
	axis_x = MATHEMATICS::math_vecnorm(axis_x);

	MATHEMATICS::math_zeromatrix(axes);
	for (int ii=0; ii<3; ii++)
	{
		axes[X][ii] = axis_x[ii];
		axes[Y][ii] = axis_y[ii];
		axes[Z][ii] = axis_z[ii];
	}

	axes=MATHEMATICS::math_transpose(axes);


	KINEMATICS::kin_qcalc(axes,q);

	for (int kk=0; kk<4; kk++) 
	{	
		calibrate_pos[3]->rot_lcs_to_gcs[kk]=q[kk];
	}


	/* 
	Patella: LCS to GCS, the same as thigh's
	*/
	for (int j=0; j<4; j++) 
	{	
		calibrate_pos[4]->rot_lcs_to_gcs[j]=0.0;
	}



	return;
}