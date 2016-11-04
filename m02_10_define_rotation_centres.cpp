#include "stdafx.h"
#include "m02_10_define_rotation_centres.h"
#include "mathematics.h"


using namespace std;

/*



*/

void m02_10_define_rotation_centres(Segment **segment_data[], Structure *calibrate_pos[], int frames, int segments, XML_Parameters* _parameters)

{
	
	Vec_DP v0(3), v1(3), constraint_knee_COR(3);
	Vec_DP FAM(3), TAM(3), FLE(3), FME(3),  ankle_COR(3), knee_COR(3), hip_COR(3), RASIS(3), LASIS(3);
	Vec_DP fixed_hip_COR(3);
	Mat_DP reference_centres(5,3);
	int tf_constraint;

	MATHEMATICS::math_zeromatrix(reference_centres);

	FAM = _parameters->anatomy->FAM_landmark_coord;
	TAM = _parameters->anatomy->TAM_landmark_coord;
	FLE = _parameters->anatomy->FLE_landmark_coord;
	FME = _parameters->anatomy->FME_landmark_coord;
	RASIS = _parameters->anatomy->RASIS_landmark_coord;
	LASIS = _parameters->anatomy->LASIS_landmark_coord;
	knee_COR = _parameters->anatomy->knee_rotation_centre_coord;
	ankle_COR = _parameters->anatomy->ankle_rotation_centre_coord;
	hip_COR = _parameters->anatomy->hip_rotation_centre_coord;


	//// when segment_number == 2,3,4, the rotation centre was set as the origin of its local segment
	for (int ii=0; ii<3; ii++)
	{
		reference_centres[0][ii] = ankle_COR[ii]-(FAM[ii]+TAM[ii])/2;
		reference_centres[1][ii] = knee_COR[ii]-(FLE[ii]+FME[ii])/2;
		constraint_knee_COR[ii] = knee_COR[ii]-hip_COR[ii];
	
	}

	//tf_constraint = 1;

	//ZY comment: static rot_centre and dynamic rot_centre
	for (int i=0; i<5; i++) 
	{
		
			for (int k=0; k<3; k++) 
			{
					
				v0[k]=reference_centres[i][k];
			}

			v0=calibrate_pos[i]->struc_rot_hors_to_lcs(v0);	


			///////////////if it is implant knee, do nont scale knee centre //////////////////
			/*if (i==1)
			{
				for (int k=0; k<3; k++) 
				{

					
					calibrate_pos[i]->rot_centre[k]=v0[k];
				}	
			}
			else
			{
				for (int k=0; k<3; k++) 
				{

					v0[k]=v0[k]*calibrate_pos[i]->scaling_factors[k];
					calibrate_pos[i]->rot_centre[k]=v0[k];
				}	

			}*/
			///////////////if it is implant knee, do nont scale knee centre //////////////////

			for (int k=0; k<3; k++) 
			{

				v0[k]=v0[k]*calibrate_pos[i]->scaling_factors[k];
				calibrate_pos[i]->rot_centre[k]=v0[k];
			}	



			for (int l=0; l<frames; l++) 
			{

				v1=segment_data[l][i]->seg_pos_point_on_seg(v0);

				for (int k=0; k<3; k++) 
				{
						
					segment_data[l][i]->rot_centre[k]=v1[k];
				}
				
			}
	}


	for(int l=0; l<frames; l++) 
	{
		// define dis_contact 
		for (int k=0; k<3; k++) 
		{		
			// for foot segment 
			segment_data[l][0]->dist_contact[k]=segment_data[l][0]->distal[k];
			// for shank segment 
			//cout<<segment_data[l][1]->dist<<endl;
			segment_data[l][1]->dist_contact[k]=segment_data[l][segment_data[l][1]->dist]->rot_centre[k];
			// for thigh segment
			//cout<<segment_data[l][2]->dist<<endl;
			segment_data[l][2]->dist_contact[k]=segment_data[l][segment_data[l][2]->dist]->rot_centre[k];
			// for pelvis segment
			//cout<<segment_data[l][3]->dist<<endl;
			segment_data[l][3]->dist_contact[k]=segment_data[l][segment_data[l][3]->dist]->rot_centre[k];

		}

		// define vec_rot_centre_to_rf
		for (int k=0; k<3; k++) 
		{
			// for foot segment
			segment_data[l][0]->vec_rot_centre_to_rf[0][k]=segment_data[l][0]->dist_contact[k]-segment_data[l][0]->rot_centre[k];
			// for shank segment
			segment_data[l][1]->vec_rot_centre_to_rf[0][k]=segment_data[l][1]->dist_contact[k]-segment_data[l][1]->rot_centre[k];
			// for thigh segment
			segment_data[l][2]->vec_rot_centre_to_rf[0][k]=segment_data[l][2]->dist_contact[k]-segment_data[l][2]->rot_centre[k];
		}

	}

	////////////////////////////////////////////////////////////////////////
	// ZY test: test if shank and foot meet in the ankle 
	/*Vec_DP ankle_in_foot_lcs(3),  ankle_in_shank_lcs(3), ankle_gcs_from_foot_lcs(3), ankle_gcs_from_shank_lcs(3), ankle_hors_foot(3), ankle_hors_shank(3), foot_origin(3), shank_origin(3);

	for (int ii=0; ii<3; ii++)
	{
		 ankle_in_foot_lcs[ii] = ankle_COR[ii]-(FAM[ii]+TAM[ii])/2;
		 ankle_in_shank_lcs[ii] = ankle_COR[ii]-(FLE[ii]+FME[ii])/2;
	}

	//cout<<ankle_in_shank_lcs<<endl;


	ankle_hors_foot = calibrate_pos[0]->struc_rot_hors_to_lcs(ankle_in_foot_lcs);
	ankle_hors_shank = calibrate_pos[1]->struc_rot_hors_to_lcs(ankle_in_shank_lcs);

	//cout<<ankle_hors_shank<<endl;

	for (int k=0; k<3; k++) 
	{

		ankle_hors_foot[k]=ankle_hors_foot[k]*calibrate_pos[0]->scaling_factors[k];
		ankle_hors_shank[k]=ankle_hors_shank[k]*calibrate_pos[1]->scaling_factors[k]; 
				
	}	

	//cout<<ankle_hors_shank<<endl;


	// test the 5st frame
	ankle_gcs_from_foot_lcs =segment_data[5][0]->seg_pos_point_on_seg(ankle_hors_foot);
	ankle_gcs_from_shank_lcs =segment_data[5][1]->seg_pos_point_on_seg(ankle_hors_shank);

	//foot_origin = segment_data[5][0]->origin;
	//shank_origin = segment_data[5][0]->origin;

	//cout<<"ankle_gcs_from_foot_lcs:"<<ankle_gcs_from_foot_lcs<<endl;
	//cout<<"ankle_gcs_from_shank_lcs:"<<ankle_gcs_from_shank_lcs<<endl;*/


	////////////////////////////////////////////////////////////////////////

	return;
}