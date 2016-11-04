#include "stdafx.h"
#include "m06_05_output_bony_geometry.h"
#include "io_dataoutput.h"





using namespace std;

/*



*/

void m06_05_output_bony_geometry(Segment **segment_data[], Muscle *patellar_tendon[], Structure *calibrate_pos[], int frames, int segments,string output_folder_for_visualisation, string study_name)
{

	Mat_DP out(frames,3*segments), patella(frames,12), scaling_factors(segments,3), rot_centres(frames,3*(segments-2)), tf_joint(frames,6), seg_distal(frames,12);


	Mat_DP pelvis_distal(frames,3), thigh_distal(frames, 3);



	for (int i=0; i<frames; i++) 
	{

		for (int j=0; j<segments; j++) 
		{

			for (int k=0; k<3; k++) 
			{

				out[i][3*j+k]=segment_data[i][j]->origin[k];
			}

		}
	}

	for (int i=0; i<frames; i++) 
	{

		for (int j=0; j<4; j++) 
		{

			for (int k=0; k<3; k++) 
			{

				seg_distal[i][3*j+k]=segment_data[i][j]->distal[k];
			}

		}
	}
		
	string filename0=output_folder_for_visualisation + "\\" + study_name + "_"+"origins.csv";

	io_dataoutput(filename0,out);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i=0; i<frames; i++) 
	{

		for (int k=0; k<3; k++) 
		{

			pelvis_distal[i][k]=segment_data[i][3]->distal[k];
			thigh_distal[i][k]=segment_data[i][2]->distal[k];
		}

	}

	string filename_pelvis_distal, file_name_thigh_distal;
	filename_pelvis_distal=output_folder_for_visualisation + "\\" + study_name + "_"+"pelvis_distal.csv";
	//file_name_thigh_distal=output_folder_for_visualisation + "\\" + study_name + "_"+"thigh_distal.csv";

	io_dataoutput(filename_pelvis_distal,pelvis_distal);
	//io_dataoutput(file_name_thigh_distal,thigh_distal);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////


	for (int i=0; i<frames; i++) {

		for (int k=0; k<3; k++) {

			patella[i][k]=segment_data[i][4]->other_points[0][k];

			patella[i][k+3]=segment_data[i][4]->origin[k];
			patella[i][k+6]=patellar_tendon[i]->gblpnts[0][k];
			patella[i][k+9]=patellar_tendon[i]->gblpnts[1][k];
		}

	}
		
	string filename_pat=output_folder_for_visualisation + "\\" + study_name + "_"+"patella.csv";

	io_dataoutput(filename_pat,patella);


	for (int i=0; i<segments; i++) {
		for (int k=0; k<3; k++) {

			scaling_factors[i][k]=calibrate_pos[i]->scaling_factors[k];
		}
	}
		
	string filename_sf=output_folder_for_visualisation + "\\" + study_name + "_"+"scaling_factors.csv";

	io_dataoutput(filename_sf,scaling_factors);

	for (int i=0; i<frames; i++) {

		for (int j=0; j<segments-2; j++) {

			for (int k=0; k<3; k++) {

				rot_centres[i][3*j+k]=segment_data[i][j]->rot_centre[k];
			}

		}
	}
		
	string filename_rc=output_folder_for_visualisation +"\\" + study_name + "_"+ "rot_centres_gcs.csv";

	io_dataoutput(filename_rc,rot_centres);

	for (int i=0; i<frames; i++) 
	{
		for (int k=0; k<3; k++) 
		{
			
			tf_joint[i][k]=segment_data[i][2]->rot_centre[k]+segment_data[i][2]->vec_rot_centre_to_rf[2][k];
			tf_joint[i][k+3]=segment_data[i][2]->rot_centre[k]+segment_data[i][2]->vec_rot_centre_to_rf[3][k];
		}
	}
		
	string filename_tf=output_folder_for_visualisation +"\\" + study_name + "_"+ "tf_contact_gcs.csv";

	io_dataoutput(filename_tf,tf_joint);

	return;
}