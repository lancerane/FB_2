#include "stdafx.h"
#include "m05_02_horsman_muscle_model.h"



using namespace std;

/*
ZY: Read muscle model, if wrapping, save the wrapping object information
Specify the muscle model parameters based on the reference's muscle model


*/

void m05_02_horsman_muscle_model(Muscle *muscle_model[], Structure *calibrate_pos[], int muscles, string filename_model, XML_Parameters* _parameters)
{

	int pnts, segment, wrap_num;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	Read the wrapping cylinder parameters from XML file
	Written by Ziyun Ding, Imperial College (c) 2014,
	*/
	int wrap_object_num = _parameters->anatomy->wrapping_object_num; 
	Vec_DP RASIS(3), LASIS(3), hip_COR(3), FLE(3), FME(3), FAM(3), TAM(3),  patella_ten_origin(3), local_point(3);
	Vec_DP v0(3), v1(3);
	Mat_DP cylinders_info(wrap_object_num,9);
	Vec_DP Z_axis(3);
	double pt_scale_unit;
	Z_axis[0] = 0; Z_axis[1] = 0; Z_axis[2] = 1; 


    double thera, scaling_factor, betha;

	RASIS = _parameters->anatomy->RASIS_landmark_coord;
	LASIS = _parameters->anatomy->LASIS_landmark_coord;
	hip_COR = _parameters->anatomy->hip_rotation_centre_coord;
	FLE = _parameters->anatomy->FLE_landmark_coord;
	FME = _parameters->anatomy->FME_landmark_coord;
	TAM = _parameters->anatomy->TAM_landmark_coord;
	FAM = _parameters->anatomy->FAM_landmark_coord;
	patella_ten_origin = _parameters->anatomy->patella_origin_coord;
	pt_scale_unit = _parameters->anatomy->musl_pt_scale_units;


	for (int ii=0; ii<wrap_object_num; ii++)
	{   

		if (_parameters->anatomy->wrapping_object[ii].coord_flag == "global")
		{

			if (_parameters->anatomy->wrapping_object[ii].wrapping_segment_flag == 0) //foot
			{
				for(int kk=0;kk<3;kk++)
				{
					local_point[kk] = _parameters->anatomy->wrapping_object[ii].point_on_cnetral_axis_coord[kk]-(FAM[kk]+TAM[kk])/2;
				}
				
			}
			if (_parameters->anatomy->wrapping_object[ii].wrapping_segment_flag == 1) //shank
			{
				for(int kk=0;kk<3;kk++)
				{
					local_point[kk] = _parameters->anatomy->wrapping_object[ii].point_on_cnetral_axis_coord[kk]-(FLE[kk]+FME[kk])/2;
				}

				
			}
			if (_parameters->anatomy->wrapping_object[ii].wrapping_segment_flag == 2) //thigh
			{
				for(int kk=0;kk<3;kk++)
				{
					local_point[kk] = _parameters->anatomy->wrapping_object[ii].point_on_cnetral_axis_coord[kk]-hip_COR[kk];
				}
				
			}
			if (_parameters->anatomy->wrapping_object[ii].wrapping_segment_flag == 3) //pelvis
			{
				for(int kk=0;kk<3;kk++)
				{
					local_point[kk] = _parameters->anatomy->wrapping_object[ii].point_on_cnetral_axis_coord[kk]-(RASIS[kk]+LASIS[kk])/2;
				}
				//cout<<local_point<<endl;
			}
			if (_parameters->anatomy->wrapping_object[ii].wrapping_segment_flag == 4) //patella
			{
				for(int kk=0;kk<3;kk++)
				{
					local_point[kk] = _parameters->anatomy->wrapping_object[ii].point_on_cnetral_axis_coord[kk]-patella_ten_origin[kk];
				}
				//cout<<local_point<<endl;
			}
		}
		else 
		{ 
			local_point =  _parameters->anatomy->wrapping_object[ii].point_on_cnetral_axis_coord;  
			//cout<<local_point<<endl; 
		}


		for (int jj=0; jj<3; jj++)
		{
			cylinders_info[_parameters->anatomy->wrapping_object[ii].wrapping_object_id][jj] = local_point[jj];
			cylinders_info[_parameters->anatomy->wrapping_object[ii].wrapping_object_id][jj+3] = _parameters->anatomy->wrapping_object[ii].central_axis_coord[jj];
		}

		cylinders_info[_parameters->anatomy->wrapping_object[ii].wrapping_object_id][6] = _parameters->anatomy->wrapping_object[ii].radius;
		cylinders_info[_parameters->anatomy->wrapping_object[ii].wrapping_object_id][7] = _parameters->anatomy->wrapping_object[ii].wrapping_direction_flag;
		cylinders_info[_parameters->anatomy->wrapping_object[ii].wrapping_object_id][8] = _parameters->anatomy->wrapping_object[ii].wrapping_segment_flag;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// Input muscle model parameters in reference laboratory frame, e.g., Horsman's

	io_datainput(filename_model,muscles,muscle_model);

	// Translate and Rotate parameters to LCS

	for (int i=0; i<muscles; i++) 
	{

		pnts=muscle_model[i]->pnts;

		for (int j=0; j<pnts; j++) 
		{								// This routine iterates for each muscle point

			for (int k=0; k<3; k++) 
			{
				v0[k]=muscle_model[i]->gblpnts[j][k]*pt_scale_unit;
			}
			segment=int (muscle_model[i]->gblpnts[j][3]);			// Determine the location of each muscle point


			if (segment == 0)
			{
				for (int k=0; k<3; k++) 
				{
					v0[k]= v0[k]-(FAM[k]+TAM[k])/2;
				}
			}

			else if (segment == 1)
			{
				for (int k=0; k<3; k++) 
				{
					v0[k]= v0[k]-(FLE[k]+FME[k])/2;
				}

			}

			else if (segment == 2)
			{
				for (int k=0; k<3; k++) 
				{
					v0[k]= v0[k]-hip_COR[k];
				}
			}

			else if (segment == 3)
			{
				for (int k=0; k<3; k++) 
				{
					v0[k]= v0[k]-(RASIS[k]+LASIS[k])/2;
				}
			}

			else if (segment == 4)
			{
				for (int k=0; k<3; k++) 
				{
					v0[k]= v0[k]-patella_ten_origin[k];
				}
			}
											
			v1=calibrate_pos[segment]->struc_rot_hors_to_lcs(v0);	// Transform the muscle point to the LCS
			

			for (int k=0; k<3; k++) 
			{
				muscle_model[i]->mslpnts[j][k]=v1[k]*calibrate_pos[segment]->scaling_factors[k];	// Scale the muscle points
				muscle_model[i]->mslpnts[j][k+3]=muscle_model[i]->gblpnts[j][k+3];
			}	
		 
			
			if (muscle_model[i]->mslpnts[j][4] == 1) 
			{				// Determine wrapping cylinders
				
				
				//cout<<muscle_model[i]->name;

				wrap_num=int (muscle_model[i]->mslpnts[j][5]);

				
	
				for (int k=0; k<3; k++) 
				{

					v0[k]=cylinders_info[wrap_num][k];
					v1[k]=cylinders_info[wrap_num][k+3];
				}



				v0=calibrate_pos[segment]->struc_rot_hors_to_lcs(v0);	// Transform the muscle point to the LCS
				v1=calibrate_pos[segment]->struc_rot_hors_to_lcs(v1);	// Transform the muscle point to the LCS



				for (int k=0; k<3; k++) 
				{

					muscle_model[i]->wrap[0][k]=v0[k]*calibrate_pos[segment]->scaling_factors[k];	// Scale wrapping points
					muscle_model[i]->wrap[1][k]=v1[k];
					
				}

				

				// Calculate the scalling factor for wrapping object radius
				// Find the angle between Z axis and the object central axis
				thera=MATHEMATICS::math_dotprd(Z_axis,v1);

				thera=thera/MATHEMATICS::math_vecmag(v1);


				if (calibrate_pos[segment]->scaling_factors[0]<calibrate_pos[segment]->scaling_factors[1])
				{
					scaling_factor = calibrate_pos[segment]->scaling_factors[0]; 
				}
				else
				{
					scaling_factor = thera*calibrate_pos[segment]->scaling_factors[1]; 
				}


				muscle_model[i]->wrap[2][0]=cylinders_info[wrap_num][6]*scaling_factor; // Scale radius of wrapping cylinder


				muscle_model[i]->wrap[2][1]=cylinders_info[wrap_num][7];                                            //ZY: the direction that the muscle wraps around the cylinder
				muscle_model[i]->wrap[2][2]=cylinders_info[wrap_num][8];											//ZY: the segment which the cylinder attached
				

				if (wrap_num == 2) 
				{				// Determine wrapping cylinders
	
					for (int k=0; k<3; k++) 
					{

						muscle_model[i]->wrap[0][k]=calibrate_pos[2]->landmarks_local[0][k];
					}

				}
			}
		
		
		}

	}

	return;
}