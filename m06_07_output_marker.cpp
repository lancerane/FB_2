#include "stdafx.h"
#include "m06_07_output_marker.h"
#include "io_dataoutput.h"


using namespace std;

/*



*/

void m06_07_output_marker(Segment **segment_data[], Structure *calibrate_pos[], int frames, int segments, string output_folder_for_visualisation, string study_name)
{

	int marker_count, calib_marker_num, last, last_cal, last_vir;

	
	
	Vec_DP dyn_q(4), dyn_trans(3), vir_marker(3), v0(3), v1(3);

	marker_count = 0;
	calib_marker_num = 0;
	last = 0;
	last_cal = 0;
	last_vir = 0;

	for (int ii=0; ii<4; ii++)
	{
		marker_count = marker_count+segment_data[0][ii]->markers;
	}

	for (int ii=0; ii<4; ii++)
	{
		calib_marker_num = calib_marker_num+calibrate_pos[ii]->calib_marker;
	}

	Mat_DP dynamic_marker(frames,3*marker_count), static_marker(1,3*calib_marker_num), virtual_static_marker(frames, 3*calib_marker_num), ex_forces(frames,6);

	
	///////	 Get markers in the dynamic process ///////	
	for (int tt=0; tt<frames; tt++)
	{
		for (int ss=0; ss<4; ss++)
		{
			
			int seg_marker = segment_data[tt][ss]->markers;
			
			for (int nn=0; nn<seg_marker; nn++)

			{
				for (int ii=0; ii<3; ii++)
				{
					dynamic_marker[tt][(last+nn)*3+ii] = segment_data[tt][ss]->marker_pos[nn][ii];
				}
				  
			}
		   
			last = last+seg_marker;	
		}

		last = 0;
		
	}
	




	for (int tt=0; tt<frames; tt++)
	{
		for (int ii=0; ii<6; ii++)
		{
			ex_forces[tt][ii] = segment_data[tt][0]->external_forces[0][ii];
		}
		
	}
	
	/////// Get markers in the calibration process ///////
	for (int ss=0; ss<4; ss++)
	{
			
		int seg_marker = calibrate_pos[ss]->calib_marker;
			
		for (int nn=0; nn<seg_marker; nn++)

		{
			for (int ii=0; ii<3; ii++)
			{
				static_marker[0][(last_cal+nn)*3+ii] = calibrate_pos[ss]->static_marker[nn][ii];
			}
				  
		}
		   
		last_cal = last_cal+seg_marker;	
	}

	

	/////// Get virtual dynamic markers for those markers in the calibration process ///////
	for (int tt=0; tt<frames; tt++)
	{

		for (int ss=0; ss<4; ss++)
		{

			for (int ii=0; ii<4; ii++) 
			{
				dyn_q[ii] = segment_data[tt][ss]->rot_cal_to_gcs[ii];
			}

			//cout<<dyn_q<<endl;

			for (int ii=0; ii<3; ii++)
			{
				dyn_trans[ii] = segment_data[tt][ss]->trans_cal_to_gcs[ii];
			}

			//cout<<dyn_trans<<endl;

			int seg_marker = calibrate_pos[ss]->calib_marker;

			for (int nn=0; nn<seg_marker; nn++)
			{

				for (int ii=0; ii<3; ii++)
				{
					v0[ii] = calibrate_pos[ss]->static_marker[nn][ii];
				}

				v1=KINEMATICS::kin_qrotate(v0,dyn_q);

				//cout<<v1;

				for (int ii=0; ii<3; ii++)
				{
					v1[ii] = v1[ii]+dyn_trans[ii];

				}

				//cout<<v1;

				for (int ii=0; ii<3; ii++)
				{
					segment_data[tt][ss]->virtual_marker_pos[nn][ii] = v1[ii];
				
				}
 
			}

		}

	}

	// cout virtual dynamic markers
	for (int tt=0; tt<frames; tt++)
	{
		for (int ss=0; ss<4; ss++)
		{
			
			int seg_marker = calibrate_pos[ss]->calib_marker;;
			
			for (int nn=0; nn<seg_marker; nn++)

			{
				for (int ii=0; ii<3; ii++)
				{
					virtual_static_marker[tt][(last_vir+nn)*3+ii] = segment_data[tt][ss]->virtual_marker_pos[nn][ii];
				}
				  
			}
		   
			last_vir = last_vir+seg_marker;	
		}

		last_vir = 0;
		
	}
	

	io_dataoutput(output_folder_for_visualisation + "\\" + study_name + "_"+ "dynamic_marker.csv",dynamic_marker);
	io_dataoutput(output_folder_for_visualisation + "\\" + study_name + "_"+ "static_marker.csv",static_marker);
	io_dataoutput(output_folder_for_visualisation + "\\" + study_name + "_"+ "virtual_static_marker.csv",virtual_static_marker);
	io_dataoutput(output_folder_for_visualisation + "\\" + study_name + "_"+ "external_forces.csv",ex_forces);
	
	
	return;

}