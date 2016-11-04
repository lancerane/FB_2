#include "stdafx.h"
#include "m02_07_tf_contact.h"



using namespace std;

/*



*/

void m02_07_tf_contact(Segment **segment_data[], Structure *calibrate_pos[], int frames, XML_Parameters* _parameters)
{

	Vec_DP v0(3), v1(3), v2(3), v3(3);

	Vec_DP FLE(3), FME(3), trans_tf_lat(3), trans_tf_med(3), tf_lat_gcs(3), tf_med_gcs(3);

	FLE = _parameters->anatomy->FLE_landmark_coord;
	FME = _parameters->anatomy->FME_landmark_coord;
	tf_lat_gcs =  _parameters->anatomy->tf_lat_contact_coord;
	tf_med_gcs =  _parameters->anatomy->tf_med_contact_coord;
		 
	if (_parameters->anatomy->tf_lat_frame == "global")
	for (int ii=0; ii<3; ii++)
	{
		trans_tf_lat[ii] = tf_lat_gcs[ii] - (FLE[ii]+FME[ii])/2;
	}

	if (_parameters->anatomy->tf_med_frame == "global")
	for (int ii=0; ii<3; ii++)
	{
		trans_tf_med[ii] = tf_med_gcs[ii] -(FLE[ii]+FME[ii])/2;
	}


	v0=trans_tf_lat;
	v1=trans_tf_med;



	v0=calibrate_pos[1]->struc_rot_hors_to_lcs(v0);
	v1=calibrate_pos[1]->struc_rot_hors_to_lcs(v1);




	//////if it is implant knee, do not scaling

	for (int i=0; i<3; i++) 
	{

		v0[i]=v0[i]*calibrate_pos[1]->scaling_factors[i];
		v1[i]=v1[i]*calibrate_pos[1]->scaling_factors[i];
	}

	for (int i=0; i<frames; i++) 
	{

		v2=segment_data[i][1]->seg_pos_point_on_seg(v0);
		v3=segment_data[i][1]->seg_pos_point_on_seg(v1);

		for (int j=0; j<3; j++) 
		{
			// fixed in the tibia
			//LAT
			segment_data[i][1]->vec_rot_centre_to_rf[2][j]=v2[j]-segment_data[i][1]->rot_centre[j];
			//MED
			segment_data[i][1]->vec_rot_centre_to_rf[3][j]=v3[j]-segment_data[i][1]->rot_centre[j];
			// fixed in the femur
			//LAT
			segment_data[i][2]->vec_rot_centre_to_rf[2][j]=v2[j]-segment_data[i][2]->rot_centre[j];
			//MED
			segment_data[i][2]->vec_rot_centre_to_rf[3][j]=v3[j]-segment_data[i][2]->rot_centre[j];
		}
	}
	return;
}