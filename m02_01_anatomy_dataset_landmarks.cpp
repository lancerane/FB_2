#include "stdafx.h"
#include "m02_01_anatomy_dataset_landmarks.h"



using namespace std;

/*

Specify the proximal/distal ends of each segment
Scaling for the hip_COR had been modified by ZIYUN 08/03/2016

*/

void m02_01_anatomy_dataset_landmarks(Structure *calibrate_pos[], int segments, double marker_radius, XML_Parameters* _parameters) 
{

	Vec_DP v0(3), v1(3), v2(3), midasis(3),  pelvis_end(3);
	Vec_DP RASIS(3), LASIS(3),RPSIS(3), FLE(3), hip_COR(3);
	Vec_DP anatomy_length_vec(3), subject_length_vec(3);
	double reference_pelvis[3][3], reference_hipcentre[3];

	// Move FM2 in calibration position downward 
	calibrate_pos[0]->landmarks[1][4]-=marker_radius;		// Adjust landmark for radius of marker

	// Centres of rotation of joints used to define ends of intermediate segments
	for (int i=0; i<segments-3; i++) 
	{
		for (int j=0; j<3; j++) 
		{
			calibrate_pos[i]->landmarks[1][j]=(calibrate_pos[i]->landmarks[2][j]+calibrate_pos[i]->landmarks[2][j+3])/2;
			calibrate_pos[i+1]->landmarks[1][j+3]=calibrate_pos[i]->landmarks[1][j];
		}
	}


	// Position of hip joint centre
	// Create LCS defined by pelvis markers
	// Calculate hip centre in reference (e.g. Horsman) pelvis frame
	RASIS = _parameters->anatomy->RASIS_landmark_coord;
	LASIS = _parameters->anatomy->LASIS_landmark_coord;
	RPSIS = _parameters->anatomy->RPSIS_landmark_coord;
	hip_COR = _parameters->anatomy->hip_rotation_centre_coord;
	FLE = _parameters->anatomy->FLE_landmark_coord;
	
	
	for (int ii=0; ii<3; ii++)
	{
		reference_pelvis[0][ii] = RASIS[ii]-RPSIS[ii];
		reference_pelvis[1][ii] = hip_COR[ii];
		reference_pelvis[2][ii] = RASIS[ii]-LASIS[ii];
		reference_hipcentre[ii] = hip_COR[ii]-(RASIS[ii]+LASIS[ii])/2;
	}

		Vec_DP x(3), y(3), z(3);
		Vec_DP hipc(reference_hipcentre,3);

		Mat_DP rot(3,3);		// rot is the rotation from the global frame to the local pelvis frame

		for (int k=0; k<3; k++) 
		{
			x[k]=reference_pelvis[0][k];
			z[k]=reference_pelvis[2][k];
		}

		double anatomy_width=MATHEMATICS::math_vecmag(z);

		x=MATHEMATICS::math_vecnorm(x);
		z=MATHEMATICS::math_vecnorm(z);

		y=MATHEMATICS::math_crossprd(z,x);
		x=MATHEMATICS::math_crossprd(y,z);

		x=MATHEMATICS::math_vecnorm(x);
		y=MATHEMATICS::math_vecnorm(y);
		z=MATHEMATICS::math_vecnorm(z);


		// G TO L
		for (int k=0; k<3; k++) 
		{
			rot[0][k]=x[k];	
			rot[1][k]=y[k];
			rot[2][k]=z[k];
		}

		// For anatomy dataset, hipc is the hip COR in the pelvis coordinate system
		hipc=MATHEMATICS::math_mxmply(rot,hipc);

		
		
		// Calculate position of hip centre in the calibration frame using markers in the static trial
		for (int k=0; k<3; k++) 
		{
			//RASIS in calibration position-RPSIS in calibration position
			x[k]=calibrate_pos[segments-2]->landmarks[0][k]-calibrate_pos[segments-2]->landmarks[0][k+3];
			//RASIS in calibration position-LASIS in calibration position
			z[k]=calibrate_pos[segments-2]->landmarks[2][k]-calibrate_pos[segments-2]->landmarks[2][k+3];
			midasis[k]=(calibrate_pos[segments-2]->landmarks[2][k]+calibrate_pos[segments-2]->landmarks[2][k+3])/2;
		}

		// MOVE middle point of RASIS and LASIS backward
		midasis[0]-=marker_radius;		// Adjust landmark for width of marker

		// distance between RASIS and LASIS in calibration position
		double subject_width=MATHEMATICS::math_vecmag(z);

		x=MATHEMATICS::math_vecnorm(x);
		z=MATHEMATICS::math_vecnorm(z);

		y=MATHEMATICS::math_crossprd(z,x);
		x=MATHEMATICS::math_crossprd(y,z);

		x=MATHEMATICS::math_vecnorm(x);
		y=MATHEMATICS::math_vecnorm(y);
		z=MATHEMATICS::math_vecnorm(z);
			
		// L TO G
		for (int k=0; k<3; k++) 
		{
			rot[k][0]=x[k];
			rot[k][1]=y[k];
			rot[k][2]=z[k];
		}

		double scal_width=subject_width/anatomy_width;

		for (int k=0; k<3; k++)
		{
			subject_length_vec[k] = (calibrate_pos[3]->landmarks[0][k]+calibrate_pos[3]->landmarks[0][k+3])/2-calibrate_pos[2]->landmarks[2][k];
			anatomy_length_vec[k] = (RASIS[k] + RPSIS[k])/2-FLE[k];
		}

		double subject_length = MATHEMATICS::math_vecmag(subject_length_vec);  
		double anatomy_length = MATHEMATICS::math_vecmag(anatomy_length_vec);  

		double scal_length=subject_length/anatomy_length;

		calibrate_pos[2]->scaling_factors[1] = calibrate_pos[3]->scaling_factors[1]  = scal_length;
		calibrate_pos[4]->scaling_factors[0] = calibrate_pos[3]->scaling_factors[1] ;

		for(int ii=0; ii<segments-1; ii++)
		{
			calibrate_pos[ii]->scaling_factors[0]=scal_width;
			calibrate_pos[ii]->scaling_factors[2]=scal_width;
		
		}

		calibrate_pos[4]->scaling_factors[1] = calibrate_pos[4]->scaling_factors[2] = scal_width;

	    
		if (_parameters->subject_sex == _parameters->anatomy->anatomy_sex && _parameters->subject_height == _parameters->anatomy->anatomy_height && _parameters->subject_mass == _parameters->anatomy->anatomy_mass)
		{
			for (int ii=0;  ii<segments; ii++)			
			{
				scal_length = 1.0;
				scal_width = 1.0;
			}
		}

		// scalling the local hip COR to match subject's local hip COR
		hipc[0]=scal_width*hipc[0];
		hipc[1]=scal_length*hipc[1];
		hipc[2]=scal_width*hipc[2];


		for (int ii=0; ii<3; ii++)
		{
			pelvis_end[ii] = hipc[ii];
		}
		 
		hipc=MATHEMATICS::math_mxmply(rot,hipc);

		for (int j=0; j<3; j++) 
		{
			calibrate_pos[segments-3]->landmarks[1][j]=hipc[j]+midasis[j];
		}

		
		// Define origins of segments

		for (int i=0; i<segments-2; i++) 
		{
			for (int j=0; j<3; j++) 
			{
				calibrate_pos[i]->origin[j]=calibrate_pos[i]->landmarks[1][j];
			}
		}

		for (int i=0; i<3; i++) 
		{
			calibrate_pos[3]->origin[i]=midasis[i];
		}
		// Define origins of segments

		// Calculate segment lengths, positions of COM, and distal points

		for (int i=0; i<segments-2; i++) 
		{
			for (int j=0; j<3; j++) 
			{
				v0[j]=calibrate_pos[i]->landmarks[1][j]-calibrate_pos[i]->landmarks[1][j+3];
			}
	
			calibrate_pos[i]->length=MATHEMATICS::math_vecmag(v0);

			calibrate_pos[i]->com[1]=-calibrate_pos[i]->length*calibrate_pos[i]->com[1];
			calibrate_pos[i]->distal[1]=-calibrate_pos[i]->length;

			calibrate_pos[i]->com[0]=calibrate_pos[i]->com[2]=0;
			calibrate_pos[i]->distal[0]=calibrate_pos[i]->distal[2]=0;
		}


		for (int ii=0; ii<3; ii++)
		{
			calibrate_pos[3]->distal[ii] = pelvis_end[ii];
		}


	return;
}