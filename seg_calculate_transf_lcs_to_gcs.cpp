#pragma once
#include "stdafx.h"
#include "Segment.h"
#include "mathematics.h"
#include "kinematics.h"

/*

Segment class definition

*/
/*

Segment class constructor

*/
void Segment::seg_calculate_transf_lcs_to_gcs(Structure *calibrate_pos[]) 
{
	


	Vec_DP q0(4), q1(4), v0(3), v1(3);
	

	int seg_num = this->segnum;

	for (int i=0; i<4; i++) 
	{
		q0[i]=this->rot_cal_to_gcs[i];                                          // ZY: variable matrix represents the transformation in the dynamic process
		q1[i]=calibrate_pos[seg_num]->rot_lcs_to_gcs[i];                   // ZY: constant matrix represents the transformation in the calibration process
	}

	for (int i=0; i<3; i++) 
	{

		
		v0[i]=this->trans_cal_to_gcs[i];   
		                                    
		v1[i]=calibrate_pos[seg_num]->origin[i];
	}



	v1=KINEMATICS::kin_qrotate(v1,q0);											

	q0=MATHEMATICS::math_qmply(q0,q1);

	
	for (int i=0; i<4; i++) 
	{
		this->rot_lcs_to_gcs[i]=q0[i];                    
	}
	for (int i=0; i<3; i++) 
	{
		this->origin[i]=v0[i]+v1[i];
	}

}
