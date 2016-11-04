#include "stdafx.h"
#include "m02_04_01_scaling.h"

#include "io_datainput.h"
#include "mathematics.h"
#include "kinematics.h"


using namespace std;

/*
		Calculate the scaling factors for the subject based upon the anatomical muscle model
		Re-written by Ziyun Ding, Imperial College (c) 2014,
		based on research and code by Dan Cleather.
*/

void m02_04_01_scaling(Structure *calibrate_pos[], int segments, XML_Parameters* _parameters)

{

	// Calculate rotation from LCS of antomical dataset subject 
	Vec_DP va(3), vb(3), vc(3), q(4);
	Vec_DP lengths(3);
	Mat_DP axes(3,3);

	int X, Y, Z;
	X=0; Y=1; Z=2;

    Vec_DP FAM(3), TAM(3), FM2(3), FLE(3), FME(3), hip_COR(3), RASIS(3), RPSIS(3), LASIS(3), LPSIS(3), v0(3);
	Vec_DP temp_axis_x(3), temp_axis_y(3), temp_axis_z(3), axis_x(3), axis_y(3), axis_z(3), width(3);

	double subject, length_reference[2], width_reference;
	
	FAM = _parameters->anatomy->FAM_landmark_coord;
	TAM = _parameters->anatomy->TAM_landmark_coord;
	FM2 = _parameters->anatomy->FM2_landmark_coord;
	FLE = _parameters->anatomy->FLE_landmark_coord;
	FME = _parameters->anatomy->FME_landmark_coord;
	hip_COR = _parameters->anatomy->hip_rotation_centre_coord;
	RASIS = _parameters->anatomy->RASIS_landmark_coord;
	RPSIS = _parameters->anatomy->RPSIS_landmark_coord;
	LASIS = _parameters->anatomy->LASIS_landmark_coord;
	LPSIS = _parameters->anatomy->LASIS_landmark_coord; 


	// Foot: LCS to GCS using reference body landmarks
	for (int ii=0; ii<3; ii++)
	{
		axis_y[ii] = (FAM[ii]+TAM[ii])/2-FM2[ii];
		temp_axis_z[ii] = FAM[ii] - TAM[ii];
	}
	length_reference[0]=MATHEMATICS::math_vecmag(axis_y);
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

	for (int j=0; j<4; j++) 
	{	
		calibrate_pos[0]->rot_lcs_to_horsman[j]=q[j];
	}

	// Shank: LCS to GCS using reference body landmarks
	for (int ii=0; ii<3; ii++)
	{
		axis_y[ii] = (FLE[ii]+FME[ii])/2 - (FAM[ii]+TAM[ii])/2;
		temp_axis_z[ii] = FLE[ii] - FME[ii];
	}

	length_reference[1]=MATHEMATICS::math_vecmag(axis_y);
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

	for (int j=0; j<4; j++) 
	{	
		calibrate_pos[1]->rot_lcs_to_horsman[j]=q[j];
	}

	// Thigh: LCS to GCS using reference body landmarks
	for (int ii=0; ii<3; ii++)
	{
		axis_y[ii] = hip_COR[ii] - (FLE[ii]+FME[ii])/2;
		temp_axis_z[ii] = FLE[ii] - FME[ii];
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

	for (int j=0; j<4; j++) 
	{	
		calibrate_pos[2]->rot_lcs_to_horsman[j]=q[j];
	}

	// Pelvis: LCS to GCS using reference body landmarks
	for (int ii=0; ii<3; ii++)
	{
		temp_axis_x[ii] = RASIS[ii] - RPSIS[ii];
		axis_z[ii] = RASIS[ii] - LASIS[ii];
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

	for (int j=0; j<4; j++) 
	{	
		calibrate_pos[3]->rot_lcs_to_horsman[j]=q[j];
	}

	// Patella: LCS to GCS, the same as thigh's
	for (int j=0; j<4; j++) 
	{	
		calibrate_pos[4]->rot_lcs_to_horsman[j]=calibrate_pos[2]->rot_lcs_to_horsman[j];
	}


	for (int i=0; i<2; i++) 
	{
		calibrate_pos[i]->scaling_factors[1]=calibrate_pos[i]->length/length_reference[i];

	}


	if (_parameters->subject_sex == _parameters->anatomy->anatomy_sex && _parameters->subject_height == _parameters->anatomy->anatomy_height && _parameters->subject_mass == _parameters->anatomy->anatomy_mass)
	{
		for (int ii=0;  ii<segments; ii++)
		{
			calibrate_pos[ii]->scaling_factors[0] = 1.0;
			calibrate_pos[ii]->scaling_factors[1] = 1.0;
			calibrate_pos[ii]->scaling_factors[2] = 1.0;
		}
	}
	
	return;
}